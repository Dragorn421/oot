#include "libultra_ucode.h"
#include <libdragon.h>
#include <stdbool.h>

void libultra_ucode_SP_interrupt_handler(void) {
}
void libultra_ucode_DP_interrupt_handler(void) {
}

void libultra_ucode_init(void) {
    set_SP_interrupt(true);
    set_DP_interrupt(true);

    register_SP_handler(libultra_ucode_SP_interrupt_handler);
    register_DP_handler(libultra_ucode_DP_interrupt_handler);
}

void libultra_ucode_run(OSTask* task, struct libultra_ucode_task_handle* task_handle) {
}

void libultra_ucode_wait(struct libultra_ucode_task_handle* task_handle) {
}

void libultra_ucode_wait_all(void) {
}
