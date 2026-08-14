#ifndef LIBULTRA_UCODE_H
#define LIBULTRA_UCODE_H

#include "ultra64/sptask.h"
#include <stdbool.h>

struct libultra_ucode_task_handle {
    OSTask* task;
    bool uses_rdp;
    void (*cb)(void* cb_arg);
    void* cb_arg;
    bool rsp_finished;
    bool rdp_finished;
    struct libultra_ucode_task_handle* next;
};

void libultra_ucode_init(void);
void libultra_ucode_run_with_callback(OSTask* task, bool uses_rdp, void (*cb)(void* cb_arg), void* cb_arg,
                                      struct libultra_ucode_task_handle* task_handle);
void libultra_ucode_run(OSTask* task, bool uses_rdp, struct libultra_ucode_task_handle* task_handle);
void libultra_ucode_wait(struct libultra_ucode_task_handle* task_handle);
void libultra_ucode_wait_all(void);

#endif
