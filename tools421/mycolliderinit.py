# SPDX-License-Identifier: CC0-1.0 OR Unlicense

import ctypes
from lib2to3.pytree import convert
import sys
import os
from pathlib import Path
import struct

import mapparser

from typing import (
    TYPE_CHECKING,
    Optional,
)

if TYPE_CHECKING:
    import C2Py.C2Py.C2PyHandler as C2PyHandler
else:
    sys.path.insert(0, "/home/dragorn421/Documents/oot/tools421/C2Py/")
    import C2Py.C2PyHandler as C2PyHandler


baserom_bytes: Optional[bytes] = None
z64collision_check: Optional[C2PyHandler.DefaultRuntimeBufferC2PyHandler] = None

cached_lengths = dict()


def load_baserom_bytes():
    global baserom_bytes
    baserom_bytes = Path("baserom.z64").read_bytes()


def load_z64collision_check():
    global z64collision_check
    z64collision_check = C2PyHandler.DefaultRuntimeBufferC2PyHandler(
        # cpp -Iinclude -I. -D_LANGUAGE_C include/z64.h > tools421/z64.i
        "/home/dragorn421/Documents/oot/tools421/z64.i"
    )


def ctype_to_c(data, data_type):
    if issubclass(data_type, ctypes.Structure):
        fmt_nl = " "
        fmt_ind = ""
        fmt_fieldnames = data.struct_type not in {"Vec3s", "Vec3f"}
        if data.struct_type in {
            "ColliderCylinderInit",
            "Cylinder16",
            "ColliderQuadInit",
            # "ColliderQuadDimInit",
            "ColliderInit",
            "ColliderInfoInit",
            "ColliderTouch",
            "ColliderBumpInit",
        }:
            fmt_nl = "\n"
            fmt_ind = "    "

        def format_field(field_name, field_type):
            if field_name == "elements":
                symbol_name = mapparser.find_address(getattr(data, field_name))[4]
                cached_lengths[symbol_name] = data.count
                return symbol_name
            return (f"/* {field_name} */ " if fmt_fieldnames else "") + ctype_to_c(
                getattr(data, field_name), field_type
            )

        return (
            "{"
            + fmt_nl
            + fmt_ind
            + ("," + fmt_nl)
            .join(
                format_field(field_name, field_type)
                for field_name, field_type in data._fields_
            )
            .replace("\n", "\n" + fmt_ind)
            + fmt_nl
            + "}"
        )
    elif issubclass(data_type, ctypes.Array):
        return "{ " + ", ".join(ctype_to_c(e, data._type_) for e in data) + " }"
    elif data_type._type_ == "I":  # u32
        return f"0x{data:08X}"
    elif data_type._type_ == "B":  # u8
        return f"0x{data:02X}"
    elif any(
        data_type._type_ == type_for_decimal
        for type_for_decimal in (
            "i",  # s32
            "h",  # s16
        )
    ):
        return f"{data:d}"
    elif data_type._type_ == "f":  # f32
        c_str = f"{data:g}"
        if "." not in c_str:
            c_str += ".0"
        c_str += "f"
        return c_str
    else:
        raise NotImplementedError(data_type, type(data))


def parse_and_format_collider_init_data(struct_name, rom_offset):
    data = z64collision_check.convert(struct_name, rom_offset, baserom_bytes)
    print(str(data))
    c = ctype_to_c(data, ctypes.Structure)
    return c


def parse_and_format_collider_init_data_array(struct_name, rom_offset, length):
    sizes = {
        "ColliderJntSphElementInit": 0x24,
        "ColliderTrisElementInit": 0x3C,
    }
    size = sizes[struct_name]
    indent = "    "
    return (
        "{\n"
        + indent
        + ",\n".join(
            f"/* {i} */ "
            + parse_and_format_collider_init_data(struct_name, rom_offset + size * i)
            for i in range(length)
        ).replace("\n", "\n" + indent)
        + "\n}"
    )


def parse_and_format_collider_init_data_array_cached_length(
    struct_name, rom_offset, symbol_name
):
    return parse_and_format_collider_init_data_array(
        struct_name, rom_offset, cached_lengths[symbol_name]
    )
