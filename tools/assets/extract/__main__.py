from __future__ import annotations
# The purpose of this file atm is running extract_xml only once it's
# fully imported, to avoid weird import behavior
# TODO revisit this, the situation may have changed after splitting the code

from . import extract_xml_z64

if __name__ == "__main__":
    profile = True
    if profile:
        import cProfile

        cProfile.run("extract_xml_z64.main()", "cprof_assets421.txt")
    else:
        extract_xml_z64.main()
