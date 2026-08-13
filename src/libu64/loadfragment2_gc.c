#include "elf_reader.h"
#include "libc64/malloc.h"
#include "libu64/overlay.h"

void* Overlay_AllocateAndLoad(const char* dll_name) {
    size_t size = elf_section_get_dll_ramsize(dll_name);
    void* allocatedRamAddr = SYSTEM_ARENA_MALLOC_R(size, "../loadfragment2.c", 31);

    elf_section_load_dll(dll_name, allocatedRamAddr);

    return allocatedRamAddr;
}
