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
    Path("src/overlays/misc"),
):
    for dlldir_p in sorted(dllrootdir_p.iterdir()):
        if dlldir_p.name in {"ovl_player_actor", "ovl_kaleido_scope"}:
            continue
        c_files = sorted(
            _p
            for _p in dlldir_p.iterdir()
            if _p.suffix == ".c" and _p.suffixes[-2:] != [".inc", ".c"]
        )
        if dlldir_p.name == "ovl_map_mark_data":
            c_files.remove(
                Path("src/overlays/misc/ovl_map_mark_data/z_map_mark_data.c")
            )
        o_files_reqs = " ".join(
            f"$(BUILD_DIR)/{_p.with_suffix(".o")}" for _p in c_files
        )
        dllsmk_frags.append(f"""
$(BUILD_DIR)/{dlldir_p}/dll.plf: {o_files_reqs}
dlls_OBJS += $(BUILD_DIR)/{dlldir_p}/dll.o
""")

dllsmk_p.write_text("".join(dllsmk_frags))
