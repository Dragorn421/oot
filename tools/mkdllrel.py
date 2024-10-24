# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

import argparse
import dataclasses
import enum
from pathlib import Path
import struct

from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.enums import ENUM_RELOC_TYPE_MIPS

ENUM_RELOC_TYPE_MIPS_reversed = {v: k for k, v in ENUM_RELOC_TYPE_MIPS.items()}


class DllRelType(enum.Enum):
    _32 = "R_MIPS_32"
    _26 = "R_MIPS_26"
    HI16 = "R_MIPS_HI16"
    LO16 = "R_MIPS_LO16"


rel_type_codes = {
    DllRelType._32: 0,
    DllRelType._26: 1,
    DllRelType.HI16: 2,
    DllRelType.LO16: 3,
}


@dataclasses.dataclass
class DllRel:
    offset: int
    rel_type: DllRelType


STRUCT_U32_BE = struct.Struct(">I")


parser = argparse.ArgumentParser()
parser.add_argument("dll_code_in", type=Path)
parser.add_argument("dll_rel_out", type=Path)
args = parser.parse_args()

dll_code_in_p: Path = args.dll_code_in
dll_rel_out_p: Path = args.dll_rel_out

dll_rels = list[DllRel]()

with dll_code_in_p.open("rb") as dll_code_in_f:
    elf = ELFFile(dll_code_in_f)
    dll_section_index = elf.get_section_index("dll.code")
    bss_section_index = elf.get_section_index("dll.code.bss")
    rel_section = elf.get_section_by_name(".reldll.code")
    assert isinstance(rel_section, RelocationSection)
    symtab = elf.get_section_by_name(".symtab")
    assert isinstance(symtab, SymbolTableSection)
    for rel in rel_section.iter_relocations():
        rel_type = DllRelType(ENUM_RELOC_TYPE_MIPS_reversed[rel["r_info_type"]])
        sym = symtab.get_symbol(rel["r_info_sym"])
        assert sym["st_shndx"] in {"SHN_UNDEF", dll_section_index, bss_section_index}
        sym_is_undef = sym["st_shndx"] == "SHN_UNDEF"
        if sym_is_undef:
            continue

        rel_offset = rel["r_offset"]
        assert rel_offset & 3 == 0

        if 0:
            print(
                hex(rel_offset),
                rel_type,
                sym.name,
                hex(sym["st_value"]),
                {
                    "SHN_UNDEF": "UNDEF",
                    dll_section_index: "in dll.code",
                    bss_section_index: "in dll.code.bss",
                }[sym["st_shndx"]],
            )

        dll_rels.append(DllRel(rel_offset, rel_type))


dll_rel_out_p.write_bytes(
    b"".join(STRUCT_U32_BE.pack(_rel.offset | rel_type_codes[_rel.rel_type]) for _rel in dll_rels)
)
