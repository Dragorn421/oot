#!/bin/env python3

# SPDX-License-Identifier: CC0-1.0 OR Unlicense

import re

from typing import (
    List,
    Dict,
    Tuple,
    Iterable,
    Generator,
    Optional,
)

# (objfile, section, ram, rom, symbol)
ParsedMapEntry = Tuple[str, str, Optional[int], int, str]

parsed_map: Optional[List[ParsedMapEntry]] = None
parsed_map_by_symbol: Optional[Dict[str, ParsedMapEntry]] = None
parsed_map_by_address: Optional[Dict[int, ParsedMapEntry]] = None


def parse_map_file(
    mapfile_lines: Iterable[str],
) -> Generator[ParsedMapEntry, None, None]:
    """Parse a map file from its lines, and yield informations on all symbols

    Yields `(objfile, section, ram, rom, symbol)`, for example:

    ```
       ("build/src/makerom/entry.o", ".text", None, 0x1000, "entrypoint")
       ("build/src/code/sys_matrix.o", ".text", 0x800d1084, 0xb48224, "Matrix_RotateZYX")
       ("build/src/code/z_rcp.o", ".data", 0x80126280, 0xb9d420, "sSetupDL")
       ("build/src/buffers/heaps.o", ".bss", 0x801d89e0, 0xc4fb80, "gAudioHeap")
       ("build/src/overlays/actors/ovl_En_Daiku/z_en_daiku.o", ".text", 0x809e3a8c, 0xd9a67c, "EnDaiku_EscapeRun")
    ```

    Note: rom offsets, even if they're never `None`, may not mean anything.
    For example, for symbols in .bss sections.
    """

    symbol_pattern = re.compile(r"[a-zA-Z_][0-9a-zA-Z_]*")

    cur_section = None
    cur_objfile = None
    ram_to_rom = None

    for line in mapfile_lines:
        tokens = line.split()

        if not tokens:
            continue

        # hardcode some things that mess up the map parsing.
        # this map parsing doesn't need to be robust anyway
        # TODO it didn't but if I want to actually use this it should be
        if (
            (
                # "Memory Configuration"
                tokens
                == ["Memory", "Configuration"]
            )
            or (
                # "LOAD build/src/makerom/rom_header.o" ...
                tokens[0]
                == "LOAD"
            )
            or (
                # "OUTPUT(zelda_ocarina_mq_dbg.elf elf32-tradbigmips)"
                tokens[0].startswith("OUTPUT")
            )
            or (
                # " *(.debug_info .gnu.linkonce.wi.*)" ...
                tokens[0].startswith("*(")
            )
        ):
            continue

        if tokens[0] in {".text", ".data", ".rodata", ".bss"}:
            cur_objfile = tokens[3]
            cur_section = tokens[0]
        elif line.startswith(" ."):
            cur_section = None

        if "load address" in line:
            if len(tokens) == 6:
                # "..ovl_En_Daiku  0x00000000809e2b30     0x17f0 load address 0x0000000000d99720"
                ram_str = tokens[1]
                rom_str = tokens[5]
            elif len(tokens) == 5:
                # long segment names are put in the previous line, shifting tokens by 1
                # previous line: "..ovl_En_Daiku_Kakariko"
                # "                0x00000000809e4320     0x14b0 load address 0x0000000000d9af10"
                ram_str = tokens[0]
                rom_str = tokens[4]
            else:
                raise Exception(
                    f"Unexpected amount of tokens {len(tokens)} {tokens} in line {line}"
                )
            ram = int(ram_str, 0)
            rom = int(rom_str, 0)
            ram_to_rom = rom - ram

            # "load address" comes before sections
            cur_objfile = None
            cur_section = None

        if len(tokens) == 2:
            try:
                offset = int(tokens[0], 0)
            except ValueError:
                raise Exception(f"Could not parse {tokens}")
            if ram_to_rom is None:
                ram = None
                rom = offset
            else:
                ram = offset
                rom = ram + ram_to_rom
            symbol_name = tokens[1]
            if symbol_pattern.fullmatch(symbol_name):
                yield (
                    cur_objfile,
                    cur_section,
                    ram,
                    rom,
                    symbol_name,
                )
            else:
                # the line may be like "                0x0000000006008870        0x0"
                if not symbol_name.startswith("0x"):
                    raise Exception(f"Could not parse {tokens}")


def load_map_file():
    global parsed_map
    global parsed_map_by_symbol
    global parsed_map_by_address

    with open("build/z64.map") as f:
        mapfile_lines = f.readlines()

    parsed_map = list(parse_map_file(mapfile_lines))
    parsed_map_by_symbol = {
        symbol: (objfile, section, ram, rom, symbol)
        for (objfile, section, ram, rom, symbol) in parsed_map
    }
    parsed_map_by_address = {
        ram: (objfile, section, ram, rom, symbol)
        for (objfile, section, ram, rom, symbol) in parsed_map
    }
    # remove address lookups for addresses shared by several symbols
    for (objfile, section, ram, rom, symbol) in parsed_map:
        if ram in parsed_map_by_address:
            if parsed_map_by_address[ram][4] != symbol:
                del parsed_map_by_address[ram]


def find_symbol(name: str):
    assert parsed_map_by_symbol is not None, "call load_map_file first"
    return parsed_map_by_symbol[name]


def find_address(address: int):
    assert parsed_map_by_address is not None, "call load_map_file first"
    return parsed_map_by_address[address]


if __name__ == "__main__":
    with open("build/z64.map") as f:
        mapfile_lines = f.readlines()
    for objfile, section, ram, rom, symbol in parse_map_file(mapfile_lines):
        print(objfile, section, "None" if ram is None else hex(ram), hex(rom), symbol)
