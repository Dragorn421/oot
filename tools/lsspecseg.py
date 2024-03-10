from pathlib import Path
import argparse

import pyspec

SPEC_P = Path("build/gc-eu-mq/spec")

parser = argparse.ArgumentParser()
parser.add_argument("--segment", default=None)

args = parser.parse_args()

if args.segment:
    segment = pyspec.parse_spec_p(SPEC_P).find_segment_by_name(args.segment)
    for inc in segment.includes:
        print(inc.file)
