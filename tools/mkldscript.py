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

script_assets_lines = []
for name, asset_info in assets_list.items():
    seg = asset_info["segment"]
    script_assets_lines.append(f"    assets.{name} 0x{seg << 24:08X} (OVERLAY) : " "{")
    srcs = asset_info["srcs"]
    if isinstance(srcs, str):
        srcs = [srcs]
    for src in srcs:
        assert src.endswith(".c")
        script_assets_lines.append(
            f'        KEEP(build/{f"{src.removesuffix(".c")}.o"} (.data* .rodata*))'
        )
    script_assets_lines.append("    }")
    script_assets_lines.append("")
script_assets = "\n".join(script_assets_lines)


script_assets_text = """
    assets_text.staff 0x07000000 (OVERLAY) : {
        KEEP(build/assets/text/staff_message_data_static.o (.data* .rodata*))
    }
    assets_text.fra 0x07000000 (OVERLAY) : {
        KEEP(build/assets/text/fra_message_data_static.o (.data* .rodata*))
    }
    assets_text.ger 0x07000000 (OVERLAY) : {
        KEEP(build/assets/text/ger_message_data_static.o (.data* .rodata*))
    }
    assets_text.nes 0x07000000 (OVERLAY) : {
        KEEP(build/assets/text/nes_message_data_static.o (.data* .rodata*))
    }
"""

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
        __text_start = .;
        EXCLUDE_FILE_repl *(.boot)
        . = ALIGN(16);
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
        . = ALIGN(4);
        __tdata_align = .;
        LONG (ALIGNOF(.tdata));
        . = ALIGN(8);
    }

    . = ALIGN(4);

    .ctors : {
        __CTOR_LIST__ = .;
        KEEP(EXCLUDE_FILE_repl *(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP(EXCLUDE_FILE_repl *(SORT(.ctors.*)))
        KEEP(EXCLUDE_FILE_repl *crtend.o(.ctors))
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

    .tdata : {
        __tls_base = .;
        __tdata_start = .;
        EXCLUDE_FILE_repl *(.tdata)
        EXCLUDE_FILE_repl *(.tdata.*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.td.*)
        __tdata_end = .;
    }

    .tbss : {
        __tbss_start = .;
        EXCLUDE_FILE_repl *(.tbss)
        EXCLUDE_FILE_repl *(.tbss.*)
        EXCLUDE_FILE_repl *(.gnu.linkonce.tb.*)
        . = ALIGN(8);
        __tbss_end = .;
        __tls_end = .;
    }

    /* Fix dot for TBSS Sections */
    . = .+(__tbss_end-__tbss_start);

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
        __th_tdata_copy = .;
        . = .+(__tdata_end-__tdata_start);
        . = ALIGN(8);
         __bss_end = .;
    }

    . = ALIGN(8);

"""
        + script_assets
        + script_assets_text
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
    .replace(
        "EXCLUDE_FILE_repl",
        "EXCLUDE_FILE(build/assets/* build/src/overlays/* build/src/elf_message/*)",
    )
    .replace("build/", "build/gc-eu-mq-dbg/")
)
