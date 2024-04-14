#include "z_demo_kankyo.h"
#include "z64cutscene_commands.h"

// clang-format off
CutsceneData gAdultWarpOutCS[] = {
    CS_BEGIN_CUTSCENE(5, 1167),
    CS_CAM_EYE_SPLINE_REL_TO_PLAYER(0, 1138),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0xA1BC),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0xA5E1),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0xA5CB),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0xA5EB),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0xA1BC),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 0, CS_FLOAT(0x42714CA8, 60.32486f), 31, 82, 61, 0xA5E1),
    CS_CAM_AT_SPLINE_REL_TO_PLAYER(0, 1167),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42714CA8, 60.32486f), 55, 99, 31, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42714CA8, 60.32486f), 55, 99, 31, 0xA1BC),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 10, CS_FLOAT(0x42714CA8, 60.32486f), 55, 99, 31, 0xA5E1),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 7, CS_FLOAT(0x42714CA8, 60.32486f), 17, 59, 31, 0xA5CB),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42714CA8, 60.32486f), 17, 59, 31, 0xA5EB),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 1000, CS_FLOAT(0x42714CA8, 60.32486f), 17, 59, 31, 0x20BA),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0x00, 30, CS_FLOAT(0x42714CA8, 60.32486f), 17, 59, 31, 0xA1BC),
        CS_CAM_POINT(CS_CAM_STOP, 0x00, 30, CS_FLOAT(0x42714CA8, 60.32486f), 17, 59, 31, 0xA5E1),
    CS_TRANSITION(CS_TRANS_GRAY_FILL_OUT, 36, 46),
    CS_TRANSITION(CS_TRANS_GRAY_FILL_IN, 30, 35),
    CS_MISC_LIST(1),
        CS_MISC(CS_MISC_STOP_CUTSCENE, 95, 97, 0x0000, 0x00000000, 0x00000000, 0xFFFFFFE3, 0xFFFFFFF9, 0x00000000, 0xFFFFFFE3, 0xFFFFFFF9, 0x00000000, 0x00000000, 0x00000000),
    CS_END(),
};
// clang-format on
