#include "sys_debug_controller.h"
#include "joypad.h"
#include "stdbool.h"
#include "ultra64.h"
#include "padmgr.h"

u32 gIsCtrlr2Valid = false;

void func_800D31F0(void) {
    gIsCtrlr2Valid = joypad_is_connected(JOYPAD_PORT_2);
}

void func_800D3210(void) {
    gIsCtrlr2Valid = false;
}
