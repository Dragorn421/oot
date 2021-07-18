#include "global.h"

#define ENTRANCE(scene, spawn, continueBgm, displayTitleCard, fadeIn, fadeOut)                                     \
    {                                                                                                              \
        scene, spawn,                                                                                              \
            ((continueBgm & 1) << 15) | ((displayTitleCard & 1) << 14) | ((fadeIn & 0x7F) << 7) | (fadeOut & 0x7F) \
    }

EntranceInfo gEntranceTable[] = {
    ENTRANCE(0x00, 0x00, 0, 1, 0x02, 0x02), ENTRANCE(0x00, 0x00, 0, 1, 0x02, 0x02),
    ENTRANCE(0x00, 0x00, 0, 1, 0x02, 0x02), ENTRANCE(0x00, 0x00, 0, 1, 0x02, 0x02),
};

#define TITLED_SCENE(name, title, unk_10, config, unk_12)                                            \
    {                                                                                                \
        (u32) _##name##SegmentRomStart, (u32)_##name##SegmentRomEnd, (u32)_##title##SegmentRomStart, \
            (u32)_##title##SegmentRomEnd, unk_10, config, unk_12, 0                                  \
    }

#define UNTITLED_SCENE(name, unk_10, config, unk_12) \
    { (u32) _##name##SegmentRomStart, (u32)_##name##SegmentRomEnd, 0, 0, unk_10, config, unk_12, 0 }

Scene gSceneTable[] = {
    UNTITLED_SCENE(mod_scene, 0, 0, 0),
};

Gfx sDefaultDisplayList[] = {
    gsSPSegment(0x08, gEmptyDL),
    gsSPSegment(0x09, gEmptyDL),
    gsSPSegment(0x0A, gEmptyDL),
    gsSPSegment(0x0B, gEmptyDL),
    gsSPSegment(0x0C, gEmptyDL),
    gsSPSegment(0x0D, gEmptyDL),
    gsDPPipeSync(),
    gsDPSetPrimColor(0, 0, 128, 128, 128, 128),
    gsDPSetEnvColor(128, 128, 128, 128),
    gsSPEndDisplayList(),
};

// Computes next entrance index based on age and day time to set the fade out transition
void func_800994A0(GlobalContext* globalCtx) {
    s16 computedEntranceIndex;

    if (gSaveContext.nightFlag != 0) {
        if (LINK_IS_CHILD) {
            computedEntranceIndex = globalCtx->nextEntranceIndex + 1;
        } else {
            computedEntranceIndex = globalCtx->nextEntranceIndex + 3;
        }
    } else {
        if (LINK_IS_CHILD) {
            computedEntranceIndex = globalCtx->nextEntranceIndex;
        } else {
            computedEntranceIndex = globalCtx->nextEntranceIndex + 2;
        }
    }

    globalCtx->fadeTransition = gEntranceTable[computedEntranceIndex].field & 0x7F; // Fade out
}

// Scene Draw Config 0
void func_80099550(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4725);

    gSPDisplayList(POLY_OPA_DISP++, sDefaultDisplayList);
    gSPDisplayList(POLY_XLU_DISP++, sDefaultDisplayList);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4735);
}

u32 D_8012A2F8[] = { 0x0200BA18, 0x0200CA18 };

// Scene Draw Config 19
void func_800995DC(GlobalContext* globalCtx) {
    u32 gameplayFrames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4763);

    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - (gameplayFrames % 128), (gameplayFrames * 1) % 128,
                                32, 32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    { s32 pad; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A2F8[gSaveContext.nightFlag]));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4783);
}

// Scene Draw Config 28
void func_80099760(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4845);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 2) % 256, 0, 64, 32, 1, 0,
                                (gameplayFrames * 2) % 128, 64, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4859);
}

u32 D_8012A300[] = { 0x02012378, 0x02013378 };
u32 D_8012A308[] = { 0x02011F78, 0x02014778, 0x02014378, 0x02013F78, 0x02014B78, 0x02013B78, 0x02012F78, 0x02012B78 };

// Scene Draw Config 20
void func_80099878(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    s32 pad;
    Gfx* displayListHead = Graph_Alloc(globalCtx->state.gfxCtx, 6 * sizeof(Gfx));

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4905);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A300[gSaveContext.nightFlag]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(D_8012A308[(s32)(gameplayFrames & 14) >> 1]));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 256, 0, 64, 32, 1, 0,
                                (gameplayFrames * 1) % 128, 64, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 2) % 128, 32, 32));

    { s32 pad2[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_OPA_DISP++, 0x0B, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 255, 255, 255, globalCtx->unk_11D30[0]);
    gSPEndDisplayList(displayListHead++);
    gSPSegment(POLY_OPA_DISP++, 0x0C, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 255, 255, 255, globalCtx->unk_11D30[1]);
    gSPEndDisplayList(displayListHead);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 4956);
}

// Scene Draw Config 30
void func_80099BD8(GlobalContext* globalCtx) {
    f32 temp;
    Gfx* displayListHead = Graph_Alloc(globalCtx->state.gfxCtx, 18 * sizeof(Gfx));

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5069);

    temp = globalCtx->unk_11D30[0] / 255.0f;

    gSPSegment(POLY_XLU_DISP++, 0x08, displayListHead);
    gSPSegment(POLY_OPA_DISP++, 0x08, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 255 - (u8)(185.0f * temp), 255 - (u8)(145.0f * temp),
                    255 - (u8)(105.0f * temp), 255);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_XLU_DISP++, 0x09, displayListHead);
    gSPSegment(POLY_OPA_DISP++, 0x09, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 76 + (u8)(6.0f * temp), 76 + (u8)(34.0f * temp), 76 + (u8)(74.0f * temp),
                    255);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0A, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0A, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, globalCtx->unk_11D30[0]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0B, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0B, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 89 + (u8)(166.0f * temp), 89 + (u8)(166.0f * temp),
                    89 + (u8)(166.0f * temp), 255);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, globalCtx->unk_11D30[0]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0C, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0C, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 255 + (u8)(179.0f * temp), 255 + (u8)(179.0f * temp),
                    255 + (u8)(179.0f * temp), 255);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, globalCtx->unk_11D30[0]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0D, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0D, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, globalCtx->unk_11D30[1]);
    gSPEndDisplayList(displayListHead);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5145);

    if (gSaveContext.sceneSetupIndex == 5) {
        D_8015FCF0 = 1;
        D_8015FCF8.x = -20.0f;
        D_8015FCF8.y = 1220.0f;
        D_8015FCF8.z = -684.0f;
        D_8015FD06 = 10;
        D_8015FD08 = 8.0f;
        D_8015FD0C = 200;
    }
}

// Scene Draw Config 31
void func_8009A45C(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5171);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 64, 256, 16));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - (gameplayFrames % 128), (gameplayFrames * 1) % 128,
                                32, 32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(
        POLY_OPA_DISP++, 0x0A,
        Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 127 - (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0B, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0C,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 50) % 2048, 8, 512, 1, 0,
                                (gameplayFrames * 60) % 2048, 8, 512));
    gSPSegment(POLY_OPA_DISP++, 0x0D,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5212);
}

// Scene Draw Config 32
void func_8009A798(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5226);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 2) % 256, 64, 64));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - (gameplayFrames * 1) % 128,
                                (gameplayFrames * 1) % 256, 32, 64, 1, 0, 0, 32, 128));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5264);
}

// Scene Draw Config 33
void func_8009A9DC(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5278);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 256, 32,
                                64, 1, gameplayFrames % 128, (gameplayFrames * 3) % 256, 32, 64));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5301);
}

// Scene Draw Config 48
void func_8009AB98(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5317);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames % 64, 256, 16));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5330);
}

// Scene Draw Config 39
void func_8009ACA8(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5346);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames % 64, 256, 16));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5367);
}

// Scene Draw Config 24
void func_8009AE30(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5384);

    gameplayFrames = globalCtx->gameplayFrames;

    if (globalCtx->sceneNum == SCENE_HAKADAN_BS) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 2) % 128, 0, 32, 32, 1,
                                    (gameplayFrames * 2) % 128, 0, 32, 32));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 2) % 128, 0, 32, 32, 1,
                                    (gameplayFrames * 2) % 128, 0, 32, 32));
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5416);
}

u32 D_8012A328[] = { 0x0200BD20, 0x0200B920 };

// Scene Draw Config 40
void func_8009AFE0(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5490);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x09, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 3) % 128, 32, 32));

    { s32 pad[2]; }

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A328[gSaveContext.nightFlag]));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5507);
}

u32 D_8012A330[] = { 0x02014C30, 0x02015830 };

// Scene Draw Config 23
void func_8009B0FC(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    s32 spB0;
    s32 spAC;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5535);

    if (1) {} // Necessary to match

    spB0 = (globalCtx->unk_11D30[1] >> 8) & 0xFF;
    spAC = globalCtx->unk_11D30[1] & 0xFF;
    gameplayFrames = globalCtx->gameplayFrames;

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A330[gSaveContext.nightFlag]));

    if (spB0 == 1) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, spAC));
    } else if (spB0 < 1) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 255));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 160));
    }

    if (spB0 == 2) {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, spAC));
    } else if (spB0 < 2) {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 255));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 160));
    }

    if (spB0 != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 160));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 3, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 180));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 128, 0, 32, 32, 1, 0, 0,
                                            32, 32, 0, 0, 0, 160 + (s32)((spAC / 200.0f) * 95.0f)));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 3, 0, 32, 32, 1, 0, 0, 32, 32,
                                            0, 0, 0, 185 + (s32)((spAC / 200.0f) * 70.0f)));
    }

    gSPSegment(POLY_XLU_DISP++, 0x0C,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, gameplayFrames * 1, 32, 32, 1,
                                        0, 127 - (gameplayFrames * 1), 32, 32, 0, 0, 0, 128));
    gSPSegment(POLY_XLU_DISP++, 0x0D,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames * 4, 0, 32, 32, 1,
                                        gameplayFrames * 4, 0, 32, 32, 0, 0, 0, 128));

    { s32 pad[2]; }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5644);
}

// Scene Draw Config 29
void func_8009B86C(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5791);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, globalCtx->unk_11D30[0]);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 145);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5808);
}

// Scene Draw Config 34
void func_8009B9BC(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5822);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames % 64, 4, 16));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5836);
}

// Scene Draw Config 35
void func_8009BAA4(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5850);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));

    if (globalCtx->sceneNum == SCENE_HAIRAL_NIWA) {
        gSPSegment(POLY_XLU_DISP++, 0x09,
                   Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 10) % 256, 32, 64));
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5876);
}

// Scene Draw Config 36
void func_8009BC44(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    s8 sp83;

    if (1) {} // Necessary to match

    sp83 = coss((globalCtx->gameplayFrames * 1500) & 0xFFFF) >> 8;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5894);

    gameplayFrames = globalCtx->gameplayFrames;

    if (globalCtx->sceneNum == SCENE_GANON_TOU) {
        gSPSegment(POLY_XLU_DISP++, 0x09,
                   Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 256, 64, 64));
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 255 - (gameplayFrames * 1) % 256, 64, 64, 1, 0,
                                    (gameplayFrames * 1) % 256, 64, 64));
    }

    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 255 - (gameplayFrames * 1) % 128,
                                (gameplayFrames * 1) % 128, 32, 32, 1, (gameplayFrames * 1) % 128,
                                (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    sp83 = (sp83 >> 1) + 192;
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp83, sp83, sp83, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5930);
}

// Screen Shake for Ganon's Tower Collapse
void func_8009BEEC(GlobalContext* globalCtx) {
    s32 var;

    if (globalCtx->gameplayFrames % 128 == 13) {
        var = Quake_Add(ACTIVE_CAM, 2);
        Quake_SetSpeed(var, 10000);
        Quake_SetQuakeValues(var, 4, 0, 0, 0);
        Quake_SetCountdown(var, 127);
    }

    if ((globalCtx->gameplayFrames % 64 == 0) && (Rand_ZeroOne() > 0.6f)) {
        var = Quake_Add(ACTIVE_CAM, 3);
        Quake_SetSpeed(var, 32000.0f + (Rand_ZeroOne() * 3000.0f));
        Quake_SetQuakeValues(var, 10.0f - (Rand_ZeroOne() * 9.0f), 0, 0, 0);
        Quake_SetCountdown(var, 48.0f - (Rand_ZeroOne() * 15.0f));
    }
}

// Scene Draw Config 38
void func_8009C0AC(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    s8 sp7B;

    if (1) {} // Necessary to match

    sp7B = coss((globalCtx->gameplayFrames * 1500) & 0xFFFF) >> 8;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 5968);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 512, 64, 128, 1, 0,
                                511 - (gameplayFrames * 1) % 512, 64, 128));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 256, 32, 64, 1, 0,
                                255 - (gameplayFrames * 1) % 256, 32, 64));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 20) % 2048, 16, 512, 1, 0,
                                (gameplayFrames * 30) % 2048, 16, 512));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    sp7B = (sp7B >> 1) + 192;
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp7B, sp7B, sp7B, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6004);

    if (Flags_GetSwitch(globalCtx, 0x37)) {
        if ((globalCtx->sceneNum == SCENE_GANON_DEMO) || (globalCtx->sceneNum == SCENE_GANON_FINAL) ||
            (globalCtx->sceneNum == SCENE_GANON_SONOGO) || (globalCtx->sceneNum == SCENE_GANONTIKA_SONOGO)) {
            func_8009BEEC(globalCtx);
        }
    }
}

u32 D_8012A338[] = { 0x0200FAC0, 0x0200F8C0 };

// Scene Draw Config 37
void func_8009C3EC(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    if (0) {} // Necessary to match

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6042);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A338[gSaveContext.nightFlag]));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    { s32 pad[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6076);
}

// Scene Draw Config 42
void func_8009C608(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6151);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 64, 256, 16));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 60) % 2048, 8, 512, 1, 0,
                                (gameplayFrames * 50) % 2048, 8, 512));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - (gameplayFrames * 1) % 128, 0, 32, 32, 1,
                                (gameplayFrames * 1) % 128, 0, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0B,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 1023 - (gameplayFrames * 6) % 1024, 16, 256, 1, 0,
                                1023 - (gameplayFrames * 3) % 1024, 16, 256));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6187);
}

// Scene Draw Config 43
void func_8009C8B8(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6201);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 0, 255 - (gameplayFrames * 10) % 256, 32, 64));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6232);
}

// Scene Draw Config 47
void func_8009CAC0(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6249);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6264);
}

u32 D_8012A340[] = { 0x0200F8C0, 0x020100C0 };

// Scene Draw Config 27
void func_8009CC00(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    if (0) {} // Necessary to match

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6290);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A340[gSaveContext.nightFlag]));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    { s32 pad[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6320);
}

Gfx* Gfx_TwoTexScrollPrimColor(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2,
                               u32 x2, u32 y2, s32 width2, s32 height2, s32 r, s32 g, s32 b, s32 a) {
    Gfx* displayList = Graph_Alloc(gfxCtx, 10 * sizeof(Gfx));

    x1 %= 2048;
    y1 %= 2048;
    x2 %= 2048;
    y2 %= 2048;

    gDPTileSync(displayList);
    gDPSetTileSize(displayList + 1, tile1, x1, y1, (x1 + ((width1 - 1) << 2)), (y1 + ((height1 - 1) << 2)));
    gDPTileSync(displayList + 2);
    gDPSetTileSize(displayList + 3, tile2, x2, y2, (x2 + ((width2 - 1) << 2)), (y2 + ((height2 - 1) << 2)));
    gDPSetPrimColor(displayList + 4, 0, 0, r, g, b, a);
    gSPEndDisplayList(displayList + 5);

    return displayList;
}

// Scene Draw Config 50
void func_8009CF84(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6433);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScrollPrimColor(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128,
                                         (gameplayFrames * 1) % 128, 32, 32, 1, gameplayFrames % 128,
                                         (gameplayFrames * 1) % 128, 32, 32, 255, 255, 255,
                                         globalCtx->unk_11D30[0] + 127));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6449);
}

// Scene Draw Config 41
void func_8009D0E8(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6463);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 127 - (gameplayFrames * 4) % 128, 0, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x09, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 5) % 64, 16, 16));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 0, 63 - (gameplayFrames * 2) % 64, 16, 16));
    gSPSegment(
        POLY_XLU_DISP++, 0x0B,
        Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 127 - (gameplayFrames * 3) % 128, 32, 32, 1, 0, 0, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6491);
}

u32 D_8012A348[] = { 0x02005210, 0x02005010 };

// Scene Draw Config 44
void func_8009D31C(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6515);

    { s32 pad[2]; }

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A348[gSaveContext.nightFlag]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6528);
}

u32 D_8012A350[] = { 0x02006550, 0x02003550 };
u32 D_8012A358[] = { 0x02002350, 0x02001350 };

// Scene Draw Config 45
void func_8009D438(GlobalContext* globalCtx) {
    s32 var;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6560);

    if (LINK_IS_ADULT) {
        var = 1;
    } else {
        var = gSaveContext.nightFlag;
    }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A358[var]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(D_8012A350[var]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6581);
}

// Scene Draw Config 46
void func_8009D5B4(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6595);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 1023 - (gameplayFrames * 3) % 1024, 16, 256, 1, 0,
                                1023 - (gameplayFrames * 6) % 1024, 16, 256));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6615);
}

u32 D_8012A360[] = { 0x02014D90, 0x02014590 };

// Scene Draw Config 22
void func_8009D758(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    if (0) {} // Necessary to match

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6640);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A360[gSaveContext.nightFlag]));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    { s32 pad[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6671);
}

u32 D_8012A368[] = { 0x02018920, 0x02018020 };

// Scene Draw Config 25
void func_8009D974(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6752);

    { s32 pad[2]; }

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A368[gSaveContext.nightFlag]));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6762);
}

// Scene Draw Config 1
void func_8009DA30(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    Gfx* displayListHead;

    displayListHead = Graph_Alloc(globalCtx->state.gfxCtx, 3 * sizeof(Gfx));

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6814);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 10) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_XLU_DISP++, 0x0A, displayListHead);

    if ((gSaveContext.dayTime >= 0x4AAC) && (gSaveContext.dayTime <= 0xC555)) {
        gSPEndDisplayList(displayListHead);
    } else {
        if (gSaveContext.dayTime > 0xC555) {
            if (globalCtx->unk_11D30[0] != 255) {
                Math_StepToS(&globalCtx->unk_11D30[0], 255, 5);
            }
        } else if (gSaveContext.dayTime >= 0x4000) {
            if (globalCtx->unk_11D30[0] != 0) {
                Math_StepToS(&globalCtx->unk_11D30[0], 0, 10);
            }
        }

        gDPSetPrimColor(displayListHead++, 0, 0, 255, 255, 255, globalCtx->unk_11D30[0]);
        gSPDisplayList(displayListHead++, &D_03012B20);
        gSPEndDisplayList(displayListHead);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6866);
}

u32 D_8012A370[] = { 0x02015B50, 0x02016B50 };

// Scene Draw Config 2
void func_8009DD5C(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6890);

    { s32 pad[2]; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A370[gSaveContext.nightFlag]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6903);
}

// Scene Draw Config 3
void func_8009DE78(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6917);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 6) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 6) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6948);
}

// Scene Draw Config 4
void func_8009E0B8(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    u8 spA3;
    u16 spA0;
    Gfx* displayListHead;

    spA3 = 128;
    spA0 = 500;
    displayListHead = Graph_Alloc(globalCtx->state.gfxCtx, 6 * sizeof(Gfx));

    if (1) {}
    if (1) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 6965);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 10) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    if (gSaveContext.sceneSetupIndex == 4) {
        spA3 = 255 - (u8)globalCtx->unk_11D30[0];
    } else if (gSaveContext.sceneSetupIndex == 6) {
        spA0 = globalCtx->unk_11D30[0] + 500;
    } else if (((gSaveContext.sceneSetupIndex < 4) || LINK_IS_ADULT) && (gSaveContext.eventChkInf[0] & 0x80)) {
        spA0 = 2150;
    }

    gSPSegment(POLY_OPA_DISP++, 0x0A, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 128, 128, 128, spA3);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_XLU_DISP++, 0x0B, displayListHead);
    gSPSegment(POLY_OPA_DISP++, 0x0B, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 128, 128, 128, spA0 * 0.1f);
    gSPEndDisplayList(displayListHead);

    gSPSegment(POLY_OPA_DISP++, 0x0C,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (s16)(-globalCtx->unk_11D30[0] * 0.02f), 32, 16, 1, 0,
                                (s16)(-globalCtx->unk_11D30[0] * 0.02f), 32, 16));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7044);
}

// Scene Draw Config 5
void func_8009E54C(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7058);

    if ((gSaveContext.sceneSetupIndex > 3) || (LINK_IS_ADULT && !(gSaveContext.eventChkInf[6] & 0x200))) {
        globalCtx->unk_11D30[0] = 87;
    }

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, gameplayFrames, gameplayFrames, 32, 32, 1, 0, 0, 32,
                                        32, 0, 0, 0, globalCtx->unk_11D30[0] + 168));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, -gameplayFrames, -gameplayFrames, 32, 32, 1, 0, 0,
                                        16, 64, 0, 0, 0, globalCtx->unk_11D30[0] + 168));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7097);
}

u32 D_8012A378[] = { 0x02008F98, 0x02008FD8 };

// Scene Draw Config 6
void func_8009E730(GlobalContext* globalCtx) {
    u32 gameplayFrames;
    u32 var;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7123);

    gameplayFrames = globalCtx->gameplayFrames;
    var = 127 - (gameplayFrames * 1) % 128;
    if (LINK_IS_ADULT) {
        var = 0;
    }
    gSPSegment(POLY_OPA_DISP++, 0x0C, Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 64, 32, 1, 0, var, 64, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A378[gSaveContext.nightFlag]));

    { s32 pad[2]; }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7147);
}

// Scene Draw Config 7
void func_8009E8C0(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7161);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 1) % 128, 0, 32, 32, 1, 0, 0, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 255 - (gameplayFrames * 2) % 256, 64, 64, 1, 0,
                                255 - (gameplayFrames * 2) % 256, 64, 64));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7192);
}

// Scene Draw Config 8
void func_8009EAD8(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7206);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 3) % 1024, 32, 256, 1, 0,
                                (gameplayFrames * 3) % 1024, 32, 256));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 256, 64, 64, 1, 0,
                                (gameplayFrames * 1) % 256, 64, 64));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 2) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 2) % 128, 32, 32));
    gSPSegment(
        POLY_OPA_DISP++, 0x0B,
        Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 127 - (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0C,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0D,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 64, 16, 16, 1, 0,
                                (gameplayFrames * 1) % 64, 16, 16));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7260);
}

// Scene Draw Config 9
void func_8009EE44(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    if (0) {} // Necessary to match

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7274);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames % 128, 0, 32, 16, 1, gameplayFrames % 128, 0,
                                32, 16));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, gameplayFrames % 128, 32, 32, 1,
                                gameplayFrames % 128, gameplayFrames % 128, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    if ((globalCtx->unk_11D30[0] == 0) && (INV_CONTENT(ITEM_COJIRO) == ITEM_COJIRO)) {
        if (globalCtx->unk_11D30[1] == 50) {
            func_8002F7DC(&PLAYER->actor, NA_SE_EV_CHICKEN_CRY_M);
            globalCtx->unk_11D30[0] = 1;
        }
        globalCtx->unk_11D30[1]++;
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7309);
}

// Scene Draw Config 10
void func_8009F074(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7323);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 127 - gameplayFrames % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7339);
}

u32 D_8012A380[] = { 0x02009678, 0x0200DE78 };

// Scene Draw Config 11
void func_8009F1B4(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7363);

    { s32 pad[2]; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A380[gSaveContext.nightFlag]));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7371);
}

// Scene Draw Config 12
void func_8009F270(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7385);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, gameplayFrames % 128, 32, 32, 1, 0, gameplayFrames % 128,
                                32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, gameplayFrames % 128, 32, 32, 1, 0, gameplayFrames % 128,
                                32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7409);
}

// Scene Draw Config 13
void func_8009F40C(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7423);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 10) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7443);
}

// Scene Draw Config 14
void func_8009F5D4(GlobalContext* globalCtx) {
    Gfx* displayListHead = Graph_Alloc(globalCtx->state.gfxCtx, 3 * sizeof(Gfx));

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7461);

    gSPSegment(POLY_XLU_DISP++, 0x08, displayListHead);

    if ((gSaveContext.dayTime >= 0x4AAC) && (gSaveContext.dayTime <= 0xC000)) {
        gSPEndDisplayList(displayListHead);
    } else {
        if (gSaveContext.dayTime > 0xC000) {
            if (globalCtx->unk_11D30[0] != 255) {
                Math_StepToS(&globalCtx->unk_11D30[0], 255, 5);
            }
        } else if (gSaveContext.dayTime >= 0x4000) {
            if (globalCtx->unk_11D30[0] != 0) {
                Math_StepToS(&globalCtx->unk_11D30[0], 0, 10);
            }
        }

        gDPSetPrimColor(displayListHead++, 0, 0, 255, 255, 255, globalCtx->unk_11D30[0]);
        gSPDisplayList(displayListHead++, &D_0300AA48);
        gSPEndDisplayList(displayListHead);
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7495);
}

// Scene Draw Config 15
void func_8009F7D4(GlobalContext* globalCtx) {
    s8 sp6F = coss((globalCtx->gameplayFrames * 1500) & 0xFFFF) >> 8;
    s8 sp6E = coss((globalCtx->gameplayFrames * 1500) & 0xFFFF) >> 8;
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7512);

    gameplayFrames = globalCtx->gameplayFrames;
    sp6F = (sp6F >> 1) + 192;
    sp6E = (sp6E >> 1) + 192;

    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, gameplayFrames % 128, 32, 32, 1, 0, gameplayFrames % 128,
                                32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp6F, sp6E, 255, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7530);
}

u32 D_8012A388[] = { 0x02009808, 0x02008FC8 };

// Scene Draw Config 16
void func_8009F9D0(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7555);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 127 - gameplayFrames % 128, 32, 32, 1,
                                gameplayFrames % 128, 0, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A388[gSaveContext.nightFlag]));

    { s32 pad[2]; }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7578);
}

u32 D_8012A390[] = { 0x020081E0, 0x0200FBE0 };

// Scene Draw Config 17
void func_8009FB74(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7602);

    { s32 pad[2]; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A390[gSaveContext.nightFlag]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7615);
}

// Scene Draw Config 18
void func_8009FC90(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7630);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 127 - gameplayFrames % 128, 32, 32, 1,
                                127 - gameplayFrames % 128, 0, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 3) % 128,
                                127 - (gameplayFrames * 6) % 128, 32, 32, 1, (gameplayFrames * 6) % 128,
                                127 - (gameplayFrames * 3) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 64);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 64);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7653);
}

f32 D_8012A398 = 0.0f;

void func_8009FE58(GlobalContext* globalCtx) {
    static s16 D_8012A39C = 538;
    static s16 D_8012A3A0 = 4272;
    u32 gameplayFrames;
    f32 temp;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7712);

    gameplayFrames = globalCtx->gameplayFrames;
    if (globalCtx->sceneNum == SCENE_BDAN) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames % 128, (gameplayFrames * 2) % 128, 32,
                                    32, 1, 127 - gameplayFrames % 128, (gameplayFrames * 2) % 128, 32, 32));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 255 - (gameplayFrames * 4) % 256, 32, 64, 1, 0,
                                    255 - (gameplayFrames * 4) % 256, 32, 64));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TexScroll(globalCtx->state.gfxCtx, (127 - (gameplayFrames * 1)) % 128,
                                 (gameplayFrames * 1) % 128, 32, 32));
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    if (FrameAdvance_IsEnabled(globalCtx) != true) {
        D_8012A39C += 1820;
        D_8012A3A0 += 1820;

        temp = 0.020000001f;
        func_800AA76C(&globalCtx->view, ((360.00018f / 65535.0f) * (M_PI / 180.0f)) * temp * Math_CosS(D_8012A39C),
                      ((360.00018f / 65535.0f) * (M_PI / 180.0f)) * temp * Math_SinS(D_8012A39C),
                      ((360.00018f / 65535.0f) * (M_PI / 180.0f)) * temp * Math_SinS(D_8012A3A0));
        func_800AA78C(&globalCtx->view, 1.f + (0.79999995f * temp * Math_SinS(D_8012A3A0)),
                      1.f + (0.39999998f * temp * Math_CosS(D_8012A3A0)), 1.f + (1 * temp * Math_CosS(D_8012A39C)));
        func_800AA7AC(&globalCtx->view, 0.95f);

        switch (globalCtx->unk_11D30[0]) {
            case 0:
                break;
            case 1:
                if (globalCtx->unk_11D30[1] < 1200) {
                    globalCtx->unk_11D30[1] += 200;
                } else {
                    globalCtx->unk_11D30[0]++;
                }
                break;
            case 2:
                if (globalCtx->unk_11D30[1] > 0) {
                    globalCtx->unk_11D30[1] -= 30;
                } else {
                    globalCtx->unk_11D30[1] = 0;
                    globalCtx->unk_11D30[0] = 0;
                }
                break;
        }

        D_8012A398 += 0.15f + (globalCtx->unk_11D30[1] * 0.001f);
    }

    if (globalCtx->roomCtx.curRoom.num == 2) {
        Matrix_Scale(1.0f, sinf(D_8012A398) * 0.8f, 1.0f, MTXMODE_NEW);
    } else {
        Matrix_Scale(1.005f, sinf(D_8012A398) * 0.8f, 1.005f, MTXMODE_NEW);
    }

    gSPSegment(POLY_OPA_DISP++, 0x0D, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_scene_table.c", 7809));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7811);
}

// Scene Draw Config 26
void func_800A0334(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7825);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 512, 32,
                                128, 1, gameplayFrames % 128, (gameplayFrames * 1) % 512, 32, 128));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7852);
}

// Scene Draw Config 52
void func_800A055C(GlobalContext* globalCtx) {
    func_8009BEEC(globalCtx);
}

// Scene Draw Config 51
void func_800A057C(GlobalContext* globalCtx) {
    func_8009BEEC(globalCtx);
}

// Scene Draw Config 49
void func_800A059C(GlobalContext* globalCtx) {
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7893);

    gameplayFrames = globalCtx->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 127 - (gameplayFrames * 2) % 128, 0, 32, 64));
    gSPSegment(POLY_OPA_DISP++, 0x09, Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (gameplayFrames * 2) % 512, 128, 128));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 7910);
}

void (*sSceneDrawHandlers[])(GlobalContext*) = {
    func_80099550, func_8009DA30, func_8009DD5C, func_8009DE78, func_8009E0B8, func_8009E54C, func_8009E730,
    func_8009E8C0, func_8009EAD8, func_8009EE44, func_8009F074, func_8009F1B4, func_8009F270, func_8009F40C,
    func_8009F5D4, func_8009F7D4, func_8009F9D0, func_8009FB74, func_8009FC90, func_800995DC, func_80099878,
    func_8009FE58, func_8009D758, func_8009B0FC, func_8009AE30, func_8009D974, func_800A0334, func_8009CC00,
    func_80099760, func_8009B86C, func_80099BD8, func_8009A45C, func_8009A798, func_8009A9DC, func_8009B9BC,
    func_8009BAA4, func_8009BC44, func_8009C3EC, func_8009C0AC, func_8009ACA8, func_8009AFE0, func_8009D0E8,
    func_8009C608, func_8009C8B8, func_8009D31C, func_8009D438, func_8009D5B4, func_8009CAC0, func_8009AB98,
    func_800A059C, func_8009CF84, func_800A057C, func_800A055C,
};

void Scene_Draw(GlobalContext* globalCtx) {
    if (HREG(80) == 17) {
        if (HREG(95) != 17) {
            HREG(95) = 17;
            HREG(81) = 1;
            HREG(82) = 1;
            HREG(83) = 0;
            HREG(84) = 0;
            HREG(85) = 0;
            HREG(86) = 0;
            HREG(87) = 0;
            HREG(88) = 0;
            HREG(89) = 0;
            HREG(91) = 0;
            HREG(92) = 0;
            HREG(93) = 0;
            HREG(94) = 0;
        }

        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 8104);

        if (HREG(81) == 1) {
            gSPDisplayList(POLY_OPA_DISP++, sDefaultDisplayList);
            gSPDisplayList(POLY_XLU_DISP++, sDefaultDisplayList);
        }

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_scene_table.c", 8109);

        if (HREG(82) == 1) {
            sSceneDrawHandlers[globalCtx->sceneConfig](globalCtx);
        }
    } else {
        sSceneDrawHandlers[globalCtx->sceneConfig](globalCtx);
    }
}
