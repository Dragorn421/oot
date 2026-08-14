#ifndef LIBULTRA_UCODE_H
#define LIBULTRA_UCODE_H

#include "ultra64/sptask.h"

struct libultra_ucode_task_handle {
    struct libultra_ucode_task_handle* next;
};

void libultra_ucode_init(void);
void libultra_ucode_run(OSTask* task, struct libultra_ucode_task_handle* task_handle);
void libultra_ucode_wait(struct libultra_ucode_task_handle* task_handle);
void libultra_ucode_wait_all(void);

#endif
