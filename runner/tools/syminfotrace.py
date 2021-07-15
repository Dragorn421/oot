stackTraceStr = """
8022cf50
8022cca8
8022cb40
80030a40
800bdae8
800bf14c
800c4054
800c5808
800c5ea0
"""
overlaysStr = """
8022c860-8022d500 TestActor
"""

import os, subprocess

def sym_info(address):
    symInfoPyPath = 'sym_info.py'
    for i in range(2):
        if not os.path.isfile(symInfoPyPath):
            symInfoPyPath = '../' + symInfoPyPath
    return subprocess.run(['python3', symInfoPyPath, hex(address)], stdout=subprocess.PIPE).stdout.decode().rstrip('\n')

stackTrace = [int(l, 16) for l in stackTraceStr.splitlines() if l]
overlays = []
for l in overlaysStr.splitlines():
    if not l:
        continue
    dashIdx = l.index('-')
    spaceIdx = l.index(' ')
    if dashIdx < 0:
        print(f'malformatted line in overlaysStr: {l} (expected "<startHex>-<endHex> <name>", eg "8022c860-8022d500 TestActor"')
        continue
    overlays.append((int(l[:dashIdx], 16), int(l[dashIdx+1:spaceIdx], 16), l[spaceIdx+1:]))

for stackTraceAddr in stackTrace:
    symInfoRes = sym_info(stackTraceAddr)
    print(symInfoRes)
    inOverlays = [ovl for ovl in overlays if ovl[0] <= stackTraceAddr and stackTraceAddr < ovl[1]]
    if len(inOverlays) > 1:
        print('\tWeird, found several overlays where this address belongs:')
        print(f'\t{inOverlays}')
    if 'gSystemHeap' in symInfoRes:
        if not inOverlays:
            print('\tThis address does not belong in the listed overlays')
        else:
            inOvl = inOverlays[0]
            mapPath = 'build/z64.map'
            for i in range(2):
                if not os.path.isfile(mapPath):
                    mapPath = '../' + mapPath
            searchSymbol = f'_ovl_{inOvl[2]}SegmentStart'
            with open(mapPath) as mapF:
                matches = [l for l in mapF if searchSymbol in l]
            if len(matches) != 1:
                print(f'Found not exactly 1 match when searching {searchSymbol} in the map:')
                print(matches)
            if matches:
                m = matches[0]
                hexAddrStartIdx = m.index('0x')
                hexAddrEndIdx = m.index(' ', hexAddrStartIdx)
                ovlVaddr = int(m[hexAddrStartIdx+2:hexAddrEndIdx], 16)
                symInfoRes2 = sym_info(stackTraceAddr - inOvl[0] + ovlVaddr)
                print(f'\t{symInfoRes2}')
    elif inOverlays:
        print('\tWeird, this address is not in gSystemHeap but belongs in the listed overlays:')
        print(f'\t{inOverlays}')
