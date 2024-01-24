def add_custom_arguments(parser):
    parser.add_argument("-v", "--oot-version", help="OOT version", default="gc-eu-mq-dbg")

def apply(config, args):
    version = args.oot_version
    config['mapfile'] = f'build/{version}/z64.map'
    config['myimg'] = f'oot-{version}.z64'
    config['baseimg'] = 'baseroms/gc-eu-mq-dbg/baserom-decompressed.z64'
    config['makeflags'] = [f'VERSION={version}']
    config['source_directories'] = ['src', 'include', 'spec']
