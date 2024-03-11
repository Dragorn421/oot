#!/bin/env python3

# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

# v3 !

DEBUGIDK = False

from pathlib import Path
import sys
import io
import os

from elftools.construct import Container
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section, SymbolTableSection, StringTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.constants import SHN_INDICES

import colorama


def align(v: int, align: int):
    """Returns the next multiple of `align` that is >= `v`"""
    return (v + align - 1) // align * align


def stream_copy(f_in: io.IOBase, f_out: io.IOBase, size: int):
    while size != 0:
        buf = f_in.read(size)
        if not buf:
            raise Exception(f"Unexpected EOF, expected {size} more")
        f_out.write(buf)
        size -= len(buf)


def detach_and_write(
    elf: ELFFile,
    detach_section_name: str,
    section_symbols_prefix: str,
    frankenelf_p: Path,
    syms_p: Path,
):
    section_symbols_prefix_bytes = section_symbols_prefix.encode("ascii")
    sections: list[Section] = []
    sections.extend(elf.iter_sections())

    section_index_by_section_name = {
        section.name: i for i, section in enumerate(sections)
    }

    detach_section_index = section_index_by_section_name.get(detach_section_name)
    if detach_section_index is None:
        return False

    # Compiler-generated elf files typically have basic sections text, data, bss
    # even if there is nothing inside.
    # Detaching an empty section does no harm but is useless,
    # and confusing when comparing with the disassembly
    # (the disassembly does not "disassemble empty sections" whatever that could mean).
    # Empty sections resulting from compiling C basically cannot have symbols,
    # so nothing is lost by acting as if empty sections didn't exist at all.
    if sections[detach_section_index].header["sh_size"] == 0:
        return False

    reloc_detach_section_index = section_index_by_section_name.get(
        f".rel{detach_section_name}"
    )
    if reloc_detach_section_index is not None:
        reloc_detach_section = sections[reloc_detach_section_index]
        assert isinstance(reloc_detach_section, RelocationSection)
    else:
        reloc_detach_section = None

    remove_sections_indices: list[int] = []
    for remove_section_name in (".text", ".data", ".rodata", ".bss"):
        if remove_section_name != detach_section_name:
            i = section_index_by_section_name.get(remove_section_name)
            i_rel = section_index_by_section_name.get(f".rel{remove_section_name}")
            if i is not None:
                remove_sections_indices.append(i)
            if i_rel is not None:
                remove_sections_indices.append(i_rel)

    # It is not entirely impossible that an elf has no symbol table or string table,
    # but it's impossible enough in the context of reading an elf resulting from
    # assembly/compilation to just assume they always exist.

    symtab_section_index = section_index_by_section_name[".symtab"]
    symtab_section = sections[symtab_section_index]
    assert isinstance(symtab_section, SymbolTableSection), symtab_section

    strtab_section_index = section_index_by_section_name[".strtab"]
    strtab_section = sections[strtab_section_index]
    assert isinstance(strtab_section, StringTableSection), strtab_section

    new_symtab_data = io.BytesIO(symtab_section.data())
    new_strtab_data = io.BytesIO(strtab_section.data())
    new_strtab_data.seek(0, os.SEEK_END)
    new_reloc_data = None

    syms_ldscript = io.StringIO()

    new_syms = []

    for i, sym in enumerate(symtab_section.iter_symbols()):
        sym_section_index = sym.entry.st_shndx
        is_section_symbol = sym.entry.st_info.type == "STT_SECTION"

        if DEBUGIDK:
            if sym.name == "extern_thing":
                if 0:
                    print(sym.name, sym.entry)

        if sym_section_index == detach_section_index:

            if not is_section_symbol:
                assert '"' not in section_symbols_prefix
                assert '"' not in detach_section_name
                if 1:
                    syms_ldscript.write(
                        f"PROVIDE("
                        f"{sym.name}"
                        f' = "{section_symbols_prefix}{detach_section_name}"'
                        f" + {sym.entry.st_value:#X}"
                        f");\n"
                    )
                else:
                    syms_ldscript.write(f"PROVIDE(" f"{sym.name}" f" = 0x69690" f");\n")

        # FIXME this doesn't do a deepcopy?
        # e.g. sym_entry.st_info.type
        sym_entry = sym.entry.copy()

        if sym_section_index in remove_sections_indices:
            sym_entry.st_shndx = "SHN_UNDEF"
            if sym_entry.st_info.bind == "STB_LOCAL":
                sym_entry.st_info.bind = "STB_GLOBAL"
            if is_section_symbol:
                # It doesn't seem like changing the type would be required,
                # but ld doesn't accept a section symbol defined in a linker script (in a section).
                # TODO check if STT_NOTYPE works, trying out STT_OBJECT for now (investigating issues)
                sym_entry.st_info.type = "STT_OBJECT"

                sym_section = sections[sym_section_index]
                sym_entry.st_name = new_strtab_data.tell()
                if 0 and DEBUGIDK:
                    new_strtab_data.write(
                        b"sectionsymbol_"
                        + detach_section_name.removeprefix(".").encode("ascii")
                        + b"_"
                        + sym_section.name.removeprefix(".").encode("ascii")
                        + b"\0"
                    )
                    if 0:
                        print("section sym", sym.name, "turned to", sym_entry)
                else:
                    new_strtab_data.write(section_symbols_prefix_bytes)
                    new_strtab_data.write(sym_section.name.encode("ascii"))
                    new_strtab_data.write(b"\0")

        new_syms.append(sym_entry)

    # cf https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.symtab.html
    # > In each symbol table, all symbols with STB_LOCAL binding precede the weak and global symbols.
    # TODO sort new_syms, local syms first
    # TODO change relocs accordingly (they contain symbol indices)
    new_syms_sorted = sorted(
        enumerate(new_syms),
        key=lambda item: -1 if item[1].st_info.bind == "STB_LOCAL" else 0,
    )
    new_syms = [sym_entry for i, sym_entry in new_syms_sorted]
    sym_index_shuffle_map = {
        i_in: i_out
        for i_out, (i_in, sym_entry) in enumerate(new_syms_sorted)
        if i_in != i_out
    }
    if DEBUGIDK:
        print(f"{sym_index_shuffle_map=}")
    if reloc_detach_section:
        new_reloc_data = io.BytesIO()
        for reloc in reloc_detach_section.iter_relocations():
            reloc_entry = reloc.entry.copy()
            if DEBUGIDK:
                print(reloc_entry)
            new_rel_sym_i = sym_index_shuffle_map.get(reloc_entry.r_info_sym)
            if new_rel_sym_i is not None:
                reloc_entry.r_info_sym = new_rel_sym_i
                # elftools doesn't update the actual field r_info from r_info_sym...
                assert elf.header.e_ident.EI_CLASS == "ELFCLASS32", elf.header
                reloc_entry.r_info = (new_rel_sym_i & 0xFFFFFF) << 8 | (
                    reloc_entry.r_info_type & 0xFF
                )
                if DEBUGIDK:
                    print("->", reloc_entry)
            elf.structs.Elf_Rel.build_stream(reloc_entry, new_reloc_data)

    if DEBUGIDK:
        _strtab = new_strtab_data.getvalue()
        isym_sectionsymbol_text_rodata = None
        isym_extern_thing = None
        for i, sym_entry in enumerate(new_syms):
            sym_name = _strtab[
                sym_entry.st_name : _strtab.index(b"\0", sym_entry.st_name)
            ]
            if sym_name == b"sectionsymbol_text_rodata" or (
                (
                    detach_section_name == ".text"
                    and sym_name == (section_symbols_prefix_bytes + b".rodata")
                )
            ):
                isym_sectionsymbol_text_rodata = i
                print(colorama.Fore.RED, end="")
            if sym_name == b"extern_thing":
                isym_extern_thing = i
                print(colorama.Fore.RED, end="")
            print(i, sym_name, sym_entry)
            print(colorama.Fore.RESET, end="")
        if 0:
            if (
                isym_sectionsymbol_text_rodata is not None
                and isym_extern_thing is not None
            ):
                print(f"{isym_sectionsymbol_text_rodata=}  <->  {isym_extern_thing=}")
                (
                    new_syms[isym_sectionsymbol_text_rodata],
                    new_syms[isym_extern_thing],
                ) = (
                    new_syms[isym_extern_thing],
                    new_syms[isym_sectionsymbol_text_rodata],
                )

    symtab_local_symbols_end = 0
    assert symtab_section.header.sh_entsize >= elf.structs.Elf_Sym.sizeof()
    for i, sym_entry in enumerate(new_syms):
        if sym_entry.st_info.bind == "STB_LOCAL":
            symtab_local_symbols_end = 1 + i
        new_symtab_data.seek(i * symtab_section.header.sh_entsize)
        elf.structs.Elf_Sym.build_stream(sym_entry, new_symtab_data)

    section_data_override = {
        symtab_section_index: new_symtab_data.getvalue(),
        strtab_section_index: new_strtab_data.getvalue(),
    }
    if new_reloc_data is not None:
        assert reloc_detach_section_index is not None
        section_data_override[reloc_detach_section_index] = new_reloc_data.getvalue()

    # Here and above we just assume the symbol names are ascii, rather than
    # assuming an encoding supporting more characters (e.g. UTF-8) would be fine.
    syms_p.write_text(syms_ldscript.getvalue(), encoding="ascii")

    frankenheader = elf.header.copy()

    # We expect the input elf to not have program headers,
    # therefore there is no code to copy them to the output elf.
    assert frankenheader.e_phoff == 0 and frankenheader.e_phnum == 0, frankenheader

    sizeof_Elf_Shdr = elf.structs.Elf_Shdr.sizeof()

    # Our code could technically allow for elf header and section headers
    # to have different (larger) sizes,
    # but it's much simpler to just work with the same struct sizes,
    # which will be the case anyway.
    assert frankenheader.e_ehsize == elf.structs.Elf_Ehdr.sizeof(), frankenheader
    assert frankenheader.e_shentsize == sizeof_Elf_Shdr, frankenheader

    shstrtab_section_index = frankenheader.e_shstrndx
    # An elf is allowed to have no section name string table,
    # but in practice it always will.
    assert shstrtab_section_index != SHN_INDICES.SHN_UNDEF
    shstrtab_section = sections[shstrtab_section_index]
    # For naming a section with an empty string
    shstrtab_nul_offset = shstrtab_section.data().index(b"\0")

    # Note: assuming 32 bits elf
    ALIGNOF_SECTION_HEADER_TABLE = 4
    ALIGNOF_SECTION_DATA = 0x10  # idk, or maybe use sh_addralign though that's for vma

    with frankenelf_p.open("wb") as f:
        offset = 0
        # Reserve space for the elf header
        offset += elf.structs.Elf_Ehdr.sizeof()

        offset = align(offset, ALIGNOF_SECTION_HEADER_TABLE)
        section_header_table_offset = offset
        frankenheader.e_shoff = section_header_table_offset
        # Reserve space for the section header table
        offset += len(sections) * sizeof_Elf_Shdr
        for section_index, section in enumerate(sections):
            section_header = section.header.copy()
            if section_header.sh_type == "SHT_NULL":
                # Section header is inactive. Other values are garbage.
                pass
            elif section_index in remove_sections_indices:
                # Make the section header inactive,
                # much easier than deleting it and shifting the table.
                section_header.sh_type = "SHT_NULL"
                # Not required but makes things less confusing:
                # TODO investigate if tools behave better with/without this
                section_header.sh_name = shstrtab_nul_offset
                # work around a bfd bug showing benine errors (binutils-2.42, elfcode.h:333, size check should be ignored for SHT_NULL)
                section_header.sh_offset = 0
                section_header.sh_size = 0
            elif section_index in section_data_override:
                data = section_data_override[section_index]
                section_size = len(data)

                offset = align(offset, ALIGNOF_SECTION_DATA)
                section_offset_out = offset
                offset += section_size

                section_header.sh_offset = section_offset_out
                section_header.sh_size = section_size

                f.seek(section_offset_out)
                f.write(data)
            elif section_header.sh_type == "SHT_NOBITS":
                # Section contains no bytes (e.g. .bss)
                # The elf spec says in this case sh_offset
                # > locates the conceptual placement in the file
                # , and inspecting compiled objects suggests using
                # the offset the section data would have been at.
                offset = align(offset, ALIGNOF_SECTION_DATA)
                section_header.sh_offset = offset
            else:
                section_offset_in = section_header.sh_offset
                section_size = section_header.sh_size

                offset = align(offset, ALIGNOF_SECTION_DATA)
                section_offset_out = offset
                offset += section_size

                section_header.sh_offset = section_offset_out

                # Note: don't use section.data() as it may process the raw data,
                # like decompressing it (though it seems only modern debug sections
                # may be compressed so not really a concern in the context of IDO)
                elf.stream.seek(section_offset_in)
                f.seek(section_offset_out)
                stream_copy(elf.stream, f, section_size)

            if section_index == symtab_section_index:
                # cf https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.sheader.html
                # > One greater than the symbol table index of the last local symbol (binding STB_LOCAL).
                # cf https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.symtab.html
                # > a symbol table section's sh_info section header member holds the symbol table index for the first non-local symbol.
                section_header.sh_info = symtab_local_symbols_end
                if DEBUGIDK:
                    print(
                        f"{detach_section_name}: "
                        f"symtab set sh_info to {symtab_local_symbols_end=}"
                    )

            f.seek(section_header_table_offset + section_index * sizeof_Elf_Shdr)
            elf.structs.Elf_Shdr.build_stream(section_header, f)

        f.seek(0)
        elf.structs.Elf_Ehdr.build_stream(frankenheader, f)


def main():
    object_p = Path(sys.argv[1])
    section_symbol_prefix = sys.argv[2]

    with object_p.open("rb") as f:
        elf = ELFFile(f)
        for section_name in (".text", ".data", ".rodata", ".bss"):
            if DEBUGIDK:
                print(colorama.Fore.GREEN, ">>>", section_name, colorama.Fore.RESET)
            detach_and_write(
                elf,
                section_name,
                section_symbol_prefix,
                object_p.with_suffix(f"{section_name}.o"),
                object_p.with_suffix(f"{section_name}.syms.txt"),
            )
            if DEBUGIDK:
                print(colorama.Fore.GREEN, "<<<", section_name, colorama.Fore.RESET)


if __name__ == "__main__":
    main()
