import sys
from pathlib import Path

spec_fragment_p = Path(sys.argv[1])

assert spec_fragment_p.is_relative_to("build")
assert spec_fragment_p.name == "spec_fragment"

src_folder = spec_fragment_p.relative_to("build").parent

segments_name_prefix = src_folder.name

segment_src_c_paths = [
    p for p in src_folder.glob("*.c") if not p.name.endswith(".inc.c")
]

segment_src_o_paths = [Path("build") / p.with_suffix(".o") for p in segment_src_c_paths]

segment_src_o_paths_scene = [p for p in segment_src_o_paths if "scene" in p.name]
segment_src_o_paths_rooms = [
    p for p in segment_src_o_paths if "room" in p.name and not p.stem.endswith("scene")
]
assert len(segment_src_o_paths_scene) == 1
assert len(segment_src_o_paths_rooms) >= 1
assert set(segment_src_o_paths) == (
    set(segment_src_o_paths_scene) | set(segment_src_o_paths_rooms)
)
assert len(set(segment_src_o_paths_scene) & set(segment_src_o_paths_rooms)) == 0, set(
    segment_src_o_paths_scene
) & set(segment_src_o_paths_rooms)

room_p_by_num = dict()
for room_p in segment_src_o_paths_rooms:
    stem = room_p.stem
    last_non_digit_char_i = max(
        [i for (i, c) in enumerate(stem) if c not in "0123456789"],
        default=-1,
    )
    room_num_str = stem[last_non_digit_char_i + 1 :]
    assert len(room_num_str) >= 1, stem
    room_num = int(room_num_str)
    assert room_num not in room_p_by_num, segment_src_o_paths_rooms
    room_p_by_num[room_num] = room_p
assert len(room_p_by_num) == len(segment_src_o_paths_rooms)
assert set(room_p_by_num.keys()) == set(range(len(room_p_by_num)))

spec_fragment_p.write_text(
    f"""\
beginseg
    name "{segments_name_prefix}_scene"
    romalign 0x1000
    include "{segment_src_o_paths_scene[0]}"
    number 2
endseg
"""
    + "".join(
        f"""\
beginseg
    name "{segments_name_prefix}_room_{room_num}"
    romalign 0x1000
    include "{room_p_by_num[room_num]}"
    number 3
endseg
"""
        for room_num in range(len(room_p_by_num))
    )
)
