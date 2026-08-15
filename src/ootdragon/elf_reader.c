#include <assert.h>
#include <elf.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libdragon.h>

#include "elf_reader.h"
#include "dma_queue.h"

enum elf_section_type {
    ELF_ST_PROGBITS,
    ELF_ST_NOBITS,
    ELF_ST_OTHER
};

struct elf_section {
    char* name;
    uint32_t addr, align, rom_offset, size;
    enum elf_section_type type;
};

struct elf_section* elf_sections;
size_t num_elf_sections;

static uint32_t find_elf_header(void) {
#define ELF_MAGIC 0x7F454C46
    uint32_t elf_header = 0x10001000;
    bool found_elf = false;
    for (int i = 0; i < 64 * 1024 * 1024 / 256; i++) {
        if (io_read(elf_header) == ELF_MAGIC) {
            found_elf = true;
            break;
        }
        elf_header += 0x100;
    }
    assert(found_elf);

    return elf_header;
}

static struct elf_section* read_elf_sections_metadata(uint32_t elf_header, void** shstrtab_p, size_t* num_sections_p) {
    // EI_CLASS
    uint8_t elf_class = io_read(elf_header + 0x4) >> 24;

    uint32_t e_shoff;
    uint16_t e_shentsize, e_shnum, e_shstrndx;

    switch (elf_class) {
        case ELFCLASS32: {
            Elf32_Ehdr* ehdr = CachedAddr(malloc_uncached(sizeof(Elf32_Ehdr)));
            dma_read(ehdr, elf_header, sizeof(Elf32_Ehdr));

            e_shoff = ehdr->e_shoff;
            e_shentsize = ehdr->e_shentsize;
            e_shnum = ehdr->e_shnum;
            e_shstrndx = ehdr->e_shstrndx;

            free(ehdr);
        } break;

        default: {
            assertf(false, "only elf32 supported");
            exit(EXIT_FAILURE);
        } break;
    }

    assertf(e_shstrndx != SHN_UNDEF,
            "elf has no section name string table (hint: comment out invoking N64_ELFCOMPRESS in n64.mk)");
    assertf(e_shstrndx != SHN_XINDEX, "not implemented logic for real e_shstrndx >= SHN_LORESERVE (see man elf)");
    assert(e_shstrndx < e_shnum);

    /*
     * Copy section name string table to ram
     */

    void* shdr = CachedAddr(malloc_uncached(e_shentsize * e_shnum));
    dma_read(shdr, elf_header + e_shoff, e_shentsize * e_shnum);

    uint32_t shstrtab_offset, shstrtab_size;

    switch (elf_class) {
        case ELFCLASS32: {
            Elf32_Shdr* shdr32 = shdr;
            Elf32_Shdr* shdr_shstrtab = &shdr32[e_shstrndx];

            shstrtab_offset = shdr_shstrtab->sh_offset;
            shstrtab_size = shdr_shstrtab->sh_size;
        } break;

        default: {
            assertf(false, "only elf32 supported");
            exit(EXIT_FAILURE);
        } break;
    }

    char* shstrtab = CachedAddr(malloc_uncached(shstrtab_size));
    if (shstrtab_p != NULL) {
        *shstrtab_p = shstrtab;
    }
    dma_read(shstrtab, elf_header + shstrtab_offset, shstrtab_size);

    struct elf_section* elf_sections = malloc(sizeof(struct elf_section[e_shnum]));
    struct elf_section* es = elf_sections;

    /*
     * Read section headers
     */

    switch (elf_class) {
        case ELFCLASS32: {
            Elf32_Shdr* shdr32 = shdr;
            for (uint16_t i = 0; i < e_shnum; i++) {
                enum elf_section_type type;
                switch (shdr32[i].sh_type) {
                    case SHT_NULL:
                        continue;
                    case SHT_NOBITS:
                        type = ELF_ST_NOBITS;
                        break;
                    case SHT_PROGBITS:
                        type = ELF_ST_PROGBITS;
                        break;
                    default:
                        type = ELF_ST_OTHER;
                        break;
                }
                uint32_t sh_name = shdr32[i].sh_name;
                es->name = &shstrtab[sh_name];
                es->addr = shdr32[i].sh_addr;
                es->align = shdr32[i].sh_addralign;
                if (type == ELF_ST_NOBITS)
                    es->rom_offset = 0;
                else
                    es->rom_offset = elf_header + shdr32[i].sh_offset;
                es->size = shdr32[i].sh_size;
                es->type = type;
                es++;
            }
        } break;

        default: {
            assertf(false, "only elf32 supported");
            exit(EXIT_FAILURE);
        } break;
    }

    free(shdr);

    if (num_sections_p != NULL) {
        *num_sections_p = es - elf_sections;
    }
    return elf_sections;
}

struct elf_section* elf_find_section(const char* name) {
    struct elf_section* es = NULL;
    for (size_t i = 0; i < num_elf_sections; i++) {
        if (strcmp(name, elf_sections[i].name) == 0) {
            es = &elf_sections[i];
            break;
        }
    }
    return es;
}

void read_elf_metadata(void) {
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;
    elf_sections = read_elf_sections_metadata(find_elf_header(), NULL, &num_elf_sections);
}

/*
 * elf_section_
 */

size_t elf_section_get_size(const char* section_name) {
    struct elf_section* sec = elf_find_section(section_name);
    assertf(sec != NULL, "%s not found", section_name);
    return sec->size;
}

__attribute__((format(printf, 1, 2))) size_t elf_section_get_size_fmtname(const char* section_name_format, ...) {
    va_list args;
    va_start(args, section_name_format);
    char buf[100];
    int nchar;
    nchar = vsnprintf(buf, sizeof(buf), section_name_format, args);
    assert(nchar < sizeof(buf));
    va_end(args);
    return elf_section_get_size(buf);
}

void elf_section_dma_queue_read(void* ram, const char* section_name, struct dma_request* req) {
    struct elf_section* sec = elf_find_section(section_name);
    assertf(sec != NULL, "%s not found", section_name);
    dma_queue_read(ram, sec->rom_offset, sec->size, req);
}

__attribute__((format(printf, 2, 4))) void
elf_section_dma_queue_read_fmtname(void* ram, const char* section_name_format, struct dma_request* req, ...) {
    va_list args;
    va_start(args, req);
    char buf[100];
    int nchar;
    nchar = vsnprintf(buf, sizeof(buf), section_name_format, args);
    assert(nchar < sizeof(buf));
    va_end(args);
    elf_section_dma_queue_read(ram, buf, req);
}

void elf_section_dma_queue_read_fragment(void* ram, const char* section_name, uint32_t offset, size_t size,
                                         struct dma_request* req) {
    struct elf_section* sec = elf_find_section(section_name);
    assertf(sec != NULL, "%s not found", section_name);
    assert(offset + size <= sec->size);
    dma_queue_read(ram, sec->rom_offset + offset, size, req);
}
