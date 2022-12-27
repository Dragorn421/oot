import enum


class G_IM_FMT(enum.Enum):
    RGBA = 0
    YUV = 1
    CI = 2
    IA = 3
    I = 4

    def __init__(self, i: int):
        self.i = i

    by_i: dict[int, "G_IM_FMT"]


G_IM_FMT.by_i = {fmt.i: fmt for fmt in G_IM_FMT}


class G_IM_SIZ(enum.Enum):
    _4b = (0, 4)
    _8b = (1, 8)
    _16b = (2, 16)
    _32b = (3, 32)

    def __init__(self, i: int, bpp: int):
        self.i = i
        self.bpp = bpp

    by_i: dict[int, "G_IM_SIZ"]


G_IM_SIZ.by_i = {siz.i: siz for siz in G_IM_SIZ}
