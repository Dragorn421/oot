# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>

import argparse
import os.path
import re
import struct

parser = argparse.ArgumentParser()
parser.add_argument("spec_file")
parser.add_argument("dmadata_h_file")
parser.add_argument("rom_file")
parser.add_argument("--zscenes", nargs="*")
parser.add_argument("--zsceneso", nargs="*")
parser.add_argument("--zmaps", nargs="*")
parser.add_argument("--zmapso", nargs="*")
parser.add_argument("--verbose", action="store_true")
args = parser.parse_args()

if args.verbose:
    print("args =", args)

assert len(args.zscenes) == len(args.zsceneso)
zscenes = list(zip(args.zscenes, args.zsceneso))
assert len(args.zmaps) == len(args.zmapso)
zmaps = list(zip(args.zmaps, args.zmapso))

if args.verbose:
    print("zscenes =", zscenes)
    print("zmaps =", zmaps)

# with spec, get file names using zscenes and zmaps

file_by_zscene = {}
file_by_zmap = {}

with open(args.spec_file) as f:
    seg_name = None
    dict_file_by = None
    dict_key = None
    for line in f:
        line = line.strip()
        if line.startswith("name"):
            seg_name = line.split(maxsplit=2)[1][1:-1]
        if line.startswith("endseg"):
            if dict_key is not None:
                dict_file_by[dict_key] = seg_name
                dict_key = None
                dict_file_by = None
            seg_name = None
        for zscene, zsceneo in zscenes:
            if zsceneo in line:
                assert dict_file_by is None
                assert dict_key is None
                dict_file_by = file_by_zscene
                dict_key = zscene
        for zmap, zmapo in zmaps:
            if zmapo in line:
                assert dict_file_by is None
                assert dict_key is None
                dict_file_by = file_by_zmap
                dict_key = zmap

if args.verbose:
    print("file_by_zscene =", file_by_zscene)
    print("file_by_zmap =", file_by_zmap)
"""
file_by_zscene = {
    "assets/scenes/overworld/spot00/spot00_scene.zscene": "spot00_scene",
}
file_by_zmap = {
    "assets/scenes/overworld/spot00/spot00_room_0.zmap": "spot00_room_0",
}
"""

# with dmadata h file, get dma table entry index for each file
fetch_files = set(file_by_zscene.values()) | set(file_by_zmap.values())
if args.verbose:
    print("fetch_files =", fetch_files)

dmatable_index_by_file = {}
index = 0
with open(args.dmadata_h_file) as f:
    for line in f:
        line = line.strip()
        if line == "":
            continue
        m = re.match(r"^DEFINE_DMA_ENTRY\(([a-zA-Z_0-9]+),\s*\".*\"\)$", line)
        assert m is not None
        file = m.group(1)
        if file in fetch_files:
            dmatable_index_by_file[file] = index
        index += 1

if args.verbose:
    print("dmatable_index_by_file =", dmatable_index_by_file)
"""
dmatable_index_by_file = {
    "spot00_scene": 1246,
    "spot00_room_0": 1247,
}
"""

# with map, get gDmaDataTable rom offset
fetch_symbols = [
    "_dmadataSegmentRomStart",
    "_dmadataSegmentStart",
    "gDmaDataTable",
]
syms = {}
with open("build/z64.map") as f:
    for line in f:
        line = line.strip()
        for sym in fetch_symbols:
            if sym in line:
                val_str = line.split()[0]
                val = int(val_str, 16)
                syms[sym] = val

if args.verbose:
    print("syms =")
    for name, val in syms.items():
        print(name, "=", "0x{:08X}".format(val))
"""
syms = {
    "_dmadataSegmentRomStart": 0x0000000000012F70,
    "_dmadataSegmentStart": 0x0000000080016DA0,
    "gDmaDataTable": 0x0000000080016DA0,
}
"""

dmatable_rom = syms["gDmaDataTable"] - (
    syms["_dmadataSegmentStart"] - syms["_dmadataSegmentRomStart"]
)

if args.verbose:
    print("dmatable_rom = 0x{:X}".format(dmatable_rom))

# with rom, get files offsets
with open(args.rom_file, "rb") as f:
    rom_data = f.read()

vrom_offsets_by_file = {}

for file, dmatable_index in dmatable_index_by_file.items():
    vrom_start, vrom_end, rom_start, rom_end = struct.unpack_from(
        ">IIII", rom_data, dmatable_rom + 0x10 * dmatable_index
    )
    if args.verbose:
        xxx = dmatable_rom + 0x10 * dmatable_index
        print(file, hex(xxx))
        print(rom_data[xxx:xxx+0x10])
        print(" ".join("{:08X}".format(v) for v in (vrom_start, vrom_end, rom_start, rom_end)))
    vrom_offsets_by_file[file] = (vrom_start, vrom_end)

if args.verbose:
    print("vrom_offsets_by_file =", vrom_offsets_by_file)

# figure out which zmaps are rooms of which scenes
room_files_by_scene_file = {}
for zscene, scene_file in file_by_zscene.items():
    zscene_dir = os.path.dirname(zscene)
    room_files = []
    for zmap, room_file in file_by_zmap.items():
        if zmap.startswith(zscene_dir):
            room_files.append(room_file)
    # room files should be named (`name` in the spec) with their id/number as a suffix
    def find_room_id_in_file_name(room_file):
        m = re.search(r"([0-9]+)$", room_file)
        assert m is not None
        return int(m.group(1))
    room_files.sort(key=find_room_id_in_file_name)
    room_files_by_scene_file[scene_file] = room_files


if args.verbose:
    print("room_files_by_scene_file =", room_files_by_scene_file)
"""
room_files_by_scene_file = {
    "spot00_scene": ["spot00_room_0"],
}
"""

# update room pointers
u32_struct = struct.Struct(">I")
scene_header_command_struct = struct.Struct(">BBxxI")

rom_data = bytearray(rom_data)

for scene_file, room_files in room_files_by_scene_file.items():
    scene_vrom_start, scene_vrom_end = vrom_offsets_by_file[scene_file]
    if args.verbose:
        print("VROM", scene_file, "0x{:X}-0x{:X}".format(scene_vrom_start, scene_vrom_end))
    scene_data = rom_data[scene_vrom_start:scene_vrom_end]
    scene_header_offsets = [0]
    room_lists_updated = 0
    offset = 0
    code = None
    while code != 0x14:
        (code, data1, data2) = scene_header_command_struct.unpack_from(
            scene_data, offset
        )
        offset += scene_header_command_struct.size
        if code != 0x18:
            continue
        assert (data2 >> 24) == 0x02
        alternate_headers_list_offset = data2 & 0xFFFFFF
        if args.verbose:
            print(
                "alternate_headers_list_offset = 0x{:06X}".format(
                    alternate_headers_list_offset
                )
            )
        while alternate_headers_list_offset + u32_struct.size <= len(scene_data):
            (alternate_header_segment_offset,) = u32_struct.unpack_from(
                scene_data,
                alternate_headers_list_offset,
            )
            alternate_headers_list_offset += u32_struct.size
            if alternate_header_segment_offset == 0:
                continue
            if (alternate_header_segment_offset >> 24) != 0x02:
                break
            alternate_header_offset = alternate_header_segment_offset & 0xFFFFFF
            # check if the data at the offset looks like headers
            alt_offset = alternate_header_offset
            alt_code = None
            while (
                alt_code != 0x14
                and alt_offset + scene_header_command_struct.size <= len(scene_data)
            ):
                (
                    alt_code,
                    alt_data1,
                    alt_data2,
                ) = scene_header_command_struct.unpack_from(scene_data, alt_offset)
                alt_offset += scene_header_command_struct.size
                # invalid command
                if alt_code >= 0x1A:
                    break
                # invalid commands in the context of alternate scene headers
                if alt_code in {0x01, 0x0A, 0x0B, 0x18}:
                    break
                # valid commands if data2 is a valid segment offset
                # (the segment is always the scene file)
                if alt_code in {0x00, 0x03, 0x04, 0x06, 0x0E, 0x0F, 0x13}:
                    if (data2 >> 24) != 0x02:
                        break
                    if (data2 & 0xFFFFFF) >= len(scene_data):
                        break
            if alt_code == 0x14:
                scene_header_offsets.append(alternate_header_offset)
            else:
                break
    for scene_header_offset in scene_header_offsets:
        offset = scene_header_offset
        code = None
        while code != 0x14:  # SCENE_CMD_ID_END
            (code, data1, data2) = scene_header_command_struct.unpack_from(
                scene_data, offset
            )
            offset += scene_header_command_struct.size
            if code != 0x04:  # SCENE_CMD_ID_ROOM_LIST
                continue
            assert (data2 >> 24) == 0x02
            rooms_list_offset = data2 & 0xFFFFFF
            if args.verbose:
                print("rooms_list_offset = 0x{:06X}".format(rooms_list_offset))
            for room_index, room_file in enumerate(room_files):
                room_vrom_start, room_vrom_end = vrom_offsets_by_file[room_file]
                u32_struct.pack_into(
                    scene_data, rooms_list_offset + 8 * room_index + 0, room_vrom_start
                )
                u32_struct.pack_into(
                    scene_data, rooms_list_offset + 8 * room_index + 4, room_vrom_end
                )
            room_lists_updated += 1
    # this failing means no room list was found, this most likely means not looking at a scene file
    # may be due to not building with `NON_MATCHING=1` (then build/dmadata_table_spec.h is not used)
    # `make clean` may help
    assert room_lists_updated > 0
    rom_data[scene_vrom_start:scene_vrom_end] = scene_data

with open(args.rom_file, "wb") as f:
    f.write(rom_data)
