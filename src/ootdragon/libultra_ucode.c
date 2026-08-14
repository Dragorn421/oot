#include "libultra_ucode.h"
#include "n64sys.h"
#include <libdragon.h>
#include <stdbool.h>

static OSTask sTmpTask;

OSTask* _VirtualToPhysicalTask(OSTask* intp) {
    OSTask* tp = &sTmpTask;

    memcpy(tp, intp, sizeof(OSTask));

#define _osVirtualToPhysical(ptr)                                       \
    if (ptr != NULL) {                                                  \
        /* compared to libultra this doesn't handle TLB but whatever */ \
        ptr = (void*)PhysicalAddr(ptr);                                 \
    }

    _osVirtualToPhysical(tp->t.ucode);
    _osVirtualToPhysical(tp->t.ucode_data);
    _osVirtualToPhysical(tp->t.dram_stack);
    _osVirtualToPhysical(tp->t.output_buff);
    _osVirtualToPhysical(tp->t.output_buff_size);
    _osVirtualToPhysical(tp->t.data_ptr);
    _osVirtualToPhysical(tp->t.yield_data_ptr);

    return tp;
}

void libultra_ucode_send_task(OSTask* task) {
    OSTask* tp = _VirtualToPhysicalTask(task);

    if (tp->t.flags & OS_TASK_YIELDED) {
        tp->t.ucode_data = tp->t.yield_data_ptr;
        tp->t.ucode_data_size = tp->t.yield_data_size;
        task->t.flags &= ~OS_TASK_YIELDED;

        if (tp->t.flags & OS_TASK_LOADABLE) {
            assert(false);
            // tp->t.ucode = (u64 *)IO_READ((u32)task->t.yield_data_ptr + OS_YIELD_DATA_SIZE - 4);
        }
    }

    // writeback the OSTask as well as any other data the RSP/RDP may consume
    data_cache_writeback_invalidate_all();

    *SP_STATUS = SP_WSTATUS_CLEAR_SIG0 | SP_WSTATUS_CLEAR_SIG1 | SP_WSTATUS_CLEAR_SIG2 | SP_WSTATUS_SET_INTR_BREAK;

    assert(*SP_STATUS & SP_STATUS_HALTED);
    *SP_PC = 0x80;
    assert(sizeof(OSTask) == 0x40);
    assert(tp->t.ucode_data_size <= 0xFC0);
    rsp_load_data(tp->t.ucode_data, tp->t.ucode_data_size, 0);
    rsp_load_data(tp, sizeof(OSTask), 0xFC0);
    // rsp_load_code(tp->t.ucode_boot, 0x80, 0); // useless
    rsp_load_code(tp->t.ucode, 0xF80, 0x80);
    *SP_STATUS = SP_WSTATUS_SET_INTR_BREAK | SP_WSTATUS_CLEAR_SSTEP | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_CLEAR_HALT;
}

struct libultra_ucode_task_handle* task_list;
struct libultra_ucode_task_handle* cur_task;

bool libultra_ucode_finished(volatile struct libultra_ucode_task_handle* task_handle) {
    return (task_handle->uses_rdp ? task_handle->rdp_finished : true) && task_handle->rsp_finished;
}

void libultra_ucode_next_task(void) {
    disable_interrupts();
    assert(cur_task == NULL);
    if (task_list != NULL) {
        cur_task = task_list;
        libultra_ucode_send_task(cur_task->task);
    }
    enable_interrupts();
}

void libultra_ucode_check_cur_task_done(void) {
    if (libultra_ucode_finished(cur_task)) {
        struct libultra_ucode_task_handle* done_task = cur_task;
        cur_task = NULL;
        assert(task_list == done_task);
        if (done_task->cb != NULL) {
            done_task->cb(done_task->cb_arg);
        }
        task_list = done_task->next;
        libultra_ucode_next_task();
    }
}

void libultra_ucode_SP_interrupt_handler(void) {
    cur_task->rsp_finished = true;
    libultra_ucode_check_cur_task_done();
}

void libultra_ucode_DP_interrupt_handler(void) {
    cur_task->rdp_finished = true;
    libultra_ucode_check_cur_task_done();
}

void libultra_ucode_init(void) {
    set_SP_interrupt(true);
    set_DP_interrupt(true);

    register_SP_handler(libultra_ucode_SP_interrupt_handler);
    register_DP_handler(libultra_ucode_DP_interrupt_handler);
}

void libultra_ucode_run_with_callback(OSTask* task, bool uses_rdp, void (*cb)(void* cb_arg), void* cb_arg,
                                      struct libultra_ucode_task_handle* task_handle) {
    task_handle->task = task;
    task_handle->uses_rdp = uses_rdp;
    task_handle->cb = cb;
    task_handle->cb_arg = cb_arg;
    task_handle->rsp_finished = false;
    task_handle->rdp_finished = false;

    disable_interrupts();
    if (task_list == NULL) {
        task_list = task_handle;
        libultra_ucode_next_task();
    } else {
        struct libultra_ucode_task_handle* iter = task_list;
        while (iter->next != NULL) {
            iter = iter->next;
        }
        iter->next = task_list;
    }
    enable_interrupts();
}

void libultra_ucode_run(OSTask* task, bool uses_rdp, struct libultra_ucode_task_handle* task_handle) {
    libultra_ucode_run_with_callback(task, uses_rdp, NULL, NULL, task_handle);
}

void libultra_ucode_wait(struct libultra_ucode_task_handle* task_handle) {
    disable_interrupts();

    if (libultra_ucode_finished(task_handle)) {
        enable_interrupts();
        return;
    }

#ifndef NDEBUG
    struct libultra_ucode_task_handle* iter = task_list;
    bool found = false;
    while (iter != NULL) {
        if (iter == task_handle) {
            found = true;
            break;
        }
        iter = iter->next;
    }
    assertf(found, "cannot wait for a task not in queue");
#endif

    enable_interrupts();

    volatile struct libultra_ucode_task_handle* vtask_handle = task_handle;
    RSP_WAIT_LOOP(500) {
        if (libultra_ucode_finished(vtask_handle)) {
            break;
        }
    }
}

void libultra_ucode_wait_all(void) {
    struct libultra_ucode_task_handle* last_task_handle;

    disable_interrupts();
    last_task_handle = task_list;
    if (last_task_handle != NULL) {
        while (last_task_handle->next != NULL) {
            last_task_handle = last_task_handle->next;
        }
    }
    enable_interrupts();

    if (last_task_handle != NULL) {
        libultra_ucode_wait(last_task_handle);
    }
}
