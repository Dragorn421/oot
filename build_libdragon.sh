#!/usr/bin/env bash
set -e


if [[ -z ${N64_GCCPREFIX-} ]]; then
    if [[ -z ${N64_INST-} ]]; then
        echo neither N64_GCCPREFIX nor N64_INST is set
        exit 1
    fi
    export N64_GCCPREFIX="${N64_INST}"
    echo "N64_GCCPREFIX set to ${N64_GCCPREFIX} (from N64_INST)"
else
    echo "N64_GCCPREFIX kept as ${N64_GCCPREFIX}"
fi

mkdir -p build/libdragon
export N64_INST=$(realpath build/libdragon)

echo "N64_INST set to ${N64_INST}"

set -x

# If building libdragon fails try `make -C libdragon clean`, to clean any previous build files.
make -C libdragon
make -C libdragon tools
mkdir -p "${N64_INST}/mips64-elf/include" # temporary workaround for libdragon's make install not creating that folder
make -C libdragon install
make -C libdragon tools-install
