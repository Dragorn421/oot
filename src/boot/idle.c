#include "array_count.h"
#include "buffers.h"
#include "build.h"
#include "idle.h"
#include "main.h"
#include "n64sys.h"
#include "printf.h"
#include "terminal.h"
#include "translation.h"
#include "versions.h"
#include "thread.h"

#pragma increment_block_number "gc-eu:192 gc-eu-mq:192 gc-jp:192 gc-jp-ce:192 gc-jp-mq:192 gc-us:192 gc-us-mq:192" \
                               "ntsc-1.0:192 ntsc-1.1:192 ntsc-1.2:192 pal-1.0:192 pal-1.1:192"

void Main_ThreadEntry(void* arg) {
    PRINTF(T("mainx 実行開始\n", "mainx execution started\n"));
    Main(arg);
    PRINTF(T("mainx 実行終了\n", "mainx execution finished\n"));
}

void Idle_ThreadEntry(void* arg) {
    PRINTF(T("アイドルスレッド(idleproc)実行開始\n", "Idle thread (idleproc) execution started\n"));
    PRINTF(T("作製者    : %s\n", "Created by: %s\n"), gBuildCreator);
    PRINTF(T("作成日時  : %s\n", "Created   : %s\n"), gBuildDate);
    PRINTF("MAKEOPTION: %s\n", gBuildMakeOption);
    PRINTF_COLOR_GREEN();
    PRINTF(T("ＲＡＭサイズは %d キロバイトです(osMemSize/osGetMemSize)\n",
             "RAM size is %d kilobytes (osMemSize/osGetMemSize)\n"),
           (s32)get_memory_size() / 1024);
    PRINTF(T("Ｚバッファのサイズは %d キロバイトです\n", "Z buffer size is %d kilobytes\n"), 0x96);
    PRINTF(T("ダイナミックバッファのサイズは %d キロバイトです\n", "The dynamic buffer size is %d kilobytes\n"), 0x92);
    PRINTF(T("ＦＩＦＯバッファのサイズは %d キロバイトです\n", "FIFO buffer size is %d kilobytes\n"), 0x60);
    PRINTF(T("ＹＩＥＬＤバッファのサイズは %d キロバイトです\n", "YIELD buffer size is %d kilobytes\n"), 3);
    PRINTF(T("オーディオヒープのサイズは %d キロバイトです\n", "Audio heap size is %d kilobytes\n"),
           ((intptr_t)&gAudioHeap[ARRAY_COUNT(gAudioHeap)] - (intptr_t)gAudioHeap) / 1024);
    PRINTF_RST();

    Main_ThreadEntry(arg);
}
