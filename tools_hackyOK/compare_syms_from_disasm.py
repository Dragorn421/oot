# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

# diff the built map with the symbol values expected by the disassembly


from pathlib import Path
import pickle

import mapfile_parser


MAP_P = Path("build/gc-eu-mq/oot-gc-eu-mq.map")
SYMS_FROM_DISASM_PICKLE_P = Path("syms_from_disasm.pickle")


with SYMS_FROM_DISASM_PICKLE_P.open("rb") as f:
    disasm_syms = pickle.load(f)

disasm_syms: list[tuple[str, int]]

disasm_syms_by_name = {sym_name: sym_value for sym_name, sym_value in disasm_syms}

assert len(disasm_syms_by_name) == len(disasm_syms)

mapfile = mapfile_parser.MapFile()

mapfile.readMapFile(MAP_P)

map_syms_by_name: dict[str, int] = dict()

for file in mapfile:
    for sym in file.symbols:
        assert sym.name not in map_syms_by_name
        map_syms_by_name[sym.name] = sym.vram

syms_not_in_map: list[str] = []

# ok/expected (disasm), built (map)
sym_vals_by_name: dict[str, tuple[int, int]] = dict()

for disasm_sym_name, disasm_sym_value in disasm_syms_by_name.items():
    map_sym_value = map_syms_by_name.get(disasm_sym_name)
    if map_sym_value is None:
        syms_not_in_map.append(disasm_sym_name)
    else:
        assert disasm_sym_name not in sym_vals_by_name
        sym_vals_by_name[disasm_sym_name] = (disasm_sym_value, map_sym_value)

if 0:
    print(syms_not_in_map)

# sort by expected value
syms_sorted = sorted(sym_vals_by_name.items(), key=lambda item: item[1][0])

prev_map_value = -1
for sym_name, (disasm_value, map_value) in syms_sorted:
    assert prev_map_value < map_value, "symbols not ordered the same somehow"
    prev_map_value = map_value

syms_shift = [
    (sym_name, disasm_value - map_value)
    for sym_name, (disasm_value, map_value) in syms_sorted
]

prev_sym_name = None
prev_sym_shift = syms_shift[0][1]
for sym_name, sym_shift in syms_shift:
    if prev_sym_shift != sym_shift:
        print("Prev sym", prev_sym_name, "was shifted by", hex(prev_sym_shift))
        print(
            "   ",
            prev_sym_name,
            " (disasm) =",
            hex(disasm_syms_by_name[prev_sym_name]),
            " (map) =",
            hex(map_syms_by_name[prev_sym_name]),
        )
        print("Next sym", sym_name, "is shifted by", hex(sym_shift))
        print(
            "   ",
            sym_name,
            " (disasm) =",
            hex(disasm_syms_by_name[sym_name]),
            " (map) =",
            hex(map_syms_by_name[sym_name]),
        )
    prev_sym_name = sym_name
    prev_sym_shift = sym_shift
