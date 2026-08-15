# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

import argparse
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("dsosmk", type=Path)
args = parser.parse_args()

dllsmk_p: Path = args.dsosmk

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
        if dlldir_p.name == "ovl_kaleido_scope":
            c_files.remove(
                Path("src/overlays/misc/ovl_kaleido_scope/z_lmap_mark_data.c")
            )
        if dlldir_p.name == "ovl_map_mark_data":
            c_files.remove(
                Path("src/overlays/misc/ovl_map_mark_data/z_map_mark_data.c")
            )
        o_files_reqs = " ".join(
            f"$(BUILD_DIR)/{_p.with_suffix(".o")}" for _p in c_files
        )
        dso_pstem = dlldir_p.relative_to("src/overlays")
        dllsmk_frags.append(f"""
$(BUILD_DIR)/dfs/{dso_pstem}.dso: {o_files_reqs}
DSOS += $(BUILD_DIR)/dfs/{dso_pstem}.dso
""")

dllsmk_p.write_text("".join(dllsmk_frags))
