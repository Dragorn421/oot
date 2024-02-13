from __future__ import annotations

import sys
from pathlib import Path
import json
import enum
from pprint import pprint
from dataclasses import dataclass

import pyspec


VERSION = "gc-eu-mq"
EXPECTED_P = Path("expected")


class Section(enum.Enum):
    TEXT = enum.auto()
    DATA = enum.auto()
    RODATA = enum.auto()
    BSS = enum.auto()
    OVL = enum.auto()


spec_p = Path(sys.argv[1])
ldscript_p = Path(sys.argv[2])

print(f"{spec_p=}")
print(f"{ldscript_p=}")

spec = pyspec.parse_spec_p(spec_p)

with open("frankenspec.json", encoding="UTF-8") as f:
    frankenspec = json.load(f)

assert isinstance(frankenspec, dict)
segments_to_baseromify = frankenspec["baseromify"]
for segname in segments_to_baseromify:
    spec.find_segment_by_name(segname)
frankenelf_by_segname: dict[str, dict[Path, list[Section]]] = dict()
assert isinstance(frankenspec["frankenelf"], dict)
for segname, info in frankenspec["frankenelf"].items():
    assert segname not in segments_to_baseromify
    seg = spec.find_segment_by_name(segname)
    seg_frankenelf = dict()
    frankenelf_by_segname[seg.name] = seg_frankenelf
    assert isinstance(info, dict)
    if "perfile" in info:
        info_perfile = info["perfile"]
        assert isinstance(info_perfile, dict)
        for p_str, sections_str in info_perfile.items():
            p_p = Path(p_str)
            assert p_p in (inc.file for inc in seg.includes), (p_p, seg)
            assert isinstance(sections_str, str)
            if sections_str == "*":
                sections = list(Section)
            else:
                sections = []
                for sec_str in sections_str.split():
                    sec = Section[sec_str.upper()]
                    sections.append(sec)
            seg_frankenelf[p_p] = sections
    if "default" in info:
        info_default = info["default"]
        assert isinstance(info_default, str)
        if info_default == "*":
            default_sections = list(Section)
        else:
            default_sections = []
            for sec_str in info_default.split():
                sec = Section[sec_str.upper()]
                default_sections.append(sec)
        for inc in seg.includes:
            seg_frankenelf.setdefault(inc.file, default_sections)
assert isinstance(frankenspec["override"], dict)


@dataclass
class SegOverrides:
    vram: int | None = None
    rom: int | None = None


segoverride_by_segname: dict[str, SegOverrides] = dict()
for segname, segoverrides in frankenspec["override"].items():
    vram_str = segoverrides.get("vram")
    if vram_str is not None:
        vram = int(vram_str, 16)
    else:
        vram = None
    rom_str = segoverrides.get("rom")
    if rom_str is not None:
        rom = int(rom_str, 16)
    else:
        rom = None
    segoverride_by_segname[segname] = SegOverrides(vram, rom)


with open(Path(__file__).with_suffix(".dumplog.txt"), "w") as f:
    f.write("spec =\n")
    pprint(spec, stream=f)
    f.write("\n" * 10)
    f.write("segments_to_baseromify =\n")
    pprint(segments_to_baseromify, stream=f)
    f.write("\n" * 10)
    f.write("frankenelf_by_segname =\n")
    pprint(frankenelf_by_segname, stream=f)
    f.write("\n" * 10)

with ldscript_p.open("w") as f:
    f.write('LD_FEATURE("SANE_EXPR")\n')
    f.write("\n")
    f.write("SECTIONS {\n")
    f.write(".rom = 0;\n")
    f.write("\n")
    for seg in spec.segments:
        segoverrides = segoverride_by_segname.get(seg.name)
        segment_in_rom = not (seg.flags & pyspec.SpecSegmentFlag.NOLOAD)
        f.write(
            "\n"
            f"/* {seg.name} "
            f"{'(in rom)' if segment_in_rom else '(NOLOAD)'} */\n"
            "\n"
        )
        if seg.after:
            f.write(
                f'. = _{seg.after.name}SegmentEnd; /* after "{seg.after.name}" */\n'
            )
        if seg.address is not None:
            f.write(f". = 0x{seg.address:X}; /* address 0x{seg.address:X} */\n")
        if seg.number is not None:
            f.write(f". = 0x{seg.number:02X}000000; /* number {seg.number} */\n")
        if seg.align:
            f.write(f". = ALIGN(0x{seg.align:X}); /* align 0x{seg.align:X} */\n")
        if segoverrides and segoverrides.vram is not None:
            f.write(f". = 0x{segoverrides.vram:08X}; /* OVERRIDE vram */\n")
        f.write(f"_{seg.name}SegmentStart = ABSOLUTE(.);\n")
        if seg.romalign:
            f.write(
                f".rom = ALIGN(.rom, 0x{seg.romalign:X});"
                f" /* romalign 0x{seg.romalign:X} */\n"
            )
        if segoverrides and segoverrides.rom is not None:
            assert segment_in_rom
            f.write(f".rom = 0x{segoverrides.rom:08X}; /* OVERRIDE rom */\n")
        if segment_in_rom:
            # TODO remove ABSOLUTE s when it works (pretty sure they're redundant (at least some))
            f.write(f"_{seg.name}SegmentRomStart = ABSOLUTE(.rom);\n")
        f.write(f"..{seg.name} _{seg.name}SegmentStart")
        if segment_in_rom:
            f.write(f" : AT(_{seg.name}SegmentRomStart) ")
        else:
            f.write("\n")
            f.write("(NOLOAD) /* flags NOLOAD */\n")
            f.write(": ")
        f.write("SUBALIGN(0)\n")
        f.write("{\n")
        if seg.name in segments_to_baseromify:
            baserom_object_p = Path(f"build/{VERSION}/baserom/{seg.name}.o")
            assert baserom_object_p.exists(), baserom_object_p
            f.write(f"{baserom_object_p} (*)\n")
        else:
            seg_frankenelf = frankenelf_by_segname.get(seg.name)
            for section in Section:
                section_in_rom = section != Section.BSS and segment_in_rom
                f.write(
                    "\n"
                    f"/* {seg.name} .{section.name.lower()}"
                    f" {'(in rom)' if section_in_rom else '(NOLOAD)'} */\n"
                )
                f.write(f"_{seg.name}Segment{section.name.capitalize()}Start = .;\n")
                if section_in_rom:
                    f.write(
                        f"_{seg.name}Segment{section.name.capitalize()}RomStart = ABSOLUTE(.rom);\n"
                    )
                for inc in seg.includes:
                    if seg_frankenelf is None or inc.file not in seg_frankenelf:
                        assert inc.file.exists(), inc.file
                        f.write(f"{inc.file} (.{section.name.lower()})\n")
                        continue
                    inc_sections = seg_frankenelf[inc.file]
                    expected_inc_file = EXPECTED_P / inc.file
                    if not inc_sections:
                        assert expected_inc_file.exists(), expected_inc_file
                        f.write(f"{expected_inc_file} (.{section.name.lower()})\n")
                        continue
                    # mix and match
                    # individually-assembled-disassembled-sections
                    # and
                    # frankenelf'd build objects
                    if section in inc_sections:
                        file = inc.file
                        other = expected_inc_file
                    else:
                        file = expected_inc_file
                        other = inc.file
                    file = file.with_suffix(f".{section.name.lower()}.o")
                    other = other.with_suffix(f".{section.name.lower()}.o")
                    if file.exists() != other.exists():
                        print(
                            f"Note: build file and expected file don't both have / not have a .{section.name.lower()} section:"
                        )
                        print(
                            "exists" if file.exists() else "does not exist", ":", file
                        )
                        print(
                            "exists" if other.exists() else "does not exist", ":", other
                        )
                    f.write(f"{inc.file}.{section.name.lower()} = .;\n")
                    if file.exists():
                        f.write(f"{file} (.{section.name.lower()})\n")
                    else:
                        f.write(f"/* skip link section bc not exists {file} */\n")
                    if other.exists():
                        f.write(f"INCLUDE {other.with_suffix('.syms.txt')};\n")
                    else:
                        f.write(f"/* skip include syms bc not exists {other} */\n")
                f.write(f"_{seg.name}Segment{section.name.capitalize()}End = .;\n")
                f.write(
                    f"_{seg.name}Segment{section.name.capitalize()}Size"
                    f" = ABSOLUTE(_{seg.name}Segment{section.name.capitalize()}End"
                    f" - _{seg.name}Segment{section.name.capitalize()}Start);\n"
                )
                if section_in_rom:
                    f.write(
                        f".rom = ABSOLUTE(.rom + _{seg.name}Segment{section.name.capitalize()}Size);\n"
                    )
                    f.write(
                        f"_{seg.name}Segment{section.name.capitalize()}RomEnd = ABSOLUTE(.rom);\n"
                    )
        f.write("}\n")
        f.write(f"_{seg.name}SegmentEnd = .;\n")
        if seg.name in segments_to_baseromify:
            if segment_in_rom:
                f.write(
                    f".rom = ABSOLUTE(.rom + (_{seg.name}SegmentEnd - _{seg.name}SegmentStart));\n"
                )
        if segment_in_rom:
            f.write(f"_{seg.name}SegmentRomEnd = .rom;\n")
        # f.write(f"_{seg.name}SegmentSize = _{seg.name}SegmentEnd - _{seg.name}SegmentStart;\n")
        # f.write(f"_{seg.name}SegmentRomEnd = .rom;\n")
        f.write("\n")
        f.write("\n")
    f.write("\n")
    f.write("\n")
    # the reason to align like this still unknown besides "it's what is currently done"
    f.write("_RomSize = ALIGN(.rom, 0x1000);\n")
    f.write("\n")
    f.write("/DISCARD/ : { *(*) }\n")
    f.write("\n")
    f.write("}\n")
