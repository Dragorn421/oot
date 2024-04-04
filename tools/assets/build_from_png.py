#!/usr/bin/env python3
from __future__ import annotations

import sys
from pathlib import Path


sys.path.insert(0, "/home/dragorn421/Documents/n64texconv/")
import n64texconv

from png2raw import png2raw


VERBOSE = False


def png_to_data(file):
    with png2raw.Instance(file) as inst:
        width, height = inst.get_dimensions()
        if inst.is_paletted():
            palette_data = inst.get_palette_rgba32()
            image_data = inst.read_palette_indices()

            row_len = width
        else:
            palette_data = None
            image_data = inst.read_to_rgba32()

            row_len = width * 4

    image_data = image_data
    rows = [image_data[row * row_len :][:row_len] for row in range(height)]

    png_data = width, height, rows

    if palette_data is None:
        palette_vals = None
    else:
        palette_vals = []
        for i in range(0, len(palette_data), 4):
            # must be a tuple for n64texconv to work
            palette_vals.append(tuple(palette_data[i : i + 4]))

    if VERBOSE:
        print("png_data:", len(png_data), png_data)
        print(
            "palette:",
            *((len(palette_vals),) if palette_vals is not None else ()),
            palette_vals,
        )

    return png_data, palette_vals


n64texconv.png_to_data = png_to_data

from n64 import G_IM_FMT, G_IM_SIZ


def main():
    png_path = Path(sys.argv[1])
    out_bin_path = Path(sys.argv[2])

    suffixes = png_path.suffixes
    assert len(suffixes) >= 2
    assert suffixes[-1] == ".png"
    suffixes.pop()
    if suffixes[-1] in {".u64", ".u32"}:
        suffixes.pop()
    assert len(suffixes) > 0
    if suffixes[-1].startswith(".tlut_"):
        tlut_info = suffixes.pop().removeprefix(".")
    else:
        tlut_info = None
    assert len(suffixes) > 0
    fmtsiz_str = suffixes[-1].removeprefix(".")

    fmt, siz = None, None
    for candidate_fmt in G_IM_FMT:
        for candidate_siz in G_IM_SIZ:
            candidate_fmtsiz_str = f"{candidate_fmt.name.lower()}{candidate_siz.bpp}"
            if candidate_fmtsiz_str == fmtsiz_str:
                fmt = candidate_fmt
                siz = candidate_siz

    assert fmt is not None and siz is not None, fmtsiz_str

    if fmt != G_IM_FMT.CI:
        tex = n64texconv.Image.from_png(png_path, fmt=fmt.i, siz=siz.i)
        tex_bin = tex.to_bin()
        # print(len(tex_bin), tex_bin[:0x10], tex_bin[-0x10:], file=sys.stderr)
        # sys.stdout.buffer.write(tex_bin) # for some reason the *string* "None." is also written to stdout???
        out_bin_path.write_bytes(tex_bin)
    else:
        # TODO probably move tlut_info and overall tex file suffix construction/parsing to its own library

        if tlut_info is None:
            tlut_elem_type = "u64"
            tlut_out_bin_path_base_str = str(out_bin_path)
            tlut_out_bin_path_base_str = tlut_out_bin_path_base_str.removesuffix(".bin")
            if tlut_out_bin_path_base_str.endswith(".u64"):
                tlut_out_bin_path_base_str = tlut_out_bin_path_base_str.removesuffix(
                    ".u64"
                )
            all_pngs_using_tlut = [png_path]
        else:
            tlut_elem_type = "u64"
            if tlut_info.endswith("_u64"):
                tlut_elem_type = "u64"
                tlut_name = tlut_info.removeprefix("tlut_").removesuffix("_u64")
            elif tlut_info.endswith("_u32"):
                tlut_elem_type = "u32"
                tlut_name = tlut_info.removeprefix("tlut_").removesuffix("_u32")
            else:
                tlut_name = tlut_info.removeprefix("tlut_")
            tlut_out_bin_path_base_str = str(out_bin_path.parent / tlut_name)

            # TODO this is far from perfect.
            #  what if a tlut_name is included in another
            #  what if not in the same folder (just don't support that)
            #  does the same png get built several times
            all_pngs_using_tlut = list(png_path.parent.glob(f"*.tlut_{tlut_name}*.png"))
            assert png_path in all_pngs_using_tlut
        tlut_out_bin_path = Path(
            f"{tlut_out_bin_path_base_str}.tlut.rgba16.{tlut_elem_type}.bin"
        )

        if VERBOSE:
            print(all_pngs_using_tlut)

        tex = n64texconv.Image.from_png(png_path, fmt=fmt.i, siz=siz.i)
        tex_bin = tex.to_bin()
        tlut_bin = tex.pal.to_bin()
        out_bin_path.write_bytes(tex_bin)
        tlut_out_bin_path.write_bytes(tlut_bin)

        import subprocess

        # HACK since the makefile doesn't know the tlut file should be built (bin2c'd), build it here
        cmd = [
            "tools/assets/bin2c/bin2c",
            tlut_elem_type,
        ]
        with tlut_out_bin_path.open("rb") as fin:
            with tlut_out_bin_path.with_suffix(".inc.c").open("w") as fout:
                subprocess.check_call(cmd, stdin=fin, stdout=fout)


if __name__ == "__main__":
    main()
