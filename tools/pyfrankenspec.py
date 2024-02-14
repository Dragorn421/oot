# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

from __future__ import annotations

import enum
import dataclasses
from pathlib import Path
import json


class SectionName(enum.Enum):
    TEXT = ".text"
    DATA = ".data"
    RODATA = ".rodata"
    BSS = ".bss"
    OVL = ".ovl"


ALL_SECTIONS = frozenset(SectionName)


@dataclasses.dataclass
class FrankenSpecSegmentFrankenelf:
    default: set[SectionName] = ALL_SECTIONS
    perfile: dict[Path, set[SectionName]] = dataclasses.field(default_factory=dict)

    def get_sections(self, include_object_p: Path):
        return self.perfile.get(include_object_p, self.default)


@dataclasses.dataclass
class FrankenSpecSegment:
    baseromify: bool = False
    frankenelf: FrankenSpecSegmentFrankenelf | None = None
    override_vram: int | None = None
    override_rom: int | None = None


FRANKENSPEC_SEG_DEFAULT = FrankenSpecSegment()


@dataclasses.dataclass
class FrankenSpec:
    segments: dict[str, FrankenSpecSegment] = dataclasses.field(default_factory=dict)

    def get(self, segment_name: str):
        return self.segments.get(segment_name, FRANKENSPEC_SEG_DEFAULT)


def parse_section_names(section_names_str: str):
    if section_names_str == "*":
        return ALL_SECTIONS
    section_names: set[SectionName] = set()
    for name_str in section_names_str.split():
        name = SectionName[name_str.upper()]
        section_names.add(name)
    return section_names


def parse_frankenspec(json_p: Path):
    with json_p.open(encoding="utf-8") as f:
        data = json.load(f)
    assert isinstance(data, dict), data

    frankenspec = FrankenSpec()

    data_baseromify = data["baseromify"]
    assert isinstance(data_baseromify, list), data_baseromify
    for data_baseromify_segment_name in data_baseromify:
        assert isinstance(
            data_baseromify_segment_name, str
        ), data_baseromify_segment_name
        seg = frankenspec.segments.setdefault(
            data_baseromify_segment_name, FrankenSpecSegment()
        )
        seg.baseromify = True

    data_frankenelf = data["frankenelf"]
    assert isinstance(data_frankenelf, dict), data_frankenelf
    for (
        data_frankenelf_segment_name,
        data_frankenelf_segment_info,
    ) in data_frankenelf.items():
        assert isinstance(
            data_frankenelf_segment_name, str
        ), data_frankenelf_segment_name
        seg = frankenspec.segments.setdefault(
            data_frankenelf_segment_name, FrankenSpecSegment()
        )
        seg.frankenelf = FrankenSpecSegmentFrankenelf()

        assert isinstance(
            data_frankenelf_segment_info, dict
        ), data_frankenelf_segment_info

        data_frankenelf_segment_info_default = data_frankenelf_segment_info.get(
            "default"
        )
        if data_frankenelf_segment_info_default is not None:
            assert isinstance(
                data_frankenelf_segment_info_default, str
            ), data_frankenelf_segment_info_default
            seg.frankenelf.default = parse_section_names(
                data_frankenelf_segment_info_default
            )

        data_frankenelf_segment_info_perfile = data_frankenelf_segment_info.get(
            "perfile"
        )
        if data_frankenelf_segment_info_perfile is not None:
            assert isinstance(
                data_frankenelf_segment_info_perfile, dict
            ), data_frankenelf_segment_info_perfile
            for (
                include_object,
                section_names_str,
            ) in data_frankenelf_segment_info_perfile.items():
                assert isinstance(include_object, str), include_object
                assert isinstance(section_names_str, str), section_names_str
                include_object_p = Path(include_object)
                section_names = parse_section_names(section_names_str)
                seg.frankenelf.perfile[include_object_p] = section_names

    data_override = data["override"]
    assert isinstance(data_override, dict), data_override
    for (
        data_override_segment_name,
        data_override_segment_info,
    ) in data_override.items():
        assert isinstance(data_override_segment_name, str), data_override_segment_name
        seg = frankenspec.segments.setdefault(
            data_override_segment_name, FrankenSpecSegment()
        )

        assert isinstance(data_override_segment_info, dict), data_override_segment_info

        data_override_segment_info_vram_str = data_override_segment_info.get("vram")
        if data_override_segment_info_vram_str is not None:
            assert isinstance(
                data_override_segment_info_vram_str, str
            ), data_override_segment_info_vram_str
            data_override_segment_info_vram = int(
                data_override_segment_info_vram_str, 16
            )
            seg.override_vram = data_override_segment_info_vram

        data_override_segment_info_rom_str = data_override_segment_info.get("rom")
        if data_override_segment_info_rom_str is not None:
            assert isinstance(
                data_override_segment_info_rom_str, str
            ), data_override_segment_info_rom_str
            data_override_segment_info_rom = int(data_override_segment_info_rom_str, 16)
            seg.override_rom = data_override_segment_info_rom

    return frankenspec


def main():
    from pprint import pprint

    frankenspec_json_p = Path("frankenspec.json")
    frankenspec = parse_frankenspec(frankenspec_json_p)
    pprint(frankenspec)


if __name__ == "__main__":
    main()
