#!/bin/env python3

# SPDX-License-Identifier: CC0-1.0 OR Unlicense

if False:
    DEBUG = True
    DEBUG_ONCE = True
    DEBUG_NOREPLACE = True
else:
    DEBUG = False
    DEBUG_ONCE = False
    DEBUG_NOREPLACE = False

import sys
from pathlib import Path
import re
import enum

from typing import (
    List,
)

import mapparser
import mycolliderinit


Action = enum.Enum("Action", "REMOVE_STATIC RECONSTRUCT")
action = Action[sys.argv[1]]


mapparser.load_map_file()
mycolliderinit.load_baserom_bytes()
mycolliderinit.load_z64collision_check()

src_path = Path("src/overlays")

collider_init_structs = [
    (
        name,
        re.compile(r"\b" + name + r"\b"),
    )
    for name in [
        "ColliderCylinderInit",
        "ColliderCylinderInitType1",
        # "ColliderCylinderInitToActor", # unused
        "ColliderQuadInit",
        "ColliderJntSphInit",
        "ColliderJntSphElementInit",  # must be after ColliderJntSphInit for the cached_lengths dict
        # "ColliderJntSphInitType1",
        "ColliderTrisInit",
        "ColliderTrisElementInit",  # must be after ColliderTrisInit for the cached_lengths dict
    ]
    + ([] if action == Action.REMOVE_STATIC else [])
]

for c_file_path in src_path.glob("**/*.c"):

    if any(
        c_file_path.samefile(exclude_path)
        for exclude_path in (
            # array of ColliderCylinderInit
            "src/overlays/actors/ovl_En_Ishi/z_en_ishi.c",
            "src/overlays/actors/ovl_En_Bigokuta/z_en_bigokuta.c",
            # colliders in another file from code
            "src/overlays/actors/ovl_Boss_Mo/z_boss_mo_colchk.c",
            "src/overlays/actors/ovl_Boss_Sst/z_boss_sst_colchk.c",
            "src/overlays/actors/ovl_Boss_Fd/z_boss_fd_colchk.c",
            "src/overlays/actors/ovl_Boss_Fd2/z_boss_fd2_colchk.c",
            "src/overlays/actors/ovl_Boss_Dodongo/z_boss_dodongo_data.c",
            "src/overlays/actors/ovl_Boss_Ganon2/z_boss_ganon2_data.c",
            # array of ColliderCylinderInitType1
            "src/overlays/actors/ovl_En_Gb/z_en_gb.c",
        )
    ):
        continue

    print(c_file_path)
    c_file_original = c_file_path.read_text()

    if DEBUG_ONCE:
        if "ColliderCylinderInit" not in c_file_original:
            continue
        if c_file_path.stem != "z_bg_hidan_firewall":
            continue

    c_file_processed = c_file_original

    for (target_struct_name, pattern_struct_name) in collider_init_structs:
        print(target_struct_name)

        c_file_split_at_struct_usage = pattern_struct_name.split(c_file_processed)
        c_file_parts: List[str] = []
        c_file_replace_post = dict()

        c_file_parts.append(c_file_split_at_struct_usage.pop(0))

        if DEBUG:
            print(len(c_file_split_at_struct_usage))

        while c_file_split_at_struct_usage:
            part_after_struct_split = c_file_split_at_struct_usage.pop(0)
            parts_after_struct_split = part_after_struct_split.split(";", 1)
            assert (
                len(parts_after_struct_split) == 2
            ), "is there a ; missing in parts_after_struct_split after the data definition?"
            data_definition, rest_of_part_after_struct_split = parts_after_struct_split
            if DEBUG:
                print("data_definition =")
                print(data_definition)
            if (
                data_definition.lstrip().startswith("*")  # pointer, false positive
                or "=" not in data_definition  # forward declaration
            ):
                c_file_parts[-1] += target_struct_name + part_after_struct_split
                continue

            data_definition_assignment = data_definition.split("=", 2)
            assert (
                len(data_definition_assignment) == 2
            ), "expected data_definition to look like 'symbol_name = ...'"
            data_symbol_name, data_definition_rhs = data_definition_assignment
            data_symbol_name = data_symbol_name.strip()
            data_definition_rhs = data_definition_rhs.lstrip()
            if DEBUG:
                print("data_symbol_name =", data_symbol_name)

            if data_symbol_name.endswith("]"):
                (
                    data_symbol_name,
                    after_opening_array_length_brackets,
                ) = data_symbol_name.split("[")
                data_symbol_name = data_symbol_name.strip()
                symbol_qualifier_last = "[" + after_opening_array_length_brackets
            else:
                symbol_qualifier_last = ""

            if action == Action.REMOVE_STATIC:
                previous_part = c_file_parts[-1]
                previous_part = previous_part.rstrip()
                if previous_part.endswith("static"):
                    previous_part = previous_part[: -len("static")]
                    c_file_parts[-1] = previous_part

                new_data_symbol_name = data_symbol_name + "_" + c_file_path.stem
                c_file_replace_post[data_symbol_name] = new_data_symbol_name

                c_file_parts.append(target_struct_name)
                c_file_parts.append(" ")
                c_file_parts.append(new_data_symbol_name)
                c_file_parts.append(symbol_qualifier_last)
                c_file_parts.append(" = ")
                c_file_parts.append(data_definition_rhs)
                c_file_parts.append(";")

            if action == Action.RECONSTRUCT:
                (objfile, section, ram, rom, symbol) = mapparser.find_symbol(
                    data_symbol_name
                )
                if DEBUG:
                    print(objfile, section, hex(ram), hex(rom), symbol)

                # new_data_definition = "/*\n" + (" TODO" * 10 + "\n") * 10 + "*/"

                if symbol_qualifier_last == "":
                    new_data_definition = (
                        mycolliderinit.parse_and_format_collider_init_data(
                            target_struct_name, rom
                        )
                    )
                else:  # array
                    if target_struct_name.endswith("ElementInit"):
                        new_data_definition = mycolliderinit.parse_and_format_collider_init_data_array_cached_length(
                            target_struct_name, rom, data_symbol_name
                        )
                    else:
                        raise NotImplementedError("array of " + target_struct_name)

                if DEBUG:
                    print("new_data_definition =")
                    print(new_data_definition)

                c_file_parts.append(target_struct_name)
                c_file_parts.append(" ")
                c_file_parts.append(data_symbol_name)
                c_file_parts.append(symbol_qualifier_last)
                c_file_parts.append(" = ")
                c_file_parts.append(new_data_definition)
                c_file_parts.append(";")

            c_file_parts.append(rest_of_part_after_struct_split)

        c_file_processed = "".join(c_file_parts)
        for search, replacement in c_file_replace_post.items():
            c_file_processed = re.sub(
                r"\b" + search + r"\b", replacement, c_file_processed
            )

    if not DEBUG_NOREPLACE:
        if c_file_processed != c_file_original:
            c_file_path.write_text(c_file_processed)

    if DEBUG_ONCE:
        if DEBUG_NOREPLACE:
            Path("OUT_colliderdata_reextractor.c").write_text(c_file_processed)
        break
