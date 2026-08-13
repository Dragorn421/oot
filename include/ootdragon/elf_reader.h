#ifndef ELF_READER_H
#define ELF_READER_H

#include <stddef.h>
#include <stdint.h>
#include "dma_queue.h"

size_t elf_section_get_size(const char* section_name);
__attribute__((format(printf, 1, 2))) size_t elf_section_get_size_fmtname(const char* section_name_format, ...);
void elf_section_dma_queue_read(void* ram, const char* section_name, struct dma_request* req);
__attribute__((format(printf, 2, 4))) void
elf_section_dma_queue_read_fmtname(void* ram, const char* section_name_format, struct dma_request* req, ...);
void elf_section_dma_queue_read_fragment(void* ram, const char* section_name, uint32_t offset, size_t size,
                                         struct dma_request* req);

size_t elf_section_get_dll_ramsize(const char* dll_name);
void* elf_section_get_dll_vram_start(const char* dll_name);
void elf_section_load_dll(const char* dll_name, void* ram);

#endif
