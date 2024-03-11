# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

import subprocess
from pathlib import Path
import hashlib

import colorama

import pyspec
import pyfrankenspec
from pyfrankenspec import SectionName
import gen_quitetheldscript


def linkrom():
    for cmd in (
        [
            "mips-linux-gnu-ld",
            "-T",
            "build/gc-eu-mq/quitetheldscript.txt",
            "-T",
            "build/gc-eu-mq/undefined_syms.txt",
            "--no-check-sections",
            "--accept-unknown-input-arch",
            "--emit-relocs",
            "-Map",
            "build/gc-eu-mq/oot-gc-eu-mq.map",
            "-o",
            "build/gc-eu-mq/oot-gc-eu-mq.elf",
        ],
        [
            "mips-linux-gnu-objcopy",
            "-O",
            "binary",
            "build/gc-eu-mq/oot-gc-eu-mq.elf",
            "build/gc-eu-mq/oot-gc-eu-mq.z64",
        ],
        [
            ".venv/bin/python3",
            "-m",
            "ipl3checksum",
            "sum",
            "--cic",
            "6105",
            "--update",
            "build/gc-eu-mq/oot-gc-eu-mq.z64",
        ],
    ):
        p = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        stdout, stderr = p.communicate()
        if p.returncode:
            e = subprocess.CalledProcessError(p.returncode, cmd, stdout, stderr)
            e.p = p
            raise e


def test(frankenspec: pyfrankenspec.FrankenSpec, verbose_cpe=True):
    with open("build/gc-eu-mq/quitetheldscript.txt", "w") as f:
        gen_quitetheldscript.write_ldscript(f, "gc-eu-mq", SPEC, frankenspec)
    try:
        linkrom()
    except subprocess.CalledProcessError as e:
        if verbose_cpe:
            print()
            print("linkrom error")
            print(" ".join(e.cmd))
            print(e.returncode)
            print()
            print(e.stdout.decode("utf-8") if e.stdout else "(no stdout)")
            print()
            print(e.stderr.decode("utf-8") if e.stderr else "(no stderr)")
            print()
        raise
    return (
        hashlib.md5(Path("build/gc-eu-mq/oot-gc-eu-mq.z64").read_bytes()).hexdigest()
        == "4920520254b9aab86de57b42ab477dbb"
    )


def okfind_code(frankenspec: pyfrankenspec.FrankenSpec):
    segment_code = frankenspec.segments["code"]
    for section in (
        # SectionName.TEXT,
        # SectionName.DATA,
        # SectionName.RODATA,
        # SectionName.OVL,
        SectionName.BSS,
    ):
        print(colorama.Fore.CYAN, section, colorama.Fore.RESET)
        for inc_p, inc_sections in segment_code.frankenelf.perfile.items():
            if section in inc_sections:
                continue
            inc_sections.add(section)
            try:
                is_ok = test(frankenspec, verbose_cpe=False)
            except subprocess.CalledProcessError as e:
                if 0:
                    print()
                    print("test error in okfind_code", section, inc_p)
                    print()
                    raise
                is_ok = False
                print(
                    f"{colorama.Fore.RED}ERROR{colorama.Fore.RESET}",
                    section.name.lower(),
                    inc_p,
                )
                print(e.stderr.decode("utf-8"))
                print()
            if is_ok:
                print(
                    f"{colorama.Fore.GREEN}OK",
                    section.name.lower(),
                    inc_p,
                    colorama.Fore.RESET,
                )
            else:
                inc_sections.remove(section)
                print(
                    f"{colorama.Fore.YELLOW}not ok{colorama.Fore.RESET}",
                    section.name.lower(),
                    inc_p,
                )


def main():
    frankenspec = pyfrankenspec.parse_frankenspec(Path("frankenspec.json"))

    assert test(frankenspec), "base frankenspec not even OK..."

    okfind_code(frankenspec)


SPEC = pyspec.parse_spec_p(Path("build/gc-eu-mq/spec"))
main()
