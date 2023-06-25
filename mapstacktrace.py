# SPDX-License-Identifier: CC0-1.0

import sys
import re
import subprocess

"""
$ mips-linux-gnu-nm zelda_ocarina_mq_dbg.elf -fbsd --no-sort | head
00001010 t .clear_bss
ffffffff800024f0 t mainloop
ffffffff80002524 t read_comp
ffffffff80002558 t next_iter
ffffffff80012010 t __osIntOffTable
ffffffff80012030 t __osIntTable
ffffffff80003dd0 t redispatch
ffffffff80003d98 t sw1
ffffffff80003d78 t sw2
ffffffff80003bb4 t rcp
"""
nm_out = subprocess.check_output(
    ["mips-linux-gnu-nm", "zelda_ocarina_mq_dbg.elf", "-fbsd", "--no-sort"]
)
sym_names_by_value: dict[int, list[str]] = dict()
for line in nm_out.splitlines():
    parts = line.split()
    assert len(parts) == 3
    sym_value_str, _, sym_name = parts
    assert len(sym_value_str) >= 8
    sym_value_str = sym_value_str[-8:]
    sym_value = int(sym_value_str, 16)
    sym_names_by_value.setdefault(sym_value, []).append(sym_name)

# SP PC (VPC)
stacktraceline_re = re.compile(
    r"(80[0-9a-fA-F]{6})\s+(80[0-9a-fA-F]{6})(?:\s+->\s+(80[0-9a-fA-F]{6}))?"
)

inp = sys.stdin.read()

print()
print(" -" * 7)
print()

inp_lines = inp.splitlines()
line_col_length = max([len(line) for line in inp_lines])

for line in inp_lines:
    m = stacktraceline_re.search(line)
    if m is None:
        print(line)
        continue

    pc_str = m.group(2)
    vpc_str = m.group(3)
    if vpc_str is None:
        search_addr_str = pc_str
    else:
        search_addr_str = vpc_str
    search_addr_ini = int(search_addr_str, 16)

    search_addr = search_addr_ini
    sym_names = None
    while True:
        sym_names = sym_names_by_value.get(search_addr)
        if sym_names:
            break
        search_addr -= 1

    if sym_names is not None:
        sym_names = [s.decode("ascii") for s in sym_names]

        print(
            f"{line:{line_col_length}}",
            "@",
            (
                sym_names[0]
                if len(sym_names) == 1
                else ("[ " + ", ".join(sym_names) + " ]")
            ),
            "+",
            hex(search_addr_ini - search_addr),
        )
    else:
        print(line)
