# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0


from __future__ import annotations

import enum
import dataclasses
from pathlib import Path
from typing import Iterable


class SpecIncludeSection(enum.Flag):
    ALL = enum.auto()
    TEXT = enum.auto()
    DATA = enum.auto()
    RODATA = enum.auto()
    BSS = enum.auto()


SPECINCLUDESECTIONFLAG_BY_SECTIONNAME = {
    ".text": SpecIncludeSection.TEXT,
    ".data": SpecIncludeSection.DATA,
    ".rodata": SpecIncludeSection.RODATA,
    ".bss": SpecIncludeSection.BSS,
}
SECTIONNAME_BY_SPECINCLUDESECTIONFLAG = {
    v: k for k, v in SPECINCLUDESECTIONFLAG_BY_SECTIONNAME.items()
}


@dataclasses.dataclass
class SpecInclude:
    file: Path
    includeSectionsFlags: SpecIncludeSection = SpecIncludeSection.ALL
    dataWithRodata: bool = False
    pad_text: bool = False


class SpecSegmentFlag(enum.Flag):
    NOLOAD = enum.auto()


@dataclasses.dataclass
class SpecSegment:
    name: str = None
    flags: SpecSegmentFlag = SpecSegmentFlag(0)
    compress: bool = False
    address: int | None = None  # segment start vma
    after: SpecSegment | None = None  # segment start vma is vma end of "after" segment
    align: int = 0
    romalign: int = 0
    includes: list[SpecInclude] = dataclasses.field(default_factory=list)
    number: int | None = None


@dataclasses.dataclass
class Spec:
    segments: list[SpecSegment]

    def find_segment_by_name(self, segment_name: str):
        matching_segments = [
            segment for segment in self.segments if segment.name == segment_name
        ]
        if len(matching_segments) == 1:
            return matching_segments[0]
        if len(matching_segments) == 0:
            raise KeyError("segment_name not found", segment_name)
        raise Exception("several segments have name", segment_name)


def parse_spec(lines: Iterable[str]):
    segments = []
    cur_segment: SpecSegment | None = None
    for line in lines:
        line = line.strip()
        if line == "beginseg":
            cur_segment = SpecSegment()
            segments.append(cur_segment)
        elif line == "endseg":
            assert cur_segment is not None
            cur_segment = None
        else:
            assert cur_segment is not None
            parts = line.split()
            directive = parts[0]
            args = parts[1:]
            if directive == "name":
                assert len(args) == 1
                quoted_name = args[0]
                assert quoted_name.startswith('"')
                assert quoted_name.endswith('"')
                cur_segment.name = quoted_name.removeprefix('"').removesuffix('"')
            elif directive == "flags":
                for arg in args:
                    cur_segment.flags |= SpecSegmentFlag[arg]
            elif directive == "compress":
                assert len(args) == 0
                cur_segment.compress = True
            elif directive == "address":
                assert len(args) == 1
                cur_segment.address = int(args[0], 16)
            elif directive == "after":
                assert len(args) == 1
                quoted_after_segment_name = args[0]
                assert quoted_after_segment_name.startswith('"')
                assert quoted_after_segment_name.endswith('"')
                after_segment_name = quoted_after_segment_name.removeprefix(
                    '"'
                ).removesuffix('"')
                assert cur_segment.after is None
                for seg in segments:
                    if seg.name == after_segment_name:
                        assert cur_segment.after is None, "after segment has >1 match"
                        cur_segment.after = seg
            elif directive == "align":
                assert len(args) == 1
                align = int(args[0], 16)
                cur_segment.align = align
            elif directive == "romalign":
                assert len(args) == 1
                romalign = int(args[0], 16)
                cur_segment.romalign = romalign
            elif directive in {"include", "include_data_with_rodata"}:
                assert len(args) >= 1
                quoted_filepath = args[0]
                assert quoted_filepath.startswith('"')
                assert quoted_filepath.endswith('"')
                filepath = quoted_filepath.removeprefix('"').removesuffix('"')
                include = SpecInclude(
                    Path(filepath),
                    dataWithRodata=directive == "include_data_with_rodata",
                )
                cur_segment.includes.append(include)
                if len(args) != 1:
                    include.includeSectionsFlags = SpecIncludeSection(0)
                    for arg in args[1:]:
                        include.includeSectionsFlags |= (
                            SPECINCLUDESECTIONFLAG_BY_SECTIONNAME[arg]
                        )
            elif directive == "pad_text":
                assert len(args) == 0
                cur_segment.includes[-1].pad_text = True
            elif directive == "number":
                assert len(args) == 1
                cur_segment.number = int(args[0])
            else:
                raise Exception("unknown directive", directive, line)

    return Spec(segments)


def stringify_spec(spec: Spec, indent=" " * 4):
    for segment in spec.segments:
        yield "beginseg"
        yield f'{indent}name "{segment.name}"'
        if segment.flags:
            yield f"{indent}flags " + " ".join(map(lambda f: f.name, segment.flags))
        if segment.compress:
            yield f"{indent}compress"
        if segment.address is not None:
            yield f"{indent}address 0x{segment.address:X}"
        if segment.after:
            yield f'{indent}after "{segment.after.name}"'
        if segment.align != 0:
            yield f"{indent}align 0x{segment.align:X}"
        if segment.romalign != 0:
            yield f"{indent}romalign 0x{segment.romalign:X}"
        for include in segment.includes:
            directive = (
                "include_data_with_rodata" if include.dataWithRodata else "include"
            )
            if include.includeSectionsFlags == SpecIncludeSection.ALL:
                includeSectionsFlags_str = ""
            else:
                includeSectionsFlags_str = "".join(
                    " " + SECTIONNAME_BY_SPECINCLUDESECTIONFLAG[flag]
                    for flag in include.includeSectionsFlags
                )
                assert includeSectionsFlags_str != ""
            d = f'{directive} "{include.file}"{includeSectionsFlags_str}'
            yield f"{indent}{d}"
            if include.pad_text:
                yield f"{indent}pad_text"
        if segment.number is not None:
            yield f"{indent}number {segment.number}"
        yield "endseg"


def parse_spec_p(spec_p: Path):
    with spec_p.open() as f:
        return parse_spec(f)


def write_spec_p(spec: Spec, spec_p: Path):
    with spec_p.open("w") as f:
        f.writelines(f"{line}\n" for line in stringify_spec(spec))


def main():
    base_spec_p = Path("build/gc-eu-mq-dbg/spec")
    with base_spec_p.open() as f:
        base_spec_text = f.read()
    base_spec_lines = base_spec_text.splitlines()
    spec = parse_spec(base_spec_lines)
    roundtrip_spec_lines = list(stringify_spec(spec))
    roundtrip_spec_text = "".join(f"{line}\n" for line in roundtrip_spec_lines)

    if base_spec_text == roundtrip_spec_text:
        print("OK! roundtrip spec is identical")
    else:
        print("Not ok. roundtrip spec is different:")

        import difflib

        for l in difflib.unified_diff(
            base_spec_lines,
            roundtrip_spec_lines,
            "base",
            "roundtrip",
        ):
            print(l)


if __name__ == "__main__":
    main()
