# SPDX-FileCopyrightText: 2024 Dragorn421
# SPDX-License-Identifier: CC0-1.0

import argparse
from pathlib import Path
import tomllib

parser = argparse.ArgumentParser()
parser.add_argument("ldscript", type=Path)
args = parser.parse_args()

ldscript_p: Path = args.ldscript

with Path("assets_list.toml").open("rb") as f:
    assets_list = tomllib.load(f)

assets = {_name: _info["segment"] for _name, _info in assets_list.items()}

dlls = []
for _dllrootdir_p in (
    Path("src/overlays/actors"),
    Path("src/overlays/effects"),
    Path("src/overlays/gamestates"),
    Path("src/overlays/misc"),
):
    dlls.extend(
        sorted(
            _dlldir_p.relative_to(Path("src/overlays"))
            for _dlldir_p in _dllrootdir_p.iterdir()
        )
    )

script_assets = """
    _offset = ALIGN(8);
""" + "".join(
    f"""
    assets.{_name} {_seg << 24:#08X} (OVERLAY) : AT(_offset) {{
        KEEP(build/assets/{f"{_name}.o" if _name.startswith("scenes/") else f"{_name}/*"} (.data* .rodata*))
    }}
    _offset += SIZEOF(assets.{_name});
    _offset = ALIGN(_offset, 8);
"""
    for _name, _seg in assets.items()
)

script_dlls = """
    . = 0x80800000;
""" + "".join(
    f"""
    dlls.{_name} (OVERLAY) : {{
        KEEP(build/src/overlays/{_name}/dll.o (dll.code))
    }}
    dlls.bss.{_name} (NOLOAD) : {{
        KEEP(build/src/overlays/{_name}/dll.o (dll.code.bss))
    }} :ptnul
"""
    for _name in dlls
)

script_dlls_relocs = "".join(
    f"""
    dlls.rel.{_name} (OVERLAY) : {{
        KEEP(build/src/overlays/{_name}/dll.o (dll.rel))
    }}
"""
    for _name in dlls
)

ldscript_p.write_text(
    (
        """
/* based on libdragon's n64.ld */

OUTPUT_FORMAT ("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH (mips)
EXTERN (_start)
ENTRY (_start)

PHDRS
{
    irq PT_LOAD AT ( 0x80000000 );
    main PT_LOAD AT ( 0x80000400 );
    ptnul PT_NULL;
}

SECTIONS {
    .intvectors 0x80000000 : {
        . = ALIGN(32);
        KEEP(*(.intvectors))
        __intvectors_end = .;
    } :irq

    .text 0x80000400 : {
        EXCLUDE_FILE_repl *(.boot)
        . = ALIGN(16);
        __text_start = .;
        EXCLUDE_FILE_repl *(.text)
        EXCLUDE_FILE_repl *(.text.*)
        EXCLUDE_FILE_repl *(.init)
        EXCLUDE_FILE_repl *(.fini)
        EXCLUDE_FILE_repl *(.gnu.linkonce.t.*)
        . = ALIGN(16);
        __text_end  = .;
    } :main

   .eh_frame_hdr : { EXCLUDE_FILE_repl *(.eh_frame_hdr) }
   .eh_frame : { 
		__EH_FRAME_BEGIN__ = .;
		KEEP (EXCLUDE_FILE_repl *(.eh_frame)) 
	}
   .gcc_except_table : { EXCLUDE_FILE_repl *(.gcc_except_table*) }
   .jcr : { KEEP (EXCLUDE_FILE_repl *(.jcr)) }

    .rodata : {
        EXCLUDE_FILE_repl *(.rdata)
        EXCLUDE_FILE_repl *(.rodata)
        EXCLUDE_FILE_repl *(.rodata.*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.r.*)
        . = ALIGN(8);
    }

    . = ALIGN(4);

    .ctors : {
        __CTOR_LIST__ = .;
        KEEP(EXCLUDE_FILE_repl *(.ctors))
        __CTOR_END__ = .;
    }

    . = ALIGN(8);

    .data : {
        __data_start = .;
        EXCLUDE_FILE_repl *(.data)
        EXCLUDE_FILE_repl *(.data.*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.d.*)
        . = ALIGN(8);
    }

    .sdata : {
        _gp = . + 0x8000;
        EXCLUDE_FILE_repl *(.sdata)
        EXCLUDE_FILE_repl *(.sdata.*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.s.*)
        . = ALIGN(8);
    }

    .lit8 : {
        EXCLUDE_FILE_repl *(.lit8)
        . = ALIGN(8);
    }
    .lit4 : {
        EXCLUDE_FILE_repl *(.lit4)
        . = ALIGN(8);
    }

    . = ALIGN(8);
    __data_end = .;

    . = ALIGN(8);
    __rom_end = .;

    .sbss (NOLOAD) : {
         __bss_start = .;
        EXCLUDE_FILE_repl *(.sbss)
        EXCLUDE_FILE_repl *(.sbss.*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.sb.*)
        EXCLUDE_FILE_repl *(.scommon)
        EXCLUDE_FILE_repl *(.scommon.*)
    }

    . = ALIGN(8);
    .bss (NOLOAD) : {
        EXCLUDE_FILE_repl *(.bss)
        EXCLUDE_FILE_repl *(.bss*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.b.*)
        EXCLUDE_FILE_repl *(COMMON)
        . = ALIGN(8);
         __bss_end = .;
    }

    . = ALIGN(8);

"""
        + script_assets
        + script_dlls
        + script_dlls_relocs
        + """

    /DISCARD/ : {
"""
        "        build/assets/* (*)\n"
        # + "".join(f"        build/assets/{_name}/* (*)\n" for _name in assets.keys())
        + """
    }
}
"""
    )
    .replace("EXCLUDE_FILE_repl", "EXCLUDE_FILE(build/assets/* build/src/overlays/*)")
    .replace("build/", "build/gc-eu-mq-dbg/")
)
