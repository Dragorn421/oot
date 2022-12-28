import struct
import io

from ..extase import File, Resource

from tools import csdis


class CutsceneResource(Resource, can_size_be_unknown=True):
    needs_build = True

    def __init__(self, file: File, range_start: int, name: str):
        super().__init__(file, range_start, None, name)

    def try_parse_data(self):
        assert self.range_start % 4 == 0
        data = self.file.data[self.range_start :]
        num_bytes = len(data)
        if num_bytes % 4 != 0:
            data = data[: -(num_bytes % 4)]
        data_words = [unpacked[0] for unpacked in struct.iter_unpack(">I", data)]
        size_words, cs_source = csdis.disassemble_cutscene(data_words)
        self.range_end = self.range_start + 4 * size_words
        self.cs_source = cs_source
        self.is_data_parsed = True

    def get_c_reference(self, resource_offset: int):
        raise ValueError

    extracted_path_suffix = ".f32.inc.c"

    def get_filename_stem(self):
        return f"{self.name}.csdata"

    def write_extracted(self):
        with self.extract_to_path.open("w") as f:
            f.write('#include "z64cutscene_commands.h"\n')
            f.write("{\n")
            f.write(self.cs_source)
            f.write("}\n")

    def get_c_declaration_base(self):
        return f"CutsceneData {self.symbol_name}[]"
