# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

# find which .o files in the spec are not ok by trying replacing them with expected/ .o files


from pathlib import Path
import subprocess
import functools

import colorama


VERSION = "gc-eu-mq"
OOT_DEBUG = 0


OG_SPEC_P = Path("spec")
OG_SPEC_LINES = OG_SPEC_P.read_text().splitlines()


OFILES: list[str] = []
ifdirectivestate = []
for l in OG_SPEC_LINES:
    if l.startswith("#"):
        if l.strip() == "#if OOT_DEBUG":
            ifdirectivestate.append(1)
        elif l.strip() == "#if !OOT_DEBUG":
            ifdirectivestate.append(0)
        elif l.strip() == "#ifdef COMPILER_GCC":
            ifdirectivestate.append(2)
        elif l.strip() == "#ifndef NON_MATCHING":
            ifdirectivestate.append(None)
        elif l.strip() == "#endif":
            ifdirectivestate.pop()
        else:
            assert False, ("unhandled # directive", l)
    if not all(s is None or s == OOT_DEBUG for s in ifdirectivestate):
        continue
    if l.strip().startswith("include"):
        parts = l.split('"')
        assert len(parts) == 3
        path = parts[1]
        OFILES.append(path)

assert len(OFILES) == len(set(OFILES))

OFILES_expected_doesnt_exist = [
    ofile
    for ofile in OFILES
    if not Path(
        "expected/" + ofile.replace("$(BUILD_DIR)", f"build/{VERSION}")
    ).exists()
]

if OFILES_expected_doesnt_exist:
    print(colorama.Fore.RED, "OFILES_expected_doesnt_exist =", colorama.Fore.RESET)
    print("\n".join(OFILES_expected_doesnt_exist))
    print(colorama.Fore.CYAN, "======", colorama.Fore.RESET)
    for ofile in OFILES_expected_doesnt_exist:
        OFILES.remove(ofile)

# because the disassembly doesnt provide the correct symbols
OFILES.remove("$(BUILD_DIR)/src/libultra/os/parameters.o")

# because it gets disassembled as text referencing a non existing function
OFILES.remove("$(BUILD_DIR)/data/rsp_boot.text.o")

# because the disassembly includes cutscene data while the source doesn't (split into several files)
# TODO this may be better fixed by fixing the file ranges in the disasm info file csv
OFILES.remove("$(BUILD_DIR)/src/overlays/actors/ovl_Bg_Toki_Swd/z_bg_toki_swd.o")
OFILES.remove("$(BUILD_DIR)/src/overlays/actors/ovl_Bg_Treemouth/z_bg_treemouth.o")
# OFILES.remove("$(BUILD_DIR)/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo.o") # baserom'd
# OFILES.remove("$(BUILD_DIR)/src/overlays/actors/ovl_En_Okarina_Tag/z_en_okarina_tag.o") # baserom'd
OFILES.remove("$(BUILD_DIR)/src/overlays/actors/ovl_En_Zl1/z_en_zl1.o")

DISASM_SYMS_P = Path("disasm_syms.txt")
if 0:
    if not DISASM_SYMS_P.exists():
        with DISASM_SYMS_P.open("w") as f:
            for addr in [
                0x0000_FFFF,
                0x0001_0001,
                0x0001_0002,
                0x0100_0000,
                0x0200_0000,
                0x0700_0000,
                0x0C00_0000,
                0x0E00_0000,
                0x0F00_0000,
            ]:
                f.write(f"D_{addr:08X}_unknown = 0x{addr:X};\n")
            for seg in [0, 1, 2, 3, 4, 5, 6, 0xC]:
                addr0 = seg << 24
                for off in range(0, 0x60000, 4):
                    addr = addr0 + off
                    f.write(f"D_{addr:08X}_unknown = 0x{addr:X};\n")


def edit_spec_with_expected(ofiles_use_expected: set[str]):
    edited = []
    for l in OG_SPEC_LINES:
        if l.strip().startswith("include"):
            parts = l.split('"')
            assert len(parts) == 3
            path = parts[1]
            if path in ofiles_use_expected:
                l = '"'.join((parts[0], f"expected/{path}", parts[2]))
        edited.append(l)
    return "\n".join(edited)


def make_with_spec(spec_p: Path):
    make_cmd = [
        "make",
        "-j32",
        # "HAVE_DISASM_RULES=0",
        f"VERSION={VERSION}",
        f"SPEC={spec_p}",
        "COMPARE=0",
        # otherwise make tries to update the expected/.../*.o files which depend on this
        f"--assume-old=expected/build/{VERSION}/.disasm",
        f"--assume-new={spec_p}",
    ]
    # print(make_cmd)
    p = subprocess.Popen(make_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print(colorama.Fore.CYAN, "stdout", colorama.Fore.RESET)
        print(stdout.decode())
        print(colorama.Fore.CYAN, "stderr", colorama.Fore.RESET)
        print(stderr.decode())
        print(colorama.Fore.RED, "error during make", colorama.Fore.RESET)
        print(" ".join(make_cmd))
        print(colorama.Fore.CYAN, "======", colorama.Fore.RESET)
        undefined_syms: set[str] = set()
        for l in stderr.decode().splitlines():
            i = l.find("undefined reference to")
            if i >= 0:
                sym = l[i + len("undefined reference to `") : -1]
                undefined_syms.add(sym)
        print(f"{undefined_syms=}")
        with DISASM_SYMS_P.open("a") as f:
            for sym in undefined_syms:
                if sym.startswith("D_") and sym.endswith("_unknown"):
                    sym_hex = (
                        sym.removeprefix("D_")
                        .removeprefix("FLT_")
                        .removesuffix("_unknown")
                    )
                    sym_val = int(sym_hex, 16)
                    f.write(f"{sym} = 0x{sym_val:08X};\n")
        print(colorama.Fore.CYAN, "======", colorama.Fore.RESET)
        raise Exception(p.returncode)


def compare_checksum():
    p = subprocess.run(f"md5sum -c baseroms/{VERSION}/checksum.md5".split())
    return p.returncode == 0


TMP_SPEC = Path("spec_tmp_bisect_unok_ofiles_py.txt")


@functools.cache
def is_ok_with_expected_cached(ofiles_use_expected: frozenset[str]):
    TMP_SPEC.write_text(edit_spec_with_expected(ofiles_use_expected))
    make_with_spec(TMP_SPEC)
    return compare_checksum()


def is_ok_with_expected(ofiles_use_expected: set[str]):
    return is_ok_with_expected_cached(frozenset(ofiles_use_expected))


def is_ok_with_built(ofiles_use_built: set[str]):
    return is_ok_with_expected(set(OFILES) - ofiles_use_built)


def main():
    print(
        "Checking the rom with all object files being built"
        " (not expected/) is not OK..."
    )
    if is_ok_with_expected(set()):
        print(colorama.Fore.GREEN, "rom is OK already.", colorama.Fore.RESET)
        return

    print("Checking a rom with all object files using expected/ is OK...")
    if not is_ok_with_expected(set(OFILES)):
        print(colorama.Fore.RED, "full-expected is not OK!", colorama.Fore.RESET)
        return

    def try_ofiles_as_ok(
        ok_ofiles: set[str], unknown_ofiles: list[str], pre: str = ">"
    ):
        if not unknown_ofiles:
            return ok_ofiles, set()

        print(
            colorama.Fore.CYAN,
            pre,
            "Classifying",
            len(unknown_ofiles),
            ".o files...",
            colorama.Fore.RESET,
        )

        if is_ok_with_built(ok_ofiles | set(unknown_ofiles)):
            print("All", len(unknown_ofiles), "are OK")
            return ok_ofiles | set(unknown_ofiles), set()

        if len(unknown_ofiles) == 1:
            print(colorama.Fore.RED, "Not OK:", unknown_ofiles[0], colorama.Fore.RESET)
            return ok_ofiles, set(unknown_ofiles)

        print("At least one of the", len(unknown_ofiles), "is not OK")

        assert len(unknown_ofiles) >= 2
        half = len(unknown_ofiles) // 2
        unk_ofiles_1 = unknown_ofiles[half:]
        unk_ofiles_2 = unknown_ofiles[:half]
        assert unk_ofiles_1
        assert unk_ofiles_2

        ok_ofiles_1, notok_ofiles_1 = try_ofiles_as_ok(
            ok_ofiles, unk_ofiles_1, f"{pre}1>"
        )
        ok_ofiles_2, notok_ofiles_2 = try_ofiles_as_ok(
            ok_ofiles, unk_ofiles_2, f"{pre}2>"
        )

        notok_ofiles = notok_ofiles_1 | notok_ofiles_2
        assert notok_ofiles, (
            "found that at least one of",
            unknown_ofiles,
            "is not OK, but after splitting into",
            unk_ofiles_1,
            "and",
            unk_ofiles_2,
            "neither of these sub-groups was found to have any not-OK",
        )

        return ok_ofiles_1 | ok_ofiles_2, notok_ofiles

    print("Bisecting to find which object files are not OK...")
    res_ok_ofiles, res_notok_ofiles = try_ofiles_as_ok(set(), list(OFILES))
    assert len(res_ok_ofiles) + len(res_notok_ofiles) == len(OFILES)
    print("OK:", sorted(res_ok_ofiles))
    print(colorama.Fore.RED, "Not OK:", sorted(res_notok_ofiles), colorama.Fore.RESET)
    print("OK:", len(res_ok_ofiles), " -  Not OK:", len(res_notok_ofiles))

    make_with_spec(OG_SPEC_P)
    if 0:
        # "fixed" by just running make again with the normal spec
        print(
            "running make will fail because of dep files now referencing"
            " expected/.../*.o which triggers a disasm of gc-eu-mq-dbg which is"
            " nonsensical, you probably want to delete the deps files:"
        )
        print("find build/gc-eu-mq-dbg/ -name '*.d' -delete")

    TMP_SPEC.unlink(missing_ok=True)


if __name__ == "__main__":
    main()
