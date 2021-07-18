import bpy
import sys
import os

argv = sys.argv
if not '--' in argv:
	print('Cannot export: No export arguments')
	sys.exit(1)
argv = argv[argv.index('--') + 1:]

if len(argv) != 1:
    print(f'Expected exactly 1 argument, got {len(argv)}: {argv}')
    sys.exit(1)

bpy.ops.objex.export(
    filepath = argv[0],
    # disable logging through operator's report() which fixes the triple printing of each log line
    logging_level_report = 51,
    path_mode = 'RELATIVE',
)
sys.exit()
