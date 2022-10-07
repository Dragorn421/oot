#include "global.h"

void FlagSet_Update(PlayState* play) {
    static s32 entryIdx = 0;
    static u32 curBit = 0;
    static s32 timer = 0;
    static s32 bitIdx; // ? this doesn't need to be static

    FlagSetEntry entries[53] = {
        { &GET_EVENTCHKINF_A0(0), "event_chk_inf[0]" },   { &GET_EVENTCHKINF_A0(1), "event_chk_inf[1]" },
        { &GET_EVENTCHKINF_A0(2), "event_chk_inf[2]" },   { &GET_EVENTCHKINF_A0(3), "event_chk_inf[3]" },
        { &GET_EVENTCHKINF_A0(4), "event_chk_inf[4]" },   { &GET_EVENTCHKINF_A0(5), "event_chk_inf[5]" },
        { &GET_EVENTCHKINF_A0(6), "event_chk_inf[6]" },   { &GET_EVENTCHKINF_A0(7), "event_chk_inf[7]" },
        { &GET_EVENTCHKINF_A0(8), "event_chk_inf[8]" },   { &GET_EVENTCHKINF_A0(9), "event_chk_inf[9]" },
        { &GET_EVENTCHKINF_A0(10), "event_chk_inf[10]" }, { &GET_EVENTCHKINF_A0(11), "event_chk_inf[11]" },
        { &GET_EVENTCHKINF_A0(12), "event_chk_inf[12]" }, { &GET_EVENTCHKINF_A0(13), "event_chk_inf[13]" },
        { &GET_ITEMGETINF_A0(0), "item_get_inf[0]" },     { &GET_ITEMGETINF_A0(1), "item_get_inf[1]" },
        { &GET_ITEMGETINF_A0(2), "item_get_inf[2]" },     { &GET_ITEMGETINF_A0(3), "item_get_inf[3]" },
        { &GET_INFTABLE_A0(0), "inf_table[0]" },          { &GET_INFTABLE_A0(1), "inf_table[1]" },
        { &GET_INFTABLE_A0(2), "inf_table[2]" },          { &GET_INFTABLE_A0(3), "inf_table[3]" },
        { &GET_INFTABLE_A0(4), "inf_table[4]" },          { &GET_INFTABLE_A0(5), "inf_table[5]" },
        { &GET_INFTABLE_A0(6), "inf_table[6]" },          { &GET_INFTABLE_A0(7), "inf_table[7]" },
        { &GET_INFTABLE_A0(8), "inf_table[8]" },          { &GET_INFTABLE_A0(9), "inf_table[9]" },
        { &GET_INFTABLE_A0(10), "inf_table[10]" },        { &GET_INFTABLE_A0(11), "inf_table[11]" },
        { &GET_INFTABLE_A0(12), "inf_table[12]" },        { &GET_INFTABLE_A0(13), "inf_table[13]" },
        { &GET_INFTABLE_A0(14), "inf_table[14]" },        { &GET_INFTABLE_A0(15), "inf_table[15]" },
        { &GET_INFTABLE_A0(16), "inf_table[16]" },        { &GET_INFTABLE_A0(17), "inf_table[17]" },
        { &GET_INFTABLE_A0(18), "inf_table[18]" },        { &GET_INFTABLE_A0(19), "inf_table[19]" },
        { &GET_INFTABLE_A0(20), "inf_table[20]" },        { &GET_INFTABLE_A0(21), "inf_table[21]" },
        { &GET_INFTABLE_A0(22), "inf_table[22]" },        { &GET_INFTABLE_A0(23), "inf_table[23]" },
        { &GET_INFTABLE_A0(24), "inf_table[24]" },        { &GET_INFTABLE_A0(25), "inf_table[25]" },
        { &GET_INFTABLE_A0(26), "inf_table[26]" },        { &GET_INFTABLE_A0(27), "inf_table[27]" },
        { &GET_INFTABLE_A0(28), "inf_table[28]" },        { &GET_INFTABLE_A0(29), "inf_table[29]" },
        { &GET_EVENTINF_A0(0), "event_inf[0]" },          { &GET_EVENTINF_A0(1), "event_inf[1]" },
        { &GET_EVENTINF_A0(2), "event_inf[2]" },          { &GET_EVENTINF_A0(3), "event_inf[3]" },
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
