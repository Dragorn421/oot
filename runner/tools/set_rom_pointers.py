import sys
import struct

noPrint = True
if noPrint:
    def print(*args):
        pass

with open('asm/dmadata.s') as f:
    lines = f.read().splitlines()
found_gDmaDataTable = False
dmaDataTableNames = []
for i in range(len(lines)):
    if lines[i] == 'glabel gDmaDataTable':
        found_gDmaDataTable = True
    elif found_gDmaDataTable and lines[i].startswith('DMA_TABLE_ENTRY '):
        dmaDataTableNames.append(lines[i][len('DMA_TABLE_ENTRY '):])

print(f"test_scene dma table idx = {dmaDataTableNames.index('test_scene')}")
print(f"test_room_0 dma table idx = {dmaDataTableNames.index('test_room_0')}")

#dmaDataRomOffset = 0x00012F70 # vanilla debug
dmaDataRomOffset = 0x00012F30 # don't know how but my rom shifts the dma data backwards

with open('zelda_ocarina_mq_dbg.z64', 'rb') as rom:
    rom.seek(dmaDataRomOffset)
    firstEntryBytes = rom.read(16)
    print(f'firstEntryBytes = {firstEntryBytes}')
    if firstEntryBytes != b'\x00\x00\x00\x00\x00\x00\x10\x60\x00\x00\x00\x00\x00\x00\x00\x00':
        print(f'Bad dmaDataRomOffset = {dmaDataRomOffset:08X}, first entry is unexpected')
        sys.exit(1)

dmaDataRomOffset_test_scene = dmaDataRomOffset + dmaDataTableNames.index('test_scene') * 0x10
dmaDataRomOffset_test_room_0 = dmaDataRomOffset + dmaDataTableNames.index('test_room_0') * 0x10

print(f'dmaDataRomOffset_test_scene = {dmaDataRomOffset_test_scene:08X}')
print(f'dmaDataRomOffset_test_room_0 = {dmaDataRomOffset_test_room_0:08X}')

with open('zelda_ocarina_mq_dbg.z64', 'rb+') as rom:
    rom.seek(dmaDataRomOffset_test_room_0)
    vromStart_test_room_0, vromEnd_test_room_0 = struct.unpack('>II', rom.read(8))
    print(f'vromStart_test_room_0 = {vromStart_test_room_0:08X}')
    print(f'vromEnd_test_room_0 = {vromEnd_test_room_0:08X}')

    rom.seek(dmaDataRomOffset_test_scene)
    vromStart_test_scene = struct.unpack('>I', rom.read(4))[0]
    print(f'vromStart_test_scene = {vromStart_test_scene:08X}')

    rom.seek(vromStart_test_scene)
    roomListRomOffset_test_scene = None
    while roomListRomOffset_test_scene is None:
        highWord, lowWord = struct.unpack('>II', rom.read(8))
        if highWord >> 24 & 0xFF == 0x04:
            roomListRomOffset_test_scene = vromStart_test_scene + (lowWord & 0x00FFFFFF)
    print(f'roomListRomOffset_test_scene = {roomListRomOffset_test_scene:08X}')

    rom.seek(roomListRomOffset_test_scene)
    rom.write(struct.pack('>II', vromStart_test_room_0, vromEnd_test_room_0))
