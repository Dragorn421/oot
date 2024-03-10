# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

from __future__ import annotations

import sys
from pathlib import Path
from pprint import pprint
import io

import pyspec
import pyfrankenspec
from pyfrankenspec import SectionName


EXPECTED_P = Path("expected")


class IndentedWriter:
    def __init__(self, f: io.TextIOBase):
        self._f = f
        self.indent_prefix = ""
        self._is_line_start = True

    def write(self, s: str):
        lines = s.split("\n")

        if self._is_line_start:
            if lines[0] != "":
                self._f.write(self.indent_prefix)
            self._is_line_start = False

        if len(lines) == 1:
            self._f.write(lines[0])
            return

        self._f.write(lines[0])
        for line in lines[1:-1]:
            self._f.write("\n")
            self._f.write(self.indent_prefix)
            self._f.write(line)
        if lines[-1] == "":
            self._f.write("\n")
            self._is_line_start = True
        else:
            self._f.write("\n")
            self._f.write(self.indent_prefix)
            self._f.write(lines[-1])

    def writeline(self, s: str = ""):
        self.write(s)
        self.write("\n")

    w = write
    wl = writeline

    class IndentedWriterIndentedContextManager:
        def __init__(self, iw: IndentedWriter, add_indent: str):
            self.iw = iw
            self.add_indent = add_indent

        def __enter__(self):
            self.prev_indent_prefix = self.iw.indent_prefix
            self.iw.indent_prefix += self.add_indent

        def __exit__(self, exc_type, exc_value, traceback):
            self.iw.indent_prefix = self.prev_indent_prefix

    def indented(self, indent: str):
        return self.IndentedWriterIndentedContextManager(self, indent)


def test_indented_writer():
    iw = IndentedWriter(sys.stdout)
    iw.writeline("abc")
    iw.write("de")
    iw.writeline("f")
    with iw.indented("  "):
        iw.writeline("ghi")
        with iw.indented("  "):
            iw.write("jk")
        iw.writeline("l")
        iw.writeline("mno")
    iw.write("p")
    with iw.indented("  "):
        iw.writeline("qr")
        iw.write("s")
        with iw.indented("  "):
            iw.writeline("tu")
    iw.writeline("vwx")
    iw.writeline("a\nb\nc")
    with iw.indented("  "):
        iw.writeline("a\nb\nc")


if 0:
    test_indented_writer()
    exit(1)

spec_p = Path(sys.argv[1])
ldscript_p = Path(sys.argv[2])
VERSION = sys.argv[3]
assert VERSION in {"gc-eu-mq", "gc-eu-mq-dbg"}

print(f"{spec_p=}")
print(f"{ldscript_p=}")

spec = pyspec.parse_spec_p(spec_p)

if VERSION == "gc-eu-mq":
    frankenspec = pyfrankenspec.parse_frankenspec(Path("frankenspec.json"))
else:
    frankenspec = pyfrankenspec.FrankenSpec()


with open(Path(__file__).with_suffix(".dumplog.txt"), "w") as f:
    kwargs = dict(width=90, stream=f)
    f.write("spec =\n")
    pprint(spec, **kwargs)
    f.write("\n" * 10)
    f.write("frankenspec =\n")
    pprint(frankenspec, **kwargs)
    f.write("\n" * 10)

i = 0

with ldscript_p.open("w") as f:
    iw = IndentedWriter(f)
    iw.wl('LD_FEATURE("SANE_EXPR")')
    iw.wl()
    iw.wl("SECTIONS {")
    iw.wl(".rom = 0;")
    iw.wl()
    for seg in spec.segments:
        frankenspec_seg = frankenspec.get(seg.name)
        segment_in_rom = not (seg.flags & pyspec.SpecSegmentFlag.NOLOAD)
        iw.wl()
        iw.wl(f"/* {seg.name} " f"{'(in rom)' if segment_in_rom else '(NOLOAD)'} */")
        iw.wl()
        if seg.after:
            iw.wl(f'. = _{seg.after.name}SegmentEnd; /* after "{seg.after.name}" */')
        if seg.address is not None:
            iw.wl(f". = 0x{seg.address:X}; /* address 0x{seg.address:X} */")
        if seg.number is not None:
            iw.wl(f". = 0x{seg.number:02X}000000; /* number {seg.number} */")
        if seg.align:
            iw.wl(f". = ALIGN(0x{seg.align:X}); /* align 0x{seg.align:X} */")
        if frankenspec_seg.override_vram is not None:
            iw.wl(f". = 0x{frankenspec_seg.override_vram:08X}; /* OVERRIDE vram */")
        iw.wl(f"_{seg.name}SegmentStart = ABSOLUTE(.);")
        if seg.romalign:
            iw.wl(
                f".rom = ALIGN(.rom, 0x{seg.romalign:X});"
                f" /* romalign 0x{seg.romalign:X} */"
            )
        if frankenspec_seg.override_rom is not None:
            assert segment_in_rom, (seg, frankenspec_seg)
            iw.wl(f".rom = 0x{frankenspec_seg.override_rom:08X}; /* OVERRIDE rom */")
        if segment_in_rom:
            # TODO remove ABSOLUTE s when it works (pretty sure they're redundant (at least some))
            iw.wl(f"_{seg.name}SegmentRomStart = ABSOLUTE(.rom);")
        iw.w(f"..{seg.name} _{seg.name}SegmentStart")
        if segment_in_rom:
            iw.w(f" : AT(_{seg.name}SegmentRomStart) ")
        else:
            iw.wl()
            iw.wl("(NOLOAD) /* flags NOLOAD */")
            iw.w(": ")
        # iw.wl("SUBALIGN(0)")  # FIXME this looked like a good idea on paper but we also do want to align sections properly
        iw.wl("{")
        with iw.indented("  "):
            if frankenspec_seg.baseromify:
                baserom_object_p = Path(f"build/{VERSION}/baserom/{seg.name}.o")
                assert baserom_object_p.exists(), baserom_object_p
                iw.wl(f"{baserom_object_p} (*)")
            else:
                for section in (
                    SectionName.TEXT,
                    SectionName.DATA,
                    SectionName.RODATA,
                    SectionName.OVL,
                    SectionName.BSS,
                ):
                    section_in_rom = section != SectionName.BSS and segment_in_rom
                    iw.wl()
                    iw.wl(
                        f"/* {seg.name} {section.value}"
                        f" {'(in rom)' if section_in_rom else '(NOLOAD)'} */"
                    )
                    iw.wl(f"_{seg.name}Segment{section.name.capitalize()}Start = .;")
                    if section_in_rom:
                        iw.wl(
                            f"_{seg.name}Segment{section.name.capitalize()}RomStart = ABSOLUTE(.rom);"
                        )
                    with iw.indented("  "):
                        for inc in seg.includes:
                            iw.writeline(f"dot{i} = ABSOLUTE(.);")
                            i += 1
                            if inc.dataWithRodata and section == SectionName.DATA:
                                iw_original = iw
                                dataWithRodata_f = io.StringIO()
                                iw = IndentedWriter(dataWithRodata_f)
                            if inc.dataWithRodata and section == SectionName.RODATA:
                                iw.writeline("/* include_data_with_rodata */")
                                # FIXME this just assumes dataWithRodata_src is the same, eg that there is only one include_data_with_rodata include
                                # also this relies on dat being written before rodata
                                with iw.indented("  "):
                                    iw.write(dataWithRodata_src)
                            inc_sections = (
                                frankenspec_seg.frankenelf.get_sections(inc.file)
                                if frankenspec_seg.frankenelf is not None
                                else None
                            )
                            expected_inc_file = EXPECTED_P / inc.file
                            if (
                                frankenspec_seg.frankenelf is None
                                or inc_sections == pyfrankenspec.ALL_SECTIONS
                            ):
                                assert inc.file.exists(), inc.file
                                iw.wl(f"{inc.file} ({section.value})")
                            elif not inc_sections:
                                assert expected_inc_file.exists(), expected_inc_file
                                iw.wl(f"{expected_inc_file} ({section.value})")
                            else:
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
                                        "Note: build file and expected file don't both"
                                        f" have / not have a .{section.name.lower()} section:"
                                    )
                                    print(
                                        "exists" if file.exists() else "does not exist",
                                        ":",
                                        file,
                                    )
                                    print(
                                        (
                                            "exists"
                                            if other.exists()
                                            else "does not exist"
                                        ),
                                        ":",
                                        other,
                                    )
                                iw.wl(f"{inc.file}.{section.name.lower()} = .;")
                                if file.exists():
                                    iw.wl(f"{file} ({section.value})")
                                else:
                                    iw.wl(
                                        f"/* skip link section bc not exists {file} */"
                                    )
                                if other.exists():
                                    iw.wl(f"INCLUDE {other.with_suffix('.syms.txt')};")
                                else:
                                    iw.wl(
                                        f"/* skip include syms bc not exists {other} */"
                                    )
                            if inc.pad_text and section == SectionName.TEXT:
                                iw.wl(". += 0x10; /* pad_text */")
                            if inc.dataWithRodata and section == SectionName.DATA:
                                iw = iw_original
                                dataWithRodata_src = dataWithRodata_f.getvalue()
                                del dataWithRodata_f
                    iw.wl(f"_{seg.name}Segment{section.name.capitalize()}End = .;")
                    iw.wl(
                        f"_{seg.name}Segment{section.name.capitalize()}Size"
                        f" = ABSOLUTE(_{seg.name}Segment{section.name.capitalize()}End"
                        f" - _{seg.name}Segment{section.name.capitalize()}Start);"
                    )
                    if section_in_rom:
                        iw.wl(
                            ".rom = ABSOLUTE(.rom +"
                            f" _{seg.name}Segment{section.name.capitalize()}Size);"
                        )
                        iw.wl(
                            f"_{seg.name}Segment{section.name.capitalize()}RomEnd"
                            " = ABSOLUTE(.rom);"
                        )
                iw.wl()
                # for some reason the existing writes RoData instead of Rodata
                iw.wl(
                    f"_{seg.name}SegmentRoDataSize = ABSOLUTE(_{seg.name}SegmentRodataSize);"
                )
                iw.wl()
        iw.wl("}")
        iw.wl(f"_{seg.name}SegmentEnd = .;")
        if frankenspec_seg.baseromify:
            if segment_in_rom:
                iw.wl(
                    ".rom = ABSOLUTE(.rom"
                    f" + (_{seg.name}SegmentEnd - _{seg.name}SegmentStart));"
                )
        if segment_in_rom:
            iw.wl(f"_{seg.name}SegmentRomEnd = .rom;")
        # f.write(f"_{seg.name}SegmentSize = _{seg.name}SegmentEnd - _{seg.name}SegmentStart;\n")
        # f.write(f"_{seg.name}SegmentRomEnd = .rom;\n")
        iw.wl()
        iw.wl()
    iw.wl(
        """

/* pad the rom for matching */
rom_padding .rom : AT(.rom)
{
  BYTE(0); /* otherwise ld does not fill the section */
  FILL(0x00);
  . = ALIGN(0x1000);
}

"""
    )
    if VERSION != "gc-eu-mq":
        # TODO ld fails with "memory exhausted" error for gc-eu-mq with these
        iw.wl(
            """
/* mdebug sections */
.pdr              : { *(.pdr) }
.mdebug           : { *(.mdebug) }
.mdebug.abi32     : { *(.mdebug.abi32) }
/* DWARF debug sections */
/* Symbols in the DWARF debugging sections are relative to
   the beginning of the section so we begin them at 0. */
/* DWARF 1 */
.debug          0 : { *(.debug) }
.line           0 : { *(.line) }
/* GNU DWARF 1 extensions */
.debug_srcinfo  0 : { *(.debug_srcinfo) }
.debug_sfnames  0 : { *(.debug_sfnames) }
/* DWARF 1.1 and DWARF 2 */
.debug_aranges  0 : { *(.debug_aranges) }
.debug_pubnames 0 : { *(.debug_pubnames) }
/* DWARF 2 */
.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
.debug_abbrev   0 : { *(.debug_abbrev) }
.debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
.debug_frame    0 : { *(.debug_frame) }
.debug_str      0 : { *(.debug_str) }
.debug_loc      0 : { *(.debug_loc) }
.debug_macinfo  0 : { *(.debug_macinfo) }
/* SGI/MIPS DWARF 2 extensions */
.debug_weaknames 0 : { *(.debug_weaknames) }
.debug_funcnames 0 : { *(.debug_funcnames) }
.debug_typenames 0 : { *(.debug_typenames) }
.debug_varnames  0 : { *(.debug_varnames) }
/* DWARF 3 */
.debug_pubtypes 0 : { *(.debug_pubtypes) }
.debug_ranges   0 : { *(.debug_ranges) }
/* DWARF 5 */
.debug_addr     0 : { *(.debug_addr) }
.debug_line_str 0 : { *(.debug_line_str) }
.debug_loclists 0 : { *(.debug_loclists) }
.debug_macro    0 : { *(.debug_macro) }
.debug_names    0 : { *(.debug_names) }
.debug_rnglists 0 : { *(.debug_rnglists) }
.debug_str_offsets 0 : { *(.debug_str_offsets) }
.debug_sup      0 : { *(.debug_sup) }
/* gnu attributes */
.gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
"""
        )
    iw.wl(
        """

/DISCARD/ : { *(*) }

}
"""
    )
