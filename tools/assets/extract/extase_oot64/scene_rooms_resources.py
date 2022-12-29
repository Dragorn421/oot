import enum
import struct

from ..extase import File, Resource
from ..extase.cdata_resources import (
    CDataArrayNamedLengthResource,
    CDataExt_Struct,
    CDataExt_Value,
    cdata_ext_Vec3s,
)

from . import room_shape_resources


def _SHIFTR(v: int, s: int, w: int):
    assert isinstance(v, int)
    assert isinstance(s, int)
    assert isinstance(w, int)
    assert v >= 0
    assert s >= 0
    assert w >= 1
    return (v >> s) & ((1 << w) - 1)


class SceneCmdId(enum.Enum):
    # keep the SCENE_CMD_ID_ prefix for grepability
    SCENE_CMD_ID_SPAWN_LIST = 0
    SCENE_CMD_ID_ACTOR_LIST = enum.auto()
    SCENE_CMD_ID_UNUSED_2 = enum.auto()
    SCENE_CMD_ID_COLLISION_HEADER = enum.auto()
    SCENE_CMD_ID_ROOM_LIST = enum.auto()
    SCENE_CMD_ID_WIND_SETTINGS = enum.auto()
    SCENE_CMD_ID_ENTRANCE_LIST = enum.auto()
    SCENE_CMD_ID_SPECIAL_FILES = enum.auto()
    SCENE_CMD_ID_ROOM_BEHAVIOR = enum.auto()
    SCENE_CMD_ID_UNDEFINED_9 = enum.auto()
    SCENE_CMD_ID_ROOM_SHAPE = enum.auto()
    SCENE_CMD_ID_OBJECT_LIST = enum.auto()
    SCENE_CMD_ID_LIGHT_LIST = enum.auto()
    SCENE_CMD_ID_PATH_LIST = enum.auto()
    SCENE_CMD_ID_TRANSITION_ACTOR_LIST = enum.auto()
    SCENE_CMD_ID_LIGHT_SETTINGS_LIST = enum.auto()
    SCENE_CMD_ID_TIME_SETTINGS = enum.auto()
    SCENE_CMD_ID_SKYBOX_SETTINGS = enum.auto()
    SCENE_CMD_ID_SKYBOX_DISABLES = enum.auto()
    SCENE_CMD_ID_EXIT_LIST = enum.auto()
    SCENE_CMD_ID_END = enum.auto()
    SCENE_CMD_ID_SOUND_SETTINGS = enum.auto()
    SCENE_CMD_ID_ECHO_SETTINGS = enum.auto()
    SCENE_CMD_ID_CUTSCENE_DATA = enum.auto()
    SCENE_CMD_ID_ALTERNATE_HEADER_LIST = enum.auto()
    SCENE_CMD_ID_MISC_SETTINGS = enum.auto()


class Data1Handler:
    ...


class Data1Handlers:
    RAW = Data1Handler()


scene_cmd_macro_name_by_cmd_id = {
    SceneCmdId.SCENE_CMD_ID_SPAWN_LIST: "SCENE_CMD_SPAWN_LIST",
    SceneCmdId.SCENE_CMD_ID_ACTOR_LIST: "SCENE_CMD_ACTOR_LIST",
    SceneCmdId.SCENE_CMD_ID_UNUSED_2: "SCENE_CMD_UNUSED_02",
    SceneCmdId.SCENE_CMD_ID_COLLISION_HEADER: "SCENE_CMD_COL_HEADER",
    SceneCmdId.SCENE_CMD_ID_ROOM_LIST: "SCENE_CMD_ROOM_LIST",
    SceneCmdId.SCENE_CMD_ID_WIND_SETTINGS: "SCENE_CMD_WIND_SETTINGS",
    SceneCmdId.SCENE_CMD_ID_ENTRANCE_LIST: "SCENE_CMD_ENTRANCE_LIST",
    SceneCmdId.SCENE_CMD_ID_SPECIAL_FILES: "SCENE_CMD_SPECIAL_FILES",
    SceneCmdId.SCENE_CMD_ID_ROOM_BEHAVIOR: "SCENE_CMD_ROOM_BEHAVIOR",
    SceneCmdId.SCENE_CMD_ID_UNDEFINED_9: "SCENE_CMD_UNK_09",
    SceneCmdId.SCENE_CMD_ID_ROOM_SHAPE: "SCENE_CMD_ROOM_SHAPE",
    SceneCmdId.SCENE_CMD_ID_OBJECT_LIST: "SCENE_CMD_OBJECT_LIST",
    SceneCmdId.SCENE_CMD_ID_LIGHT_LIST: "SCENE_CMD_LIGHT_LIST",
    SceneCmdId.SCENE_CMD_ID_PATH_LIST: "SCENE_CMD_PATH_LIST",
    SceneCmdId.SCENE_CMD_ID_TRANSITION_ACTOR_LIST: "SCENE_CMD_TRANSITION_ACTOR_LIST",
    SceneCmdId.SCENE_CMD_ID_LIGHT_SETTINGS_LIST: "SCENE_CMD_ENV_LIGHT_SETTINGS",
    SceneCmdId.SCENE_CMD_ID_TIME_SETTINGS: "SCENE_CMD_TIME_SETTINGS",
    SceneCmdId.SCENE_CMD_ID_SKYBOX_SETTINGS: "SCENE_CMD_SKYBOX_SETTINGS",
    SceneCmdId.SCENE_CMD_ID_SKYBOX_DISABLES: "SCENE_CMD_SKYBOX_DISABLES",
    SceneCmdId.SCENE_CMD_ID_EXIT_LIST: "SCENE_CMD_EXIT_LIST",
    SceneCmdId.SCENE_CMD_ID_END: "SCENE_CMD_END",
    SceneCmdId.SCENE_CMD_ID_SOUND_SETTINGS: "SCENE_CMD_SOUND_SETTINGS",
    SceneCmdId.SCENE_CMD_ID_ECHO_SETTINGS: "SCENE_CMD_ECHO_SETTINGS",
    SceneCmdId.SCENE_CMD_ID_CUTSCENE_DATA: "SCENE_CMD_CUTSCENE_DATA",
    SceneCmdId.SCENE_CMD_ID_ALTERNATE_HEADER_LIST: "SCENE_CMD_ALTERNATE_HEADER_LIST",
    SceneCmdId.SCENE_CMD_ID_MISC_SETTINGS: "SCENE_CMD_MISC_SETTINGS",
}


class SceneCommandsResource(Resource, can_size_be_unknown=True):
    def __init__(self, file: File, range_start: int, name: str):
        super().__init__(file, range_start, None, name)

    def try_parse_data(self):
        data = self.file.data[self.range_start :]

        offset = 0
        cmd_id = None
        end_offset = None

        while offset + 8 <= len(data):
            (cmd_id_int, data1, pad2, data2_I) = struct.unpack_from(
                ">BBHI", data, offset
            )
            (_, data2_H0, data2_H1) = struct.unpack_from(">IHH", data, offset)
            (_, data2_B0, data2_B1, data2_B2, data2_B3) = struct.unpack_from(
                ">IBBBB", data, offset
            )

            offset += 8
            cmd_id = SceneCmdId(cmd_id_int)
            assert pad2 == 0

            if cmd_id == SceneCmdId.SCENE_CMD_ID_END:
                end_offset = offset
                break

            if cmd_id == SceneCmdId.SCENE_CMD_ID_ACTOR_LIST:
                resource, _ = self.file.memory_context.report_resource_at_segmented(
                    data2_I,
                    lambda file, offset: ActorEntryListResource(
                        file, offset, f"{self.name}_{data2_I:08X}_ActorList"
                    ),
                )
                assert isinstance(resource, ActorEntryListResource)
                resource.set_length(data1)

            if cmd_id == SceneCmdId.SCENE_CMD_ID_OBJECT_LIST:
                resource, _ = self.file.memory_context.report_resource_at_segmented(
                    data2_I,
                    lambda file, offset: ObjectListResource(
                        file, offset, f"{self.name}_{data2_I:08X}_ObjectList"
                    ),
                )
                assert isinstance(resource, ObjectListResource)
                resource.set_length(data1)

            if cmd_id == SceneCmdId.SCENE_CMD_ID_ROOM_SHAPE:
                room_shape_resources.report_room_shape_at_segmented(
                    self.file.memory_context, data2_I, self.name
                )

        if cmd_id != SceneCmdId.SCENE_CMD_ID_END:
            raise Exception("reached end of data without encountering end marker")
        assert end_offset is not None

        self.range_end = self.range_start + end_offset
        self.is_data_parsed = True

    def get_c_declaration_base(self):
        return f"SceneCmd {self.symbol_name}[]"

    def write_extracted(self):
        data = self.file.data[self.range_start : self.range_end]
        with self.extract_to_path.open("w") as f:
            f.write("{\n")
            for offset in range(0, len(data), 8):
                (cmd_id_int, data1, pad2, data2_I) = struct.unpack_from(
                    ">BBHI", data, offset
                )
                (_, data2_H0, data2_H1) = struct.unpack_from(">IHH", data, offset)
                (_, data2_B0, data2_B1, data2_B2, data2_B3) = struct.unpack_from(
                    ">IBBBB", data, offset
                )
                cmd_id = SceneCmdId(cmd_id_int)
                f.write(" " * 4)
                f.write(scene_cmd_macro_name_by_cmd_id[cmd_id])
                f.write("(")
                if cmd_id == SceneCmdId.SCENE_CMD_ID_SPAWN_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ACTOR_LIST:
                    address = data2_I
                    f.write(
                        self.file.memory_context.get_c_expression_length_at_segmented(
                            address
                        )
                    )
                    f.write(", ")
                    f.write(
                        self.file.memory_context.get_c_reference_at_segmented(address)
                    )
                if cmd_id == SceneCmdId.SCENE_CMD_ID_UNUSED_2:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_COLLISION_HEADER:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ROOM_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_WIND_SETTINGS:
                    assert data1 == 0
                    # TODO cast x,y,z to s8
                    xDir = data2_B0
                    yDir = data2_B1
                    zDir = data2_B2
                    strength = data2_B3
                    f.write(f"{xDir}, {yDir}, {zDir}, {strength}")
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ENTRANCE_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_SPECIAL_FILES:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ROOM_BEHAVIOR:
                    gpFlags1 = data1
                    gpFlags2 = data2_I
                    curRoomUnk3 = gpFlags1
                    curRoomUnk2 = _SHIFTR(gpFlags2, 0, 8)
                    showInvisActors = _SHIFTR(gpFlags2, 8, 1)
                    disableWarpSongs = _SHIFTR(gpFlags2, 10, 1)
                    f.write(
                        f"{curRoomUnk3}, {curRoomUnk2}, {showInvisActors}, {disableWarpSongs}"
                    )
                if cmd_id == SceneCmdId.SCENE_CMD_ID_UNDEFINED_9:
                    assert data1 == 0
                    assert data2_I == 0
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ROOM_SHAPE:
                    assert data1 == 0  # TODO these asserts should be done on parse?
                    address = data2_I
                    f.write(
                        self.file.memory_context.get_c_reference_at_segmented(address)
                    )
                if cmd_id == SceneCmdId.SCENE_CMD_ID_OBJECT_LIST:
                    address = data2_I
                    f.write(
                        self.file.memory_context.get_c_expression_length_at_segmented(
                            address
                        )
                    )
                    f.write(", ")
                    f.write(
                        self.file.memory_context.get_c_reference_at_segmented(address)
                    )
                if cmd_id == SceneCmdId.SCENE_CMD_ID_LIGHT_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_PATH_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_TRANSITION_ACTOR_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_LIGHT_SETTINGS_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_TIME_SETTINGS:
                    assert data1 == 0
                    hour = data2_B0
                    min_ = data2_B1
                    timeSpeed = data2_B2
                    assert data2_B3 == 0
                    f.write(f"{hour}, {min_}, {timeSpeed}")
                if cmd_id == SceneCmdId.SCENE_CMD_ID_SKYBOX_SETTINGS:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_SKYBOX_DISABLES:
                    assert data1 == 0
                    skyboxDisabled = data2_B0
                    sunMoonDisabled = data2_B1
                    assert data2_B2 == data2_B3 == 0
                    f.write(f"{skyboxDisabled}, {sunMoonDisabled}")
                if cmd_id == SceneCmdId.SCENE_CMD_ID_EXIT_LIST:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_END:
                    assert data1 == 0
                    assert data2_I == 0
                if cmd_id == SceneCmdId.SCENE_CMD_ID_SOUND_SETTINGS:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ECHO_SETTINGS:
                    assert data1 == 0
                    assert data2_B0 == data2_B1 == data2_B2 == 0
                    echo = data2_B3
                    f.write(f"{echo}")
                if cmd_id == SceneCmdId.SCENE_CMD_ID_CUTSCENE_DATA:
                    raise NotImplementedError(cmd_id)
                if cmd_id == SceneCmdId.SCENE_CMD_ID_ALTERNATE_HEADER_LIST:
                    # TODO
                    assert data1 == 0
                    address = data2_I
                    f.write(f"0x{address:08X}")
                if cmd_id == SceneCmdId.SCENE_CMD_ID_MISC_SETTINGS:
                    raise NotImplementedError(cmd_id)
                f.write("),\n")
            f.write("}\n")

    def get_c_reference(self, resource_offset: int):
        raise ValueError


class ActorEntryListResource(CDataArrayNamedLengthResource):
    elem_cdata_ext = CDataExt_Struct(
        (
            ("id", CDataExt_Value.s16),
            ("pos", cdata_ext_Vec3s),
            ("rot", cdata_ext_Vec3s),
            ("params", CDataExt_Value.s16),
        )
    )

    def get_c_declaration_base(self):
        return f"ActorEntry {self.symbol_name}[{self.length_name}]"


class ObjectListResource(CDataArrayNamedLengthResource):
    elem_cdata_ext = CDataExt_Value.s16

    def get_c_declaration_base(self):
        return f"s16 {self.symbol_name}[{self.length_name}]"
