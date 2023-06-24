import os
from pathlib import Path

for dirpath, dirnames, filenames in os.walk("src/overlays"):
    if not dirnames:
        # dirpath is a leaf folder: no other folder inside it
        # assume it corresponds to an overlay of same segment name as the folder name
        dirpath_p = Path(dirpath)
        segment_name = dirpath_p.name
        reloc_p = Path("build") / dirpath_p / f"{segment_name}_reloc.o"
        print(str(reloc_p))
