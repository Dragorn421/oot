# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

import argparse
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("dllsmk", type=Path)
args = parser.parse_args()

dllsmk_p: Path = args.dllsmk

dllsmk_frags = []

for dllrootdir_p in (
    Path("src/overlays/actors"),
    Path("src/overlays/effects"),
    Path("src/overlays/gamestates"),
    Path("src/overlays/misc"),
):
    for dlldir_p in sorted(dllrootdir_p.iterdir()):
        c_files = sorted(
            _p
            for _p in dlldir_p.iterdir()
            if _p.suffix == ".c" and _p.suffixes[-2:] != [".inc", ".c"]
        )
        o_files_reqs = " ".join(
            f"$(BUILD_DIR)/{_p.with_suffix(".o")}" for _p in c_files
        )
        dllsmk_frags.append(
            f"""
$(BUILD_DIR)/{dlldir_p}/dll.partial.o: {o_files_reqs}
dlls_OBJS += $(BUILD_DIR)/{dlldir_p}/dll.o
"""
        )

dllsmk_p.write_text("".join(dllsmk_frags))
