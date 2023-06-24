# SPDX-License-Identifier: CC0-1.0

from pathlib import Path
from dataclasses import dataclass
from pprint import pprint

from tqdm import tqdm


SPEC_P = Path("spec")

seglines: list[str] = []


@dataclass
class LocatedSpecPart:
    common_parent: Path
    seglines: list[str]


@dataclass
class SpecPart:
    lines: list[str]


divided_spec: list[LocatedSpecPart | SpecPart] = []

for line in tqdm(SPEC_P.read_text().splitlines(keepends=True)):
    if line.strip() == "beginseg":
        inseg = True
        if seglines:
            divided_spec.append(SpecPart(seglines))
        seglines = [line]
    elif line.strip() == "endseg":
        assert inseg
        seglines.append(line)

        include_paths: list[Path] = []
        for segl in seglines:
            segl_parts = segl.split()
            if segl_parts and segl_parts[0] == "include":
                path_str = segl_parts[1].removeprefix('"').removesuffix('"')
                include_paths.append(Path(path_str))
        common_parent = None
        for parent in include_paths[0].parents:
            if all(p.is_relative_to(parent) for p in include_paths):
                common_parent = parent
                break

        divided_spec.append(LocatedSpecPart(common_parent, seglines))

        inseg = False
        seglines = []
    else:
        seglines.append(line)

if seglines:
    divided_spec.append(SpecPart(seglines))
paths_for_splitting = [
    Path("build/assets/objects"),
    Path("build/assets/textures"),
    Path("build/assets/scenes"),
    Path("build/src/overlays"),
]

# pprint(divided_spec)

divided_spec_filtered = []

prev_lsp = None
included_lsps_by_common_parent_ordered: dict[Path, list[LocatedSpecPart]] = dict()

for part in divided_spec:
    if isinstance(part, LocatedSpecPart):
        lsp = part
        ok_for_divide_p = None
        for p in paths_for_splitting:
            if lsp.common_parent.is_relative_to(p):
                ok_for_divide_p = p
                break
        if ok_for_divide_p is None:
            divided_spec_filtered.append(SpecPart(lsp.seglines))
        else:
            if lsp.common_parent in included_lsps_by_common_parent_ordered:
                # ensure all commonly parented lsps come in a row
                assert (
                    prev_lsp is None
                    or prev_lsp
                    in included_lsps_by_common_parent_ordered[lsp.common_parent]
                )
            else:  # only include a lsp stride once
                divided_spec_filtered.append(
                    SpecPart(
                        [
                            '#include "'
                            + str(
                                lsp.common_parent.relative_to("build") / "spec_fragment"
                            )
                            + '"\n'
                        ]
                    )
                )
            included_lsps_by_common_parent_ordered.setdefault(
                lsp.common_parent, []
            ).append(lsp)
        prev_lsp = lsp
    else:
        divided_spec_filtered.append(part)
        prev_lsp = None

# collect lines
lines1: list[str] = []

for sp in divided_spec_filtered:
    lines1.extend(sp.lines)

# remove empty lines between includes and doubled+ empty lines
lines2: list[str] = []

prev_line_is_include = False
prev_line_is_removed_space = False
for i, line in enumerate(lines1):
    line_is_empty = line == "\n"
    line_is_include = line.startswith("#include")
    if line_is_empty and (prev_line_is_include or prev_line_is_removed_space):
        # swallow line
        prev_line_is_removed_space = True
    else:
        if prev_line_is_removed_space and not line_is_include:
            # add back swallowed empty line
            lines2.append("\n")
        lines2.append(line)
        prev_line_is_removed_space = False
    prev_line_is_include = line_is_include

SPEC_P.with_suffix(".divided2").write_text("".join(lines2))

# group #include blocks by path (scenes, objects, ...)
lines3: list[str] = []
paths_for_grouping = [
    Path("assets/objects"),
    # Path("assets/textures"),
    Path("assets/scenes"),
    Path("src/overlays"),
]
grouped_includes: dict[Path, list[str]] = dict()
spec_part_p_by_group_p: dict[Path, Path] = dict()
prev_line_group_p = None
hack_put_gameplay_object_exchange_static_with_objects_include = False

for i, line in enumerate(lines2):
    if line == '#include "assets/objects/object_link_boy/spec_fragment"\n':
        hack_put_gameplay_object_exchange_static_with_objects_include = False
    if hack_put_gameplay_object_exchange_static_with_objects_include:
        grouped_includes[Path("assets/objects")].append(line)
        continue
    if line == '#include "assets/objects/gameplay_dangeon_keep/spec_fragment"\n':
        hack_put_gameplay_object_exchange_static_with_objects_include = True
    is_grouped_include = False
    if line.startswith("#include"):
        matching_group_p = None
        for group_p in paths_for_grouping:
            if line.startswith('#include "' + str(group_p)):
                matching_group_p = group_p
                break
        if matching_group_p:
            spec_part_p = matching_group_p / "spec_part"
            spec_part_p_by_group_p[matching_group_p] = spec_part_p
            is_grouped_include = True
            if matching_group_p in grouped_includes:
                assert prev_line_group_p == matching_group_p, (
                    line,
                    prev_line_group_p,
                    matching_group_p,
                )
            else:
                if prev_line_group_p is not None:
                    # if two spec_part includes follow, add a line break in between
                    lines3.append("\n")
                lines3.append('#include "' + str(spec_part_p) + '"\n')
            prev_line_group_p = matching_group_p
            grouped_includes.setdefault(matching_group_p, []).append(line)
    if not is_grouped_include:
        lines3.append(line)
        prev_line_group_p = None

SPEC_P.with_suffix(".divided3").write_text("".join(lines3))
SPEC_P.write_text("".join(lines3))

for group_p, lines in grouped_includes.items():
    spec_part_p_by_group_p[group_p].write_text("".join(lines))

for parent_p, lsps in included_lsps_by_common_parent_ordered.items():
    spec_fragment_p = parent_p.relative_to("build") / "spec_fragment"
    spec_fragment_p.write_text("" + "".join("".join(lsp.seglines) for lsp in lsps))

# diff build/spec expected/build/spec -s --unified
