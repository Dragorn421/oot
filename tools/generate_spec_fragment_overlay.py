import sys
from pathlib import Path

spec_fragment_p = Path(sys.argv[1])

assert spec_fragment_p.is_relative_to("build")
assert spec_fragment_p.name == "spec_fragment"

src_folder = spec_fragment_p.relative_to("build").parent

segment_name = src_folder.name

segment_src_c_paths = [
    p for p in src_folder.glob("*.c") if not p.name.endswith(".inc.c")
]
# For matching, as it's unlikely the order would be right
# (there may be a working heuristic for ordering, TODO investigate)
assert len(segment_src_c_paths) == 1, (segment_name, spec_fragment_p)

segment_src_o_paths = [Path("build") / p.with_suffix(".o") for p in segment_src_c_paths]

reloc_file_p = Path("build") / src_folder / f"{segment_name}_reloc.o"


spec_fragment_p.write_text(
    f"""\
beginseg
    name "{segment_name}"
"""
    + "".join(f'    include "{p}"\n' for p in segment_src_o_paths)
    + f"""\
    include "{reloc_file_p}"
endseg
"""
)
