/**
 * @file padmgr.c
 *
 * This file implements communicating with joybus devices at a high level and serving the results to other threads.
 *
 * Any device that can be plugged into one of the four controller ports such as a standard N64 controller is a joybus
 * device. Some joybus devices are also located inside the cartridge such as EEPROM for save data or the Real-Time
 * Clock, however neither of these are used in Zelda64 and so this type of communication is unimplemented. Of the
 * possible devices that can be plugged into the controller ports, the only device that padmgr will recognize and
 * attempt to communicate with is the standard N64 controller.
 *
 * Communicating with these devices is broken down into various layers:
 *
 * Other threads                    : The rest of the program that will use the polled data
 *  |
 * PadMgr                           : Manages devices, submits polling commands at vertical retrace
 *  |
 * Libultra osCont* routines        : Interface for building commands and safely using the Serial Interface
 *  |
 * Serial Interface                 : Hardware unit for sending joybus commands and receiving data via DMA
 *  |
 * PIF                              : Forwards joybus commands and receives response data from the devices
 *  |---¬---¬---¬-------¬
 *  1   2   3   4       5           : The joybus devices plugged into the four controller ports or on the cartridge
 *
 * Joybus communication is handled on another thread as polling and receiving controller data is a slow process; the
 * N64 programming manual section 26.2.4.1 quotes 2 milliseconds as the expected delay from calling
 * `osContStartReadData` to receiving the data. By running this on a separate thread to the game state, work can be
 * done while waiting for this operation to complete.
 */
#include <libdragon.h>
#include "game_controller.h"
#include "joypad.h"
#include "libu64/debug.h"
#include "array_count.h"
#include "padmgr.h"
#include "printf.h"
#include "terminal.h"
#include "translation.h"
#include "line_numbers.h"

#define PADMGR_LOG(controllerNum, msg)                                                                \
    if (DEBUG_FEATURES) {                                                                             \
        PRINTF_COLOR_YELLOW();                                                                        \
        PRINTF(T("padmgr: %dコン: %s\n", "padmgr: Controller %d: %s\n"), (controllerNum) + 1, (msg)); \
        PRINTF_RST();                                                                                 \
    }                                                                                                 \
    (void)0

#define LOG_SEVERITY_NOLOG 0
#define LOG_SEVERITY_CRITICAL 1
#define LOG_SEVERITY_ERROR 2
#define LOG_SEVERITY_VERBOSE 3

s32 gPadMgrLogSeverity = LOG_SEVERITY_CRITICAL;

/**
 * Immediately stops rumble on all controllers
 */
void PadMgr_RumbleStop(PadMgr* padMgr) {
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        joypad_set_rumble_active(i, false);
    }
}

/**
 * Enables or disables rumble on all controller ports for 240 VI,
 * ~4 seconds at 60 VI/sec and ~4.8 seconds at 50 VI/sec
 *
 * @param enable Array of u8 of length MAXCONTROLLERS containing either true or false to enable or disable rumble
 *               for that controller
 */
void PadMgr_RumbleSet(PadMgr* padMgr, u8* enable) {
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        joypad_set_rumble_active(i, enable[i]);
    }
}

/**
 * Fetches the most recently polled inputs from padmgr
 *
 * @param inputs   Array of Input of length MAXCONTROLLERS to copy inputs into
 * @param gamePoll True if polling inputs for updating the game state
 */
void PadMgr_RequestPadData(PadMgr* padMgr, Input* inputs, s32 gameRequest) {
    s32 i;
    Input* inputIn;
    Input* inputOut;
    s32 buttonDiff;

    joypad_poll();

    for (inputIn = &padMgr->inputs[0], inputOut = &inputs[0], i = 0; i < MAXCONTROLLERS; i++, inputIn++, inputOut++) {
        joypad_inputs_t joypad_input_cur = joypad_get_inputs(i);
        OSContPad input_cur = {
            (joypad_input_cur.btn.a ? BTN_A : 0) | (joypad_input_cur.btn.b ? BTN_B : 0) |
                (joypad_input_cur.btn.c_down ? BTN_CDOWN : 0) | (joypad_input_cur.btn.c_left ? BTN_CLEFT : 0) |
                (joypad_input_cur.btn.c_right ? BTN_CRIGHT : 0) | (joypad_input_cur.btn.c_up ? BTN_CUP : 0) |
                (joypad_input_cur.btn.d_right ? BTN_DRIGHT : 0) | (joypad_input_cur.btn.d_down ? BTN_DDOWN : 0) |
                (joypad_input_cur.btn.d_left ? BTN_DLEFT : 0) | (joypad_input_cur.btn.d_up ? BTN_DUP : 0) |
                (joypad_input_cur.btn.l ? BTN_L : 0) | (joypad_input_cur.btn.r ? BTN_R : 0) |
                (joypad_input_cur.btn.z ? BTN_Z : 0) | (joypad_input_cur.btn.start ? BTN_START : 0),
            joypad_input_cur.stick_x,
            joypad_input_cur.stick_y,
        };

        if (gameRequest) {
            // The following mimicks the (removed) PadMgr_UpdateInputs code
            // but one important difference is PadMgr_UpdateInputs ran every retrace,
            // whereas this function only runs every graph update
            // TODO-ootdragon does this matter?
            inputIn->prev = inputIn->cur;
            inputIn->cur = input_cur;
            // Calculate pressed and relative inputs
            buttonDiff = inputIn->prev.button ^ inputIn->cur.button;
            inputIn->press.button |= (u16)(buttonDiff & inputIn->cur.button);
            inputIn->rel.button |= (u16)(buttonDiff & inputIn->prev.button);
            PadUtils_UpdateRelXY(inputIn);
            inputIn->press.stick_x += (s8)(inputIn->cur.stick_x - inputIn->prev.stick_x);
            inputIn->press.stick_y += (s8)(inputIn->cur.stick_y - inputIn->prev.stick_y);

            // Copy inputs as-is, press and rel are calculated prior in `PadMgr_UpdateInputs`
            *inputOut = *inputIn;
            // Zero parts of the press and rel inputs in the polled inputs so they are not read more than once
            inputIn->press.button = 0;
            inputIn->press.stick_x = 0;
            inputIn->press.stick_y = 0;
            inputIn->rel.button = 0;
        } else {
            // Take as the previous inputs the inputs that are currently in the destination array
            inputOut->prev = inputOut->cur;
            // Copy current inputs from the polled inputs
            inputOut->cur = input_cur;
            // Calculate press and rel from these
            buttonDiff = inputOut->prev.button ^ inputOut->cur.button;
            inputOut->press.button = inputOut->cur.button & buttonDiff;
            inputOut->rel.button = inputOut->prev.button & buttonDiff;
            PadUtils_UpdateRelXY(inputOut);
            inputOut->press.stick_x += (s8)(inputOut->cur.stick_x - inputOut->prev.stick_x);
            inputOut->press.stick_y += (s8)(inputOut->cur.stick_y - inputOut->prev.stick_y);
        }
    }
}

void PadMgr_Init(PadMgr* padMgr) {
    PRINTF(T("パッドマネージャ作成 padmgr_Create()\n", "Pad Manager creation padmgr_Create()\n"));

    bzero(padMgr, sizeof(PadMgr));

    joypad_init();
}
