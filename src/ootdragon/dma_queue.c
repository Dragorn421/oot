#include "dma_queue.h"
#include "interrupt.h"
#include <libdragon.h>

struct dma_request* dma_requests_list = NULL;
struct dma_request* cur_dma_request = NULL;

void dma_queue_next_request(void) {
    disable_interrupts();
    assert(cur_dma_request == NULL);
    if (dma_requests_list != NULL) {
        cur_dma_request = dma_requests_list;
        data_cache_hit_writeback_invalidate(cur_dma_request->ram, cur_dma_request->size);
        dma_read_async(cur_dma_request->ram, cur_dma_request->rom, cur_dma_request->size);
    }
    enable_interrupts();
}

void dma_queue_PI_interrupt_handler(void) {
    if (cur_dma_request != NULL) {
        assert(dma_requests_list == cur_dma_request);
        dma_requests_list = dma_requests_list->next;
        cur_dma_request->finished = true;
        cur_dma_request = NULL;
        dma_queue_next_request();
    }
}

void dma_queue_init(void) {
    register_PI_handler(dma_queue_PI_interrupt_handler);
    set_PI_interrupt(true);
}

void dma_queue_read(void* ram, uint32_t rom, size_t size, struct dma_request* req) {
    disable_interrupts();

    req->ram = ram;
    req->rom = rom;
    req->size = size;
    req->finished = false;
    req->next = NULL;

    if (dma_requests_list == NULL) {
        dma_requests_list = req;

        assert(cur_dma_request == NULL);
        dma_queue_next_request();
    } else {
        struct dma_request* iter = dma_requests_list;

        while (iter->next != NULL) {
            iter = iter->next;
        }
        iter->next = req;

        assert(cur_dma_request != NULL);
    }

    enable_interrupts();
}

bool dma_queue_finished(struct dma_request* req) {
    return req->finished;
}

void dma_queue_wait(struct dma_request* req) {
    disable_interrupts();

    if (req->finished) {
        enable_interrupts();
        return;
    }

#ifndef NDEBUG
    struct dma_request* iter = dma_requests_list;
    bool found = false;
    while (iter != NULL) {
        if (iter == req) {
            found = true;
            break;
        }
        iter = iter->next;
    }
    assertf(found, "cannot wait for a request not in queue");
#endif

    enable_interrupts();

    volatile struct dma_request* vreq = req;
    while (!vreq->finished) {}
}
