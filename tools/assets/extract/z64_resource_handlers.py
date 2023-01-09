from xml.etree import ElementTree
from typing import Callable

from .extase import File, Resource, BinaryBlobResource
from .extase.cdata_resources import Vec3sArrayResource

from . import xml_errors

#
# resource handlers
#


def resource_handler(
    file: File,
    resource_elem: ElementTree.Element,
    offset: int,
) -> Resource:
    ...


del resource_handler
ResourceHandler = Callable[[File, ElementTree.Element, int], Resource]

# Returns a dummy resource_handler that produces a `BinaryBlobResource` of the given size
# This is meant as a "placeholder resource" until a resource is properly implemented
def get_fixed_size_resource_handler(size) -> ResourceHandler:
    def resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        return BinaryBlobResource(
            file,
            offset,
            offset + size,
            resource_elem.attrib["Name"],
        )

    return resource_handler


def register_resource_handlers():

    # import in a function to avoid circular imports
    # even if Python can somewhat deal with circular imports, it still causes issues
    # with classes being defined twice
    # TODO revisit this, the situation may have changed after splitting the code

    from .extase_oot64 import (
        skeleton_resources,
        animation_resources,
        collision_resources,
        dlist_resources,
        playeranim_resources,
        skelcurve_resources,
        misc_resources,
        scene_rooms_resources,
        scene_commands_resource,
    )

    # TODO check attributes more in the handlers
    # (xml_errors.xml_check_attributes)

    def skeleton_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(
            resource_elem,
            {"Name", "Type", "LimbType"},
            {"Offset", "EnumName", "LimbNone", "LimbMax"},
        )
        # TODO clean up and make proper
        limb_type = resource_elem.attrib["LimbType"]
        if limb_type == "Standard":
            pass
        elif limb_type == "LOD":
            # TODO
            if resource_elem.attrib["Type"] == "Normal":
                # } SkeletonHeader; // size = 0x8
                return BinaryBlobResource(
                    file, offset, offset + 0x8, resource_elem.attrib["Name"]
                )
            elif resource_elem.attrib["Type"] == "Flex":
                # } FlexSkeletonHeader; // size = 0xC
                return BinaryBlobResource(
                    file, offset, offset + 0xC, resource_elem.attrib["Name"]
                )
            else:
                raise NotImplementedError(
                    "LimbType=LOD",
                    "unimplemented Skeleton Type",
                    resource_elem.attrib.get("Type"),
                )
        elif limb_type == "Skin":
            # TODO
            assert resource_elem.attrib["Type"] == "Normal"
            # } SkeletonHeader; // size = 0x8
            return BinaryBlobResource(
                file, offset, offset + 0x8, resource_elem.attrib["Name"]
            )
        elif limb_type == "Curve":
            assert resource_elem.attrib["Type"] == "Curve"
            return skelcurve_resources.CurveSkeletonHeaderResource(
                file, offset, resource_elem.attrib["Name"]
            )
        else:
            raise NotImplementedError(
                "unimplemented Skeleton LimbType",
                resource_elem.attrib.get("LimbType"),
            )

        if resource_elem.attrib["Type"] == "Normal":
            return skeleton_resources.SkeletonNormalResource(
                file,
                offset,
                resource_elem.attrib["Name"],
            )
        elif resource_elem.attrib["Type"] == "Flex":
            return skeleton_resources.SkeletonFlexResource(
                file,
                offset,
                resource_elem.attrib["Name"],
            )
        else:
            raise NotImplementedError(
                "unimplemented Skeleton Type",
                resource_elem.attrib["Type"],
            )

    def limb_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(
            resource_elem, {"Name", "LimbType"}, {"Offset", "EnumName"}
        )
        if resource_elem.attrib["LimbType"] == "Standard":
            return skeleton_resources.StandardLimbResource(
                file,
                offset,
                resource_elem.attrib["Name"],
            )
        elif resource_elem.attrib["LimbType"] == "Skin":
            # } SkinLimb; // size = 0x10
            return BinaryBlobResource(
                file, offset, offset + 0x10, resource_elem.attrib["Name"]
            )
        elif resource_elem.attrib["LimbType"] == "LOD":
            # } LodLimb; // size = 0x10
            return BinaryBlobResource(
                file, offset, offset + 0x10, resource_elem.attrib["Name"]
            )
        elif resource_elem.attrib["LimbType"] == "Legacy":
            # } LegacyLimb; // size = 0x20
            return BinaryBlobResource(
                file, offset, offset + 0x20, resource_elem.attrib["Name"]
            )
        elif resource_elem.attrib["LimbType"] == "Curve":
            return skelcurve_resources.SkelCurveLimbResource(
                file, offset, resource_elem.attrib["Name"]
            )
        else:
            raise NotImplementedError(
                "unimplemented Limb Type",
                resource_elem.attrib["LimbType"],
            )

    def animation_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset"})
        return animation_resources.AnimationResource(
            file,
            offset,
            resource_elem.attrib["Name"],
        )

    def collision_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset"})
        return collision_resources.CollisionResource(
            file,
            offset,
            resource_elem.attrib["Name"],
        )

    def dlist_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset", "Ucode"})
        ucode_str = resource_elem.get("Ucode")
        if ucode_str is None:
            ucode = dlist_resources.Ucode.f3dex2
        else:
            ucode = dlist_resources.Ucode[ucode_str]
        return dlist_resources.DListResource(
            file,
            offset,
            resource_elem.attrib["Name"],
            target_ucode=ucode,
        )

    texture_formats = {
        f"{fmt.name.lower()}{siz.bpp}": (fmt, siz)
        for fmt in dlist_resources.G_IM_FMT
        for siz in dlist_resources.G_IM_SIZ
    }

    def texture_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        # the reference document says OutName is mandatory but there are resources without it
        xml_errors.xml_check_attributes(
            resource_elem,
            {"Name", "Format", "Width", "Height"},
            {
                "Offset",
                "OutName",
                "TlutOffset",
                "ExternalTlut",
                "ExternalTlutOffset",
                "SplitTlut",
            },
        )
        # TODO use OutName, TlutOffset
        format_str = resource_elem.attrib["Format"]
        width_str = resource_elem.attrib["Width"]
        height_str = resource_elem.attrib["Height"]

        try:
            format_fmt, format_siz = texture_formats[format_str.lower()]
        except KeyError as e:
            raise xml_errors.XmlProcessError(
                "Bad texture Format", format_str, xml_elem=resource_elem
            ) from e

        width = int(width_str)
        height = int(height_str)

        return dlist_resources.TextureResource(
            file,
            offset,
            resource_elem.attrib["Name"],
            format_fmt,
            format_siz,
            width,
            height,
        )

    def PlayerAnimationData_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(
            resource_elem, {"Name", "FrameCount"}, {"Offset"}
        )
        frame_count_str = resource_elem.attrib["FrameCount"]
        frame_count = int(frame_count_str)
        size = frame_count * (22 * 3 + 1) * 2
        return BinaryBlobResource(
            file,
            offset,
            offset + size,
            resource_elem.attrib["Name"],
        )
        # TODO
        return skeleton_resources.PlayerAnimationDataResource(
            file,
            offset,
            resource_elem.attrib["Name"],
            resource_elem.attrib["FrameCount"],
        )

    def PlayerAnimation_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset"})
        return playeranim_resources.PlayerAnimationResource(
            file,
            offset,
            resource_elem.attrib["Name"],
        )

    def array_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name", "Count"}, {"Offset"})
        # TODO clean up (the asserts)
        count_str = resource_elem.attrib["Count"]
        count = int(count_str)
        assert count > 0
        if len(resource_elem) != 1:
            raise Exception(
                "Expected exactly one child of Array node",
                resource_elem,
                len(resource_elem),
            )
        elem_elem = resource_elem[0]
        if elem_elem.tag == "Vector":
            elem_type = elem_elem.attrib["Type"]
            assert elem_type == "s16"
            elem_dim_str = elem_elem.attrib["Dimensions"]
            elem_dim = int(elem_dim_str)
            assert elem_dim == 3
            return Vec3sArrayResource(file, offset, resource_elem.attrib["Name"], count)
        elif elem_elem.tag == "Vtx":
            return dlist_resources.VtxArrayResource(
                file,
                offset,
                offset
                + count * dlist_resources.VtxArrayResource.element_cdata_ext.size,
                resource_elem.attrib["Name"],
            )
        else:
            raise NotImplementedError("Array of", elem_elem.tag)

    def binary_blob_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name", "Size"}, {"Offset"})
        size_str = resource_elem.attrib["Size"]
        size = int(size_str, 16)
        return BinaryBlobResource(
            file,
            offset,
            offset + size,
            resource_elem.attrib["Name"],
        )

    def CurveAnimation_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(
            resource_elem, {"Name", "SkelOffset"}, {"Offset"}
        )
        # TODO use SkelOffset
        return skelcurve_resources.CurveAnimationHeaderResource(
            file, offset, resource_elem.attrib["Name"]
        )

    def Mtx_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset"})
        return dlist_resources.MtxResource(file, offset, resource_elem.attrib["Name"])

    def cutscene_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset"})
        return misc_resources.CutsceneResource(
            file, offset, resource_elem.attrib["Name"]
        )

    def scene_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset"})
        return scene_commands_resource.SceneCommandsResource(
            file, offset, resource_elem.attrib["Name"]
        )

    def room_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset", "HackMode"})
        if resource_elem.attrib.get("HackMode") == "syotes_room":
            # TODO
            return BinaryBlobResource(
                file,
                offset,
                offset + 4,
                resource_elem.attrib["Name"] + "_HackMode_syotes_room",
            )
        return scene_commands_resource.SceneCommandsResource(
            file, offset, resource_elem.attrib["Name"]
        )

    def path_list_resource_handler(
        file: File,
        resource_elem: ElementTree.Element,
        offset: int,
    ):
        xml_errors.xml_check_attributes(resource_elem, {"Name"}, {"Offset", "NumPaths"})
        num_paths_str = resource_elem.attrib.get("NumPaths")
        if num_paths_str is not None:
            num_paths = int(num_paths_str)
        else:
            num_paths = None
        resource = scene_rooms_resources.PathListResource(
            file, offset, resource_elem.attrib["Name"]
        )
        if num_paths:
            resource.set_length(num_paths)
        return resource

    RESOURCE_HANDLERS.update(
        {
            "Skeleton": skeleton_resource_handler,
            "Limb": limb_resource_handler,
            "Animation": animation_resource_handler,
            "Collision": collision_resource_handler,
            "DList": dlist_resource_handler,
            "Texture": texture_resource_handler,
            "PlayerAnimationData": PlayerAnimationData_handler,
            "Array": array_resource_handler,
            "PlayerAnimation": PlayerAnimation_handler,
            "Blob": binary_blob_resource_handler,
            "Mtx": Mtx_handler,
            "LegacyAnimation": get_fixed_size_resource_handler(0xC),  # TODO
            "LimbTable": get_fixed_size_resource_handler(
                # idk, probably an array
                4
            ),  # TODO
            "CurveAnimation": CurveAnimation_handler,
            "Scene": scene_resource_handler,
            "Room": room_resource_handler,
            "Path": path_list_resource_handler,
            "Cutscene": cutscene_resource_handler,
        }
    )


RESOURCE_HANDLERS: dict[str, ResourceHandler] = {}


def get_resource_from_xml(
    file: File,
    resource_elem: ElementTree.Element,
    offset: int,
) -> Resource:
    resource_handler = RESOURCE_HANDLERS.get(resource_elem.tag)

    if resource_handler is None:
        raise Exception("Unknown resource tag", resource_elem.tag)

    resource = resource_handler(file, resource_elem, offset)

    static_str = resource_elem.attrib.get("Static")
    if static_str is not None:
        assert static_str in {"On", "Off"}
        is_static = static_str == "On"
        if is_static:
            # TODO nice hack right here.
            # probably instead rework the "c declaration" system into a more opaque object
            # not that this is really a required long term feature as it's only relevant for writing the source files (main .c/.h), not extracting
            if file.name.startswith("ovl_"):
                resource.HACK_IS_STATIC_ON = ...
            else:
                # TODO
                # object_link_ boy/child has Static="On"
                # and ofc the static forward declaration means the .h declares bss
                # not sure what "Static" means... again not really required long term
                print("Ignoring static on", file.name)
        """
        see https://github.com/zeldaret/ZAPD/blob/master/docs/zapd_extraction_xml_reference.md#resources-types
        for what's the deal with static
        and indeed extract_assets.py has
        if "overlays" in xmlPath:
            execStr += " --static"
        """

    return resource
