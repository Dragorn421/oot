from __future__ import annotations

from pathlib import Path
from xml.etree import ElementTree
from typing import Iterable

import pyspec

spec = pyspec.parse_spec_p(Path("../../oot/build/gc-eu-mq-dbg/spec"))

assets_segments = [
    seg
    for seg in spec.segments
    if (
        (
            any("assets" in inc.file.parts for inc in seg.includes)
            and seg.name not in "code"
        )
        or seg.name
        in {
            "elf_message_field",
            "elf_message_ydan",
        }
    )
]

BUILD_DIR_P = Path("build/gc-eu-mq-dbg/")


# scenes/dungeons/Bmori1/Bmori1_scene
# -> name_prefix = "dungeons/Bmori1"
def gen_scenes_deps(name_prefix: str, rooms: Iterable[int]):
    name_last = name_prefix.split("/")[-1]
    return {
        f"scenes/{name_prefix}/{name_last}_room_{i}": [
            f"scenes/{name_prefix}/{name_last}_scene"
        ]
        for i in rooms
    }


HARDCODE_DEPS = {
    **gen_scenes_deps("misc/hakaana_ouke", range(3)),
    **gen_scenes_deps("overworld/ganon_tou", range(1)),
    **gen_scenes_deps("overworld/spot00", range(1)),
    **gen_scenes_deps("overworld/spot01", range(1)),
    **gen_scenes_deps("overworld/spot02", range(2)),
    **gen_scenes_deps("overworld/spot04", range(3)),
    **gen_scenes_deps("overworld/spot06", range(1)),
    **gen_scenes_deps("overworld/spot07", range(2)),
    **gen_scenes_deps("overworld/spot08", range(1)),
    **gen_scenes_deps("overworld/spot09", range(1)),
    **gen_scenes_deps("overworld/spot11", range(1)),
    **gen_scenes_deps("overworld/spot12", range(2)),
    **gen_scenes_deps("overworld/spot15", range(1)),
    **gen_scenes_deps("overworld/spot16", range(1)),
    **gen_scenes_deps("overworld/spot17", range(2)),
    **gen_scenes_deps("overworld/spot18", range(4)),
    **gen_scenes_deps("overworld/spot20", range(1)),
    **gen_scenes_deps("dungeons/bdan", range(16)),
    **gen_scenes_deps("dungeons/Bmori1", range(23)),
    **gen_scenes_deps("dungeons/ddan", range(17)),
    **gen_scenes_deps("dungeons/ganontika", range(20)),
    **gen_scenes_deps("dungeons/gerudoway", range(6)),
    **gen_scenes_deps("dungeons/HAKAdan", [3]),
    **gen_scenes_deps("dungeons/ice_doukutu", range(12)),
    **gen_scenes_deps("dungeons/jyasinboss", range(3)),
    **gen_scenes_deps("dungeons/jyasinzou", range(29)),
    **gen_scenes_deps("dungeons/men", range(11)),
    **gen_scenes_deps("dungeons/MIZUsin", [*range(13), *range(14, 23)]),
    **gen_scenes_deps("dungeons/ydan", range(12)),
    **gen_scenes_deps("indoors/miharigoya", range(1)),
    **gen_scenes_deps("indoors/souko", range(3)),
    **gen_scenes_deps("indoors/tokinoma", range(2)),
}

if 0:
    import gen_assets_list_toml_HARDCODE_DEPS_autogend

    HARDCODE_DEPS.update(gen_assets_list_toml_HARDCODE_DEPS_autogend.HARDCODE_DEPS)

lines = []

for seg in assets_segments:
    srcs = [inc.file.relative_to(BUILD_DIR_P).with_suffix(".c") for inc in seg.includes]

    is_map_assets = any(src.is_relative_to("assets/scenes/") for src in srcs)
    is_texbg_assets = any(
        src.is_relative_to("assets/textures/backgrounds/") for src in srcs
    )
    is_placetitlecard_assets = any(
        src.is_relative_to("assets/textures/place_title_cards/") for src in srcs
    )
    is_skybox_assets = any(
        src.is_relative_to("assets/textures/skyboxes/") for src in srcs
    )
    is_text = any(src.is_relative_to("assets/text/") for src in srcs)
    is_audio_assets = seg.name in {"Audiobank", "Audioseq", "Audiotable"}

    if is_text:
        # ignore text for now, not the kind of asset we're handling here
        continue

    is_shared_folder = (
        is_map_assets or is_texbg_assets or is_placetitlecard_assets or is_skybox_assets
    )

    depends = []  # TODO from xmls

    if srcs[0].is_relative_to("assets"):
        if is_shared_folder:
            name = str(srcs[0].relative_to("assets/").with_suffix(""))
        else:
            name = str(srcs[0].relative_to("assets/").parent)

        if not is_audio_assets:
            if is_shared_folder:
                xml_p = (Path("assets/xml") / name).parent.with_suffix(".xml")
            else:
                xml_p = (Path("assets/xml") / name).with_suffix(".xml")
            assert xml_p.exists(), (name, xml_p)

            with xml_p.open(encoding="UTF-8") as f:
                xmldata = ElementTree.parse(f)

            depends += [
                str(Path(e.attrib["OutPath"]).relative_to("assets"))
                for e in xmldata.getroot()
                if e.tag == "ExternalFile"
            ]

            if is_map_assets:
                is_scene = seg.name.endswith("_scene")
                if is_scene:
                    depends += [
                        str(Path(name).parent / e.attrib["Name"])
                        for e in xmldata.getroot()
                        if e.tag == "File" and e.attrib["Name"] != seg.name
                    ]
    else:
        name = str(srcs[0].with_suffix(""))

    depends += HARDCODE_DEPS.get(name, [])

    segment = seg.number
    segment_comment = ""
    if segment is None:
        segment = 0
        segment_comment = " # None"

    if name.startswith("scenes/"):
        section_name = "maps." + name.split("/")[-1]
    elif name.startswith("objects/"):
        section_name = "objects." + name.split("/")[-1]
    elif name.startswith("audio/"):
        section_name = "audio." + name.split("/")[-1]
    elif name.startswith("textures/"):
        section_name = "textures." + name.split("/")[-1]
    elif name.startswith("misc/"):
        section_name = "misc." + name.split("/")[-1]
    elif name.startswith("src/elf_message/"):
        section_name = "elf_message." + name.split("/")[-1]
    else:
        raise NotImplementedError(name)

    lines.append(f"['{section_name}']")
    if len(srcs) == 1:
        lines.append(f"srcs = '{srcs[0]}'")
    else:
        lines.append("srcs = [" + ", ".join(f"'{src}'" for src in srcs) + "]")
    if depends:
        if len(depends) == 1:
            lines.append(f"depends = '{depends[0]}'")
        else:
            lines.append("depends = [" + ", ".join(f"'{dep}'" for dep in depends) + "]")
    lines.append(f"segment = {segment}{segment_comment}")
    lines.append("")

Path("assets_list.toml").write_text("\n".join(lines), encoding="UTF-8")
