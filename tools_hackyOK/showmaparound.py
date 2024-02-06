# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

# print symbols surrounding this rom offset
look_for_rom = 0x2E7C2C0
MAP_P = Path("build/gc-eu-mq/oot-gc-eu-mq.map")


from pathlib import Path
from typing import Optional

import colorama
import mapfile_parser


mapfile = mapfile_parser.MapFile()

mapfile.readMapFile(MAP_P)

map_syms_by_name: dict[str, mapfile_parser.Symbol] = dict()

for file in mapfile:
    for sym in file.symbols:
        if sym.vrom is None:
            # FIXME
            continue
        sym.file = file
        assert sym.name not in map_syms_by_name
        map_syms_by_name[sym.name] = sym

if 0:
    # sort by value
    syms_sorted = sorted(
        map_syms_by_name.values(),
        key=lambda sym: sym.vram,
    )
# sort by rom
syms_sorted = sorted(
    map_syms_by_name.values(),
    key=lambda sym: sym.vrom,
)

closest_sym_before_i = None
closest_sym_before_rom_dist = 2**31
closest_sym_after_i = None
closest_sym_after_rom_dist = 2**31

for i, sym in enumerate(syms_sorted):
    if sym.vrom is None:
        continue
    rom_dist = abs(sym.vrom - look_for_rom)
    if sym.vrom <= look_for_rom:
        if rom_dist < closest_sym_before_rom_dist:
            closest_sym_before_i = i
            closest_sym_before_rom_dist = rom_dist
    if sym.vrom > look_for_rom:
        if rom_dist < closest_sym_after_rom_dist:
            closest_sym_after_i = i
            closest_sym_after_rom_dist = rom_dist

N = 10
syms_close = syms_sorted[closest_sym_before_i - N : closest_sym_after_i + N]

located_lookfor = False
prev_file = None
for sym in syms_close:
    file: mapfile_parser.File = sym.file
    if file != prev_file:
        print(f"{colorama.Fore.CYAN}{file.filepath}{colorama.Fore.RESET}")
        prev_file = file

    common = f"{sym.name:40} {f'0x{sym.size:X}':>8}  0x{sym.vram:08X}-0x{sym.vram+sym.size:08X}"
    if sym.vrom is not None:
        if look_for_rom < sym.vrom:
            if not located_lookfor:
                located_lookfor = True
                print(
                    f"{colorama.Fore.RED}< look_for_rom = rom 0x{look_for_rom:06X} >{colorama.Fore.RESET}"
                )
        args = ()
        if look_for_rom == sym.vrom:
            located_lookfor = True
            args = (
                f" {colorama.Fore.RED}<- look_for_rom = rom 0x{look_for_rom:06X}{colorama.Fore.RESET}",
            )
        print(
            f"{common}  0x{sym.vrom:06X}-0x{sym.vrom+sym.size:06X}",
            *args,
        )
    else:
        print(
            f"{common}  (no rom)",
        )
