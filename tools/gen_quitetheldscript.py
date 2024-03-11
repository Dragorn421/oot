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
            if lines[0] != "" or len(lines) != 1:
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
    iw_out = io.StringIO()
    iw = IndentedWriter(iw_out)
    expected = ""
    iw.writeline("abc")
    expected += "abc\n"
    iw.write("de")
    iw.writeline("f")
    expected += "def\n"
    with iw.indented("  "):
        iw.writeline("ghi")
        expected += "  ghi\n"
        with iw.indented("  "):
            iw.write("jk")
        iw.writeline("l")
        expected += "    jkl\n"
        iw.writeline("mno")
        expected += "  mno\n"
        with iw.indented("  "):
            iw.writeline("AB")
            expected += "    AB\n"
            iw.write("")
        iw.writeline("CD")
        expected += "  CD\n"
    iw.write("p")
    with iw.indented("  "):
        iw.writeline("qr")
        expected += "pqr\n"
        iw.write("s")
        with iw.indented("  "):
            iw.writeline("tu")
            expected += "  stu\n"
    iw.writeline("vwx")
    expected += "vwx\n"
    iw.writeline("a\nb\nc")
    expected += "a\nb\nc\n"
    with iw.indented("  "):
        iw.writeline("a\nb\nc")
        expected += "  a\n  b\n  c\n"
    if iw_out.getvalue() == expected:
        print("test_indented_writer ok")
        print(iw_out.getvalue(), end="")
    else:
        print("test_indented_writer fail")
        iw_out_lines = iw_out.getvalue().splitlines(keepends=True)
        expected_lines = expected.splitlines(keepends=True)
        n = max(len(iw_out_lines), len(expected_lines))
        iw_out_lines += [None] * (n - len(iw_out_lines))
        expected_lines += [None] * (n - len(expected_lines))
        print(f"msg {'iw_out':15} expected")
        print(
            "".join(
                (
                    "{:3} ".format("" if iw_out_l == expected_l else "!")
                    + f"{iw_out_l!r:15} {expected_l!r}"
                    + "\n"
                )
                for iw_out_l, expected_l in zip(iw_out_lines, expected_lines)
            )
        )
        exit(1)


if 0:
    test_indented_writer()
    exit(0)


def write_ldscript(
    f: io.TextIOBase,
    oot_version: str,
    spec: pyspec.Spec,
    frankenspec: pyfrankenspec.FrankenSpec,
):
    iw = IndentedWriter(f)
    iw.wl('LD_FEATURE("SANE_EXPR")')
    iw.wl()
    iw.wl("SECTIONS {")
    iw.wl(".rom = 0;")
    iw.wl()
    extern_syms_includes: list[Path] = []
    for seg in spec.segments:
        frankenspec_seg = frankenspec.get(seg.name)
        segment_in_rom = not (seg.flags & pyspec.SpecSegmentFlag.NOLOAD)
        iw.wl()
        iw.wl(f"/* {seg.name} " f"{'(in rom)' if segment_in_rom else '(NOLOAD)'} */")
        iw.wl()
        with iw.indented("  "):
            if seg.after:
                iw.wl(
                    f'. = _{seg.after.name}SegmentEnd; /* after "{seg.after.name}" */'
                )
            if seg.address is not None:
                iw.wl(f". = 0x{seg.address:X}; /* address 0x{seg.address:X} */")
            if seg.number is not None:
                iw.wl(f". = 0x{seg.number:02X}000000; /* number {seg.number} */")
            if seg.align:
                iw.wl(f". = ALIGN(0x{seg.align:X}); /* align 0x{seg.align:X} */")
            if frankenspec_seg.override_vram is not None:
                iw.wl(f". = 0x{frankenspec_seg.override_vram:08X}; /* OVERRIDE vram */")
            iw.wl(f"_{seg.name}SegmentStart = .;")
            if seg.romalign:
                iw.wl(
                    f".rom = ALIGN(.rom, 0x{seg.romalign:X});"
                    f" /* romalign 0x{seg.romalign:X} */"
                )
            if frankenspec_seg.override_rom is not None:
                assert segment_in_rom, (seg, frankenspec_seg)
                iw.wl(
                    f".rom = 0x{frankenspec_seg.override_rom:08X}; /* OVERRIDE rom */"
                )
            if segment_in_rom:
                iw.wl(f"_{seg.name}SegmentRomStart = .rom;")
            iw.wl()
            if frankenspec_seg.baseromify:
                assert segment_in_rom
                iw.wl("/* baseromify */")
                baserom_object_p = Path(f"build/{oot_version}/baserom/{seg.name}.o")
                assert baserom_object_p.exists(), baserom_object_p
                with iw.indented("  "):
                    iw.w(f"..{seg.name} : AT(_{seg.name}SegmentRomStart) ")
                    iw.wl("{")
                    with iw.indented("  "):
                        iw.wl(f"{baserom_object_p} (*)")
                    iw.wl("}")
                    for section in (
                        SectionName.TEXT,
                        SectionName.DATA,
                        SectionName.RODATA,
                        SectionName.OVL,
                        SectionName.BSS,
                    ):
                        has_written_header_for_section_syms = False
                        for inc in seg.includes:
                            expected_inc_file = EXPECTED_P / inc.file
                            if expected_inc_file.exists():
                                file = expected_inc_file.with_suffix(
                                    f".{section.name.lower()}.o"
                                )
                                if file.exists():
                                    if not has_written_header_for_section_syms:
                                        has_written_header_for_section_syms = True
                                        iw.wl(
                                            f"/* symbols from expected {section.name.lower()} */"
                                        )
                                    with iw.indented("  "):
                                        iw.w('INCLUDE "')
                                        iw.w(str(file.with_suffix(".syms.txt")))
                                        iw.wl('"')
                    iw.wl()
                    iw.wl(
                        ".rom = ."
                        f" - _{seg.name}SegmentStart"
                        f" + _{seg.name}SegmentRomStart;"
                    )
            else:
                for section in (
                    SectionName.TEXT,
                    SectionName.DATA,
                    SectionName.RODATA,
                    SectionName.OVL,
                    SectionName.BSS,
                ):
                    section_in_rom = section != SectionName.BSS and segment_in_rom
                    iw.wl(
                        f"/* {seg.name} {section.value}"
                        f" {'(in rom)' if section_in_rom else '(NOLOAD)'} */"
                    )
                    with iw.indented("  "):
                        iw.wl(
                            f"_{seg.name}Segment{section.name.capitalize()}Start"
                            " = .;"
                        )
                        if section_in_rom:
                            iw.wl(
                                f"_{seg.name}Segment{section.name.capitalize()}RomStart"
                                " = .rom;"
                            )
                        iw.wl()
                        for inc in seg.includes:
                            if inc.dataWithRodata and section == SectionName.DATA:
                                iw_original = iw
                                dataWithRodata_f = io.StringIO()
                                iw = IndentedWriter(dataWithRodata_f)
                            inc_sections = (
                                frankenspec_seg.frankenelf.get_sections(inc.file)
                                if frankenspec_seg.frankenelf is not None
                                else None
                            )
                            expected_inc_file = EXPECTED_P / inc.file
                            input_file = None
                            post_section_script = io.StringIO()
                            if (
                                frankenspec_seg.frankenelf is None
                                or inc_sections == pyfrankenspec.ALL_SECTIONS
                            ):
                                iw.wl(f"/* {inc.file} (all built) */")
                                assert inc.file.exists(), inc.file
                                input_file = inc.file
                            elif not inc_sections:
                                iw.wl(f"/* {inc.file} (all expected) */")
                                assert expected_inc_file.exists(), expected_inc_file
                                input_file = expected_inc_file
                                input_file_extern_syms_include = (
                                    expected_inc_file.with_suffix(
                                        f".{section.name.lower()}.extern_syms.txt"
                                    )
                                )
                                if input_file_extern_syms_include.exists():
                                    extern_syms_includes.append(
                                        input_file_extern_syms_include
                                    )
                            else:
                                # mix and match
                                # individually-assembled-disassembled-sections
                                # and
                                # frankenelf'd build objects
                                if section in inc_sections:
                                    iw.wl(
                                        f"/* {inc.file} (built {section.name.lower()}) */"
                                    )
                                    file = inc.file
                                    other = expected_inc_file
                                else:
                                    iw.wl(
                                        f"/* {inc.file} (expected {section.name.lower()}) */"
                                    )
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
                                if file.exists():
                                    input_file = file
                                    if input_file.is_relative_to(EXPECTED_P):
                                        extern_syms_includes.append(
                                            input_file.with_suffix(f".extern_syms.txt")
                                        )
                                else:
                                    post_section_script.write(
                                        f"/* skip link section bc not exists {file} */\n"
                                    )

                                if other.exists():
                                    # INCLUDE with the PROVIDE symbols must come after
                                    # the symbols in the intended input are loaded,
                                    # otherwise the PROVIDE d definitions are kept even
                                    # if a section does define them later.
                                    post_section_script.write('INCLUDE "')
                                    post_section_script.write(
                                        str(other.with_suffix(".syms.txt"))
                                    )
                                    post_section_script.write('"\n')
                                else:
                                    post_section_script.write(
                                        f"/* skip include syms bc not exists {other} */\n"
                                    )

                            with iw.indented("  "):
                                if inc.dataWithRodata and section == SectionName.RODATA:
                                    iw.wl("/* include_data_with_rodata */")
                                    # FIXME this just assumes dataWithRodata_src is the same,
                                    # eg that there is only one include_data_with_rodata include
                                    # also this relies on dat being written before rodata
                                    with iw.indented("  "):
                                        iw.w(dataWithRodata_src)
                                if input_file:
                                    iw.w(
                                        f"..{seg.name}.{inc.file}.{section.name.lower()}"
                                    )
                                    if section_in_rom:
                                        iw.w(
                                            ' : AT("'
                                            f"_{seg.name}_{inc.file}.{section.name.lower()}_RomStart"
                                            '") '
                                        )
                                    else:
                                        if not segment_in_rom:
                                            iw.wl()
                                            iw.wl("(NOLOAD) /* flags NOLOAD */")
                                        else:
                                            # bss section
                                            assert section == SectionName.BSS
                                            # no need to mark the output section NOLOAD explicitly,
                                            # it will inherit being NOBITS from the input bss section
                                            iw.w(" ")
                                        iw.w(": ")
                                    iw.wl("{")
                                    with iw.indented("  "):
                                        iw.wl(f"{inc.file}.{section.name.lower()} = .;")
                                        if section_in_rom:
                                            iw.wl(
                                                ".rom = ABSOLUTE(."
                                                f" - _{seg.name}SegmentStart"
                                                f" + _{seg.name}SegmentRomStart);"
                                            )
                                            iw.wl(
                                                f"_{seg.name}_{inc.file}.{section.name.lower()}_RomStart"
                                                " = ABSOLUTE(.rom);"
                                            )
                                        iw.wl(f"{input_file} ({section.value})")
                                    iw.wl("}")
                                iw.w(post_section_script.getvalue())
                                if inc.pad_text and section == SectionName.TEXT:
                                    iw.wl(". += 0x10; /* pad_text */")
                            if inc.dataWithRodata and section == SectionName.DATA:
                                iw = iw_original
                                dataWithRodata_src = dataWithRodata_f.getvalue()
                                del dataWithRodata_f
                        iw.wl()
                        if section_in_rom:
                            iw.wl(
                                ".rom = ."
                                f" - _{seg.name}SegmentStart"
                                f" + _{seg.name}SegmentRomStart;"
                            )
                        iw.wl(f"_{seg.name}Segment{section.name.capitalize()}End = .;")
                        iw.wl(
                            f"_{seg.name}Segment{section.name.capitalize()}Size"
                            f" = _{seg.name}Segment{section.name.capitalize()}End"
                            f" - _{seg.name}Segment{section.name.capitalize()}Start;"
                        )
                        if section_in_rom:
                            iw.wl(
                                f"_{seg.name}Segment{section.name.capitalize()}RomEnd"
                                " = .rom;"
                            )
                    iw.wl()
                # for some reason the existing writes RoData instead of Rodata
                iw.wl(
                    f"_{seg.name}SegmentRoDataSize = _{seg.name}SegmentRodataSize;"
                    " /* backwards compatibility */"
                )
            iw.wl()
            iw.wl(f"_{seg.name}SegmentEnd = .;")
            if segment_in_rom:
                iw.wl(f"_{seg.name}SegmentRomEnd = .rom;")
            iw.wl()
            iw.wl()
    iw.wl("/* Extern symbols for expected sections */")
    with iw.indented("  "):
        for p in extern_syms_includes:
            iw.w(f'INCLUDE "{p}"\n')
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
    if oot_version != "gc-eu-mq":
        # TODO ld fails with "memory exhausted" error for gc-eu-mq with these
        # or at least it used to. now it segfaults unless .mdebug is not linked, frankenelf should probably drop mdebug
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


def main():
    spec_p = Path(sys.argv[1])
    ldscript_p = Path(sys.argv[2])
    oot_version = sys.argv[3]
    assert oot_version in {"gc-eu-mq", "gc-eu-mq-dbg"}

    spec = pyspec.parse_spec_p(spec_p)

    if oot_version == "gc-eu-mq":
        frankenspec = pyfrankenspec.parse_frankenspec(Path("frankenspec.json"))
    else:
        frankenspec = pyfrankenspec.FrankenSpec()

    if 0:
        with open(Path(__file__).with_suffix(".dumplog.txt"), "w") as f:
            kwargs = dict(width=90, stream=f)
            f.write("spec =\n")
            pprint(spec, **kwargs)
            f.write("\n" * 10)
            f.write("frankenspec =\n")
            pprint(frankenspec, **kwargs)
            f.write("\n" * 10)

    with ldscript_p.open("w") as f:
        write_ldscript(f, oot_version, spec, frankenspec)


if __name__ == "__main__":
    main()
