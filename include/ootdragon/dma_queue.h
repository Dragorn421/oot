#ifndef DMA_QUEUE_H
#define DMA_QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct dma_request {
    void* ram;
    uint32_t rom;
    size_t size;
    bool finished;
    struct dma_request* next;
};

void dma_queue_read(void* ram, uint32_t rom, size_t size, struct dma_request* req);
bool dma_queue_finished(struct dma_request* req);
void dma_queue_wait(struct dma_request* req);

#endif
