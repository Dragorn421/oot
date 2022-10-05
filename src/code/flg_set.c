#include "global.h"

void FlagSet_Update(PlayState* play) {
    static s32 entryIdx = 0;
    static u32 curBit = 0;
    static s32 timer = 0;
    static s32 bitIdx; // ? this doesn't need to be static

    FlagSetEntry entries[53] = {
        { &GET_EVENT_CHK_INF[0], "event_chk_inf[0]" },
        { &GET_EVENT_CHK_INF[1], "event_chk_inf[1]" },
        { &GET_EVENT_CHK_INF[2], "event_chk_inf[2]" },
        { &GET_EVENT_CHK_INF[3], "event_chk_inf[3]" },
        { &GET_EVENT_CHK_INF[4], "event_chk_inf[4]" },
        { &GET_EVENT_CHK_INF[5], "event_chk_inf[5]" },
        { &GET_EVENT_CHK_INF[6], "event_chk_inf[6]" },
        { &GET_EVENT_CHK_INF[7], "event_chk_inf[7]" },
        { &GET_EVENT_CHK_INF[8], "event_chk_inf[8]" },
        { &GET_EVENT_CHK_INF[9], "event_chk_inf[9]" },
        { &GET_EVENT_CHK_INF[10], "event_chk_inf[10]" },
        { &GET_EVENT_CHK_INF[11], "event_chk_inf[11]" },
        { &GET_EVENT_CHK_INF[12], "event_chk_inf[12]" },
        { &GET_EVENT_CHK_INF[13], "event_chk_inf[13]" },
        { &GET_ITEM_GET_INF[0], "item_get_inf[0]" },
        { &GET_ITEM_GET_INF[1], "item_get_inf[1]" },
        { &GET_ITEM_GET_INF[2], "item_get_inf[2]" },
        { &GET_ITEM_GET_INF[3], "item_get_inf[3]" },
        { &GET_INF_TABLE[0], "inf_table[0]" },
        { &GET_INF_TABLE[1], "inf_table[1]" },
        { &GET_INF_TABLE[2], "inf_table[2]" },
        { &GET_INF_TABLE[3], "inf_table[3]" },
        { &GET_INF_TABLE[4], "inf_table[4]" },
        { &GET_INF_TABLE[5], "inf_table[5]" },
        { &GET_INF_TABLE[6], "inf_table[6]" },
        { &GET_INF_TABLE[7], "inf_table[7]" },
        { &GET_INF_TABLE[8], "inf_table[8]" },
        { &GET_INF_TABLE[9], "inf_table[9]" },
        { &GET_INF_TABLE[10], "inf_table[10]" },
        { &GET_INF_TABLE[11], "inf_table[11]" },
        { &GET_INF_TABLE[12], "inf_table[12]" },
        { &GET_INF_TABLE[13], "inf_table[13]" },
        { &GET_INF_TABLE[14], "inf_table[14]" },
        { &GET_INF_TABLE[15], "inf_table[15]" },
        { &GET_INF_TABLE[16], "inf_table[16]" },
        { &GET_INF_TABLE[17], "inf_table[17]" },
        { &GET_INF_TABLE[18], "inf_table[18]" },
        { &GET_INF_TABLE[19], "inf_table[19]" },
        { &GET_INF_TABLE[20], "inf_table[20]" },
        { &GET_INF_TABLE[21], "inf_table[21]" },
        { &GET_INF_TABLE[22], "inf_table[22]" },
        { &GET_INF_TABLE[23], "inf_table[23]" },
        { &GET_INF_TABLE[24], "inf_table[24]" },
        { &GET_INF_TABLE[25], "inf_table[25]" },
        { &GET_INF_TABLE[26], "inf_table[26]" },
        { &GET_INF_TABLE[27], "inf_table[27]" },
        { &GET_INF_TABLE[28], "inf_table[28]" },
        { &GET_INF_TABLE[29], "inf_table[29]" },
        { &GET_EVENT_INF[0], "event_inf[0]" },
        { &GET_EVENT_INF[1], "event_inf[1]" },
        { &GET_EVENT_INF[2], "event_inf[2]" },
        { &GET_EVENT_INF[3], "event_inf[3]" },
    };

    GraphicsContext* gfxCtx = play->state.gfxCtx;
    Input* input = &play->state.input[0];
    Gfx* gfx;
    Gfx* polyOpa;

    OPEN_DISPS(gfxCtx, "../flg_set.c", 131);

    {
        GfxPrint printer;
        s32 pad;

        polyOpa = POLY_OPA_DISP;
        gfx = Graph_GfxPlusOne(polyOpa);
        gSPDisplayList(OVERLAY_DISP++, gfx);

        GfxPrint_Init(&printer);
        GfxPrint_Open(&printer, gfx);
        GfxPrint_SetColor(&printer, 250, 50, 50, 255);
        GfxPrint_SetPos(&printer, 4, 13);
        GfxPrint_Printf(&printer, entries[entryIdx].name);
        GfxPrint_SetPos(&printer, 4, 15);

        for (bitIdx = 15; bitIdx >= 0; bitIdx--) {
            if ((u32)bitIdx == curBit) {
                GfxPrint_SetColor(&printer, 200, 200, 200, 255);
            } else {
                GfxPrint_SetColor(&printer, 100, 100, 100, 255);
            }

            if (*entries[entryIdx].value & (1 << bitIdx)) {
                GfxPrint_Printf(&printer, "1");
            } else {
                GfxPrint_Printf(&printer, "0");
            }

            if ((bitIdx % 4) == 0) {
                GfxPrint_Printf(&printer, " ");
            }
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DLEFT)) {
            timer = 10;
            curBit++;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_DRIGHT)) {
            curBit--;
            timer = 10;
        }

        if (timer == 0) {
            if (CHECK_BTN_ALL(input->cur.button, BTN_DLEFT)) {
                curBit++;
                timer = 2;
            }
            if (CHECK_BTN_ALL(input->cur.button, BTN_DRIGHT)) {
                curBit--;
                timer = 2;
            }
        }

        curBit %= 16;
        if (CHECK_BTN_ALL(input->press.button, BTN_DUP)) {
            entryIdx--;
            if (entryIdx < 0) {
                entryIdx = 0;
            }
            timer = 10;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_DDOWN)) {
            timer = 10;
            entryIdx++;
            if (!entries[entryIdx].value) {
                entryIdx--;
            }
        }

        if (timer == 0) {
            if (CHECK_BTN_ALL(input->cur.button, BTN_DUP)) {
                entryIdx--;
                timer = 2;
                if (entryIdx < 0) {
                    entryIdx = 0;
                }
            }
            if (CHECK_BTN_ALL(input->cur.button, BTN_DDOWN)) {
                timer = 2;
                entryIdx++;
                if (!entries[entryIdx].value) {
                    entryIdx--;
                }
            }
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_A)) {
            *entries[entryIdx].value ^= (1 << curBit);
        }

        if (timer != 0) {
            timer--;
        }

        gfx = GfxPrint_Close(&printer);
        GfxPrint_Destroy(&printer);

        gSPEndDisplayList(gfx++);
        Graph_BranchDlist(polyOpa, gfx);
        POLY_OPA_DISP = gfx;
    }

    if (CHECK_BTN_ALL(input->press.button, BTN_L)) {
        play->pauseCtx.debugState = 0;
    }

    CLOSE_DISPS(gfxCtx, "../flg_set.c", 241);
}
