MAKEFLAGS += --no-builtin-rules

# Ensure the build fails if a piped command fails
SHELL = /usr/bin/env bash
.SHELLFLAGS = -o pipefail -c

#### Build options ####

# Build options can be changed by modifying them below, or by appending 'SETTING=value' to all 'make' commands
# (e.g. 'make setup VERSION=ntsc-1.0' and 'make VERSION=ntsc-1.0' to build the NTSC 1.0 version).
# Alternatively, you can create a file called .make_options.mk (gitignored by default) and add 'SETTING=value'
# there to avoid modifying the Makefile directly.

-include .make_options.mk

# Target game version. Ensure the corresponding input ROM is placed in baseroms/$(VERSION)/baserom.z64.
# Currently the following versions are supported:
#   ntsc-1.0       N64 NTSC 1.0 (Japan/US depending on REGION)
#   ntsc-1.1       N64 NTSC 1.1 (Japan/US depending on REGION)
#   pal-1.0        N64 PAL 1.0 (Europe)
#   ntsc-1.2       N64 NTSC 1.2 (Japan/US depending on REGION)
#   pal-1.1        N64 PAL 1.1 (Europe)
#   gc-jp          GameCube Japan
#   gc-jp-mq       GameCube Japan Master Quest
#   gc-us          GameCube US
#   gc-us-mq       GameCube US Master Quest
#   gc-eu-dbg-2    GameCube Europe/PAL Debug (earlier build)
#   gc-eu-mq-dbg   GameCube Europe/PAL Master Quest Debug (default)
#   gc-eu-dbg      GameCube Europe/PAL Debug
#   gc-eu          GameCube Europe/PAL
#   gc-eu-mq       GameCube Europe/PAL Master Quest
#   gc-jp-ce       GameCube Japan (Collector's Edition disc)
#   ique-cn        iQue Player (Simplified Chinese)
VERSION ?= gc-eu-mq-dbg
# Number of threads to extract assets with.
N_THREADS ?= $(shell nproc)
# Set to override game region in the ROM header (options: JP, US, EU). This can be used to build a fake US version
# of the debug ROM for better emulator compatibility, or to build US versions of NTSC N64 ROMs.
# REGION ?= US
# Set to enable debug features regardless of ROM version.
# Note that by enabling debug features on non-debug ROM versions, some debug ROM specific assets will not be included.
# This means the debug test scenes and some debug graphics in the elf_msg actors will not work as expected.
# This may also be used to disable debug features on debug ROMs by setting DEBUG_FEATURES to 0
# DEBUG_FEATURES ?= 1

# Version-specific settings
ifeq ($(VERSION),ntsc-1.0)
  REGION ?= JP
  PLATFORM := N64
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),ntsc-1.1)
  REGION ?= JP
  PLATFORM := N64
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),pal-1.0)
  REGION ?= EU
  PLATFORM := N64
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),ntsc-1.2)
  REGIONAL_CHECKSUM := 1
  REGION ?= JP
  PLATFORM := N64
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),pal-1.1)
  REGION ?= EU
  PLATFORM := N64
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-jp)
  REGION ?= JP
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-jp-mq)
  REGION ?= JP
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-us)
  REGION ?= US
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-us-mq)
  REGION ?= US
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-eu-dbg-2)
  REGION ?= EU
  PLATFORM := GC
  DEBUG_FEATURES ?= 1
else ifeq ($(VERSION),gc-eu-mq-dbg)
  REGION ?= EU
  PLATFORM := GC
  DEBUG_FEATURES ?= 1
else ifeq ($(VERSION),gc-eu-dbg)
  REGION ?= EU
  PLATFORM := GC
  DEBUG_FEATURES ?= 1
else ifeq ($(VERSION),gc-eu)
  REGION ?= EU
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-eu-mq)
  REGION ?= EU
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),gc-jp-ce)
  REGION ?= JP
  PLATFORM := GC
  DEBUG_FEATURES ?= 0
else ifeq ($(VERSION),ique-cn)
  REGION ?= US
  PLATFORM := IQUE
  DEBUG_FEATURES ?= 0
else
$(error Unsupported version $(VERSION))
endif

CPP_DEFINES += -DNON_MATCHING -DAVOID_UB

BUILD_DIR := build/$(VERSION)
BASEROM_DIR := baseroms/$(VERSION)
EXTRACTED_DIR := extracted/$(VERSION)
VENV := .venv

MAKE = make

ifeq ($(PLATFORM),N64)
  CPP_DEFINES += -DPLATFORM_N64=1 -DPLATFORM_GC=0 -DPLATFORM_IQUE=0
else ifeq ($(PLATFORM),GC)
  CPP_DEFINES += -DPLATFORM_N64=0 -DPLATFORM_GC=1 -DPLATFORM_IQUE=0
else ifeq ($(PLATFORM),IQUE)
  CPP_DEFINES += -DPLATFORM_N64=0 -DPLATFORM_GC=0 -DPLATFORM_IQUE=1
else
$(error Unsupported platform $(PLATFORM))
endif

export N64_INST := libdragon_inst
$(info Using N64_INST = $(N64_INST))
ifeq ($(wildcard $(N64_INST)/include/n64.mk),)
  $(error run ./build_libdragon.sh)
endif

include $(N64_INST)/include/n64.mk

# TODO-ootdragon we nop N64_ELFCOMPRESS because it strips the elf, yet we want to keep information such as the section names table
N64_ELFCOMPRESS := @:

N64_MKDFS_ROOT := $(BUILD_DIR)/dfs

# Converts e.g. ntsc-1.0 to NTSC_1_0
VERSION_MACRO := $(shell echo $(VERSION) | tr a-z-. A-Z__)
CPP_DEFINES += -DOOT_VERSION=$(VERSION_MACRO)
CPP_DEFINES += -DOOT_REGION=REGION_$(REGION)
ifeq ($(PLATFORM),IQUE)
  CPP_DEFINES += -DBBPLAYER
endif

ifeq ($(DEBUG_FEATURES),1)
  CPP_DEFINES += -DDEBUG_FEATURES=1
else
  CPP_DEFINES += -DDEBUG_FEATURES=0 -DNDEBUG
endif

INC := -Iinclude -Iinclude/ootdragon -Isrc -I$(BUILD_DIR) -I. -I$(EXTRACTED_DIR)

CPP            := $(N64_GCCPREFIX_TRIPLET)cpp
MKDMADATA      := tools/mkdmadata
BIN2C          := tools/bin2c
ARMIPS         := tools/armips
PYTHON         ?= $(VENV)/bin/python3
BUILD_FROM_PNG := tools/assets/build_from_png/build_from_png
BUILD_JFIF     := tools/assets/build_jfif/build_jfif

# Command to replace $(BUILD_DIR) in some files with the build path.
# We can't use the C preprocessor for this because it won't substitute inside string literals.
BUILD_DIR_REPLACE := sed -e 's|$$(BUILD_DIR)|$(BUILD_DIR)|g'

# Audio tools
SAMPLECONV    := tools/audio/sampleconv/sampleconv
SBC           := tools/audio/sbc
SFC           := tools/audio/sfc
SFPATCH       := tools/audio/sfpatch
ATBLGEN       := tools/audio/atblgen
AFILE_SIZES   := tools/audio/afile_sizes
# We want linemarkers in sequence assembly files for better assembler error messages
SEQ_CPP       := $(CPP) -x assembler-with-cpp -fno-dollars-in-identifiers
SEQ_CPPFLAGS  := -D_LANGUAGE_ASEQ -DMML_VERSION=MML_VERSION_OOT $(CPP_DEFINES) -I include -I include/audio -I include/tables/sfx -I $(BUILD_DIR)/assets/audio/soundfonts

SBCFLAGS := --matching
SFCFLAGS := --matching

GBI_DEFINES := -DF3DEX_GBI_2
ifneq ($(PLATFORM),N64)
  GBI_DEFINES += -DF3DEX_GBI_PL -DGBI_DOWHILE
endif
ifeq ($(DEBUG_FEATURES),1)
  GBI_DEFINES += -DGBI_DEBUG
endif

CPPFLAGS += -P -xc -fno-dollars-in-identifiers $(CPP_DEFINES) $(GBI_DEFINES)

# ROM image
ROM      := oot-$(VERSION).z64
ELF      := $(BUILD_DIR)/$(ROM:.z64=.elf)

# description of ROM segments
SPEC := spec/spec
SPEC_INCLUDES := $(wildcard spec/*.inc)

SRC_DIRS := $(shell find src -type d)
RSP_DIRS := $(shell find rsp -type d)
UNDECOMPILED_DATA_DIRS := $(shell find data -type d)

ifneq ($(wildcard $(EXTRACTED_DIR)/assets/audio),)
  SAMPLE_EXTRACT_DIRS := $(shell find $(EXTRACTED_DIR)/assets/audio/samples -type d)
  SAMPLEBANK_EXTRACT_DIRS := $(shell find $(EXTRACTED_DIR)/assets/audio/samplebanks -type d)
  SOUNDFONT_EXTRACT_DIRS := $(shell find $(EXTRACTED_DIR)/assets/audio/soundfonts -type d)
  SEQUENCE_EXTRACT_DIRS := $(shell find $(EXTRACTED_DIR)/assets/audio/sequences -type d)
else
  SAMPLE_EXTRACT_DIRS :=
  SAMPLEBANK_EXTRACT_DIRS :=
  SOUNDFONT_EXTRACT_DIRS :=
  SEQUENCE_EXTRACT_DIRS :=
endif

ifneq ($(wildcard assets/audio/samples),)
  SAMPLE_DIRS := $(shell find assets/audio/samples -type d)
else
  SAMPLE_DIRS :=
endif

ifneq ($(wildcard assets/audio/samplebanks),)
  SAMPLEBANK_DIRS := $(shell find assets/audio/samplebanks -type d)
else
  SAMPLEBANK_DIRS :=
endif

ifneq ($(wildcard assets/audio/soundfonts),)
  SOUNDFONT_DIRS := $(shell find assets/audio/soundfonts -type d)
else
  SOUNDFONT_DIRS :=
endif

ifneq ($(wildcard assets/audio/sequences),)
  SEQUENCE_DIRS := $(shell find assets/audio/sequences -type d)
else
  SEQUENCE_DIRS :=
endif

SAMPLE_FILES         := $(foreach dir,$(SAMPLE_DIRS),$(wildcard $(dir)/*.wav))
SAMPLE_EXTRACT_FILES := $(foreach dir,$(SAMPLE_EXTRACT_DIRS),$(wildcard $(dir)/*.wav))
AIFC_FILES           := $(foreach f,$(SAMPLE_FILES),$(BUILD_DIR)/$(f:.wav=.aifc)) $(foreach f,$(SAMPLE_EXTRACT_FILES:.wav=.aifc),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%))

SAMPLEBANK_XMLS         := $(foreach dir,$(SAMPLEBANK_DIRS),$(wildcard $(dir)/*.xml))
SAMPLEBANK_EXTRACT_XMLS := $(foreach dir,$(SAMPLEBANK_EXTRACT_DIRS),$(wildcard $(dir)/*.xml))
SAMPLEBANK_BUILD_XMLS   := $(foreach f,$(SAMPLEBANK_XMLS),$(BUILD_DIR)/$f) $(foreach f,$(SAMPLEBANK_EXTRACT_XMLS),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%))
SAMPLEBANK_O_FILES      := $(foreach f,$(SAMPLEBANK_BUILD_XMLS),$(f:.xml=.o))
SAMPLEBANK_DEP_FILES    := $(foreach f,$(SAMPLEBANK_O_FILES),$(f:.o=.d))

SOUNDFONT_XMLS         := $(foreach dir,$(SOUNDFONT_DIRS),$(wildcard $(dir)/*.xml))
SOUNDFONT_EXTRACT_XMLS := $(foreach dir,$(SOUNDFONT_EXTRACT_DIRS),$(wildcard $(dir)/*.xml))
SOUNDFONT_BUILD_XMLS   := $(foreach f,$(SOUNDFONT_XMLS),$(BUILD_DIR)/$f) $(foreach f,$(SOUNDFONT_EXTRACT_XMLS),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%))
SOUNDFONT_O_FILES      := $(foreach f,$(SOUNDFONT_BUILD_XMLS),$(f:.xml=.o))
SOUNDFONT_HEADERS      := $(foreach f,$(SOUNDFONT_BUILD_XMLS),$(f:.xml=.h))
SOUNDFONT_DEP_FILES    := $(foreach f,$(SOUNDFONT_O_FILES),$(f:.o=.c.d))

SEQUENCE_FILES         := $(foreach dir,$(SEQUENCE_DIRS),$(wildcard $(dir)/*.seq))
SEQUENCE_EXTRACT_FILES := $(foreach dir,$(SEQUENCE_EXTRACT_DIRS),$(wildcard $(dir)/*.seq))
SEQUENCE_O_FILES       := $(foreach f,$(SEQUENCE_FILES),$(BUILD_DIR)/$(f:.seq=.o)) $(foreach f,$(SEQUENCE_EXTRACT_FILES:.seq=.o),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%))
SEQUENCE_DEP_FILES     := $(foreach f,$(SEQUENCE_O_FILES),$(f:.o=.d))

SEQUENCE_TABLE := include/tables/sequence_table.h

# create extracted directory
$(shell mkdir -p $(EXTRACTED_DIR))

ifneq ($(wildcard $(EXTRACTED_DIR)/assets),)
  ASSET_BIN_DIRS_EXTRACTED := $(shell find $(EXTRACTED_DIR)/assets -type d)
else
  ASSET_BIN_DIRS_EXTRACTED :=
endif
ASSET_BIN_DIRS_COMMITTED := $(shell find assets -type d -not -path "assets/xml*" -not -path "assets/audio*" -not -path assets/text)
ASSET_BIN_DIRS := $(ASSET_BIN_DIRS_EXTRACTED) $(ASSET_BIN_DIRS_COMMITTED)

ASSET_FILES_BIN_EXTRACTED := $(foreach dir,$(ASSET_BIN_DIRS_EXTRACTED),$(wildcard $(dir)/*.bin))
ASSET_FILES_BIN_COMMITTED := $(foreach dir,$(ASSET_BIN_DIRS_COMMITTED),$(wildcard $(dir)/*.bin))
ASSET_FILES_OUT := $(foreach f,$(ASSET_FILES_BIN_EXTRACTED:.bin=.bin.inc.c),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%)) \
                   $(foreach f,$(ASSET_FILES_BIN_COMMITTED:.bin=.bin.inc.c),$(BUILD_DIR)/$f)

TEXTURE_FILES_PNG_EXTRACTED := $(foreach dir,$(ASSET_BIN_DIRS_EXTRACTED),$(wildcard $(dir)/*.png))
TEXTURE_FILES_PNG_COMMITTED := $(foreach dir,$(ASSET_BIN_DIRS_COMMITTED),$(wildcard $(dir)/*.png))
TEXTURE_FILES_JPG_EXTRACTED := $(foreach dir,$(ASSET_BIN_DIRS_EXTRACTED),$(wildcard $(dir)/*.jpg))
TEXTURE_FILES_JPG_COMMITTED := $(foreach dir,$(ASSET_BIN_DIRS_COMMITTED),$(wildcard $(dir)/*.jpg))
TEXTURE_FILES_OUT := $(foreach f,$(TEXTURE_FILES_PNG_EXTRACTED:.png=.inc.c),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%)) \
                     $(foreach f,$(TEXTURE_FILES_PNG_COMMITTED:.png=.inc.c),$(BUILD_DIR)/$f) \
                     $(foreach f,$(TEXTURE_FILES_JPG_EXTRACTED:.jpg=.jpg.inc.c),$(f:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%)) \
                     $(foreach f,$(TEXTURE_FILES_JPG_COMMITTED:.jpg=.jpg.inc.c),$(BUILD_DIR)/$f)

SEGMENTS_DIR := $(BUILD_DIR)/segments

# create build directories
$(shell mkdir -p $(BUILD_DIR)/baserom \
                 $(BUILD_DIR)/assets/text \
                 $(BUILD_DIR)/linker_scripts \
				 $(SEGMENTS_DIR))
$(shell mkdir -p $(foreach dir, \
                      $(SRC_DIRS) \
                      $(RSP_DIRS) \
                      $(UNDECOMPILED_DATA_DIRS) \
                      $(SAMPLE_DIRS) \
                      $(SAMPLEBANK_DIRS) \
                      $(SOUNDFONT_DIRS) \
                      $(SEQUENCE_DIRS) \
                      $(ASSET_BIN_DIRS_COMMITTED), \
                    $(BUILD_DIR)/$(dir)))
ifneq ($(wildcard $(EXTRACTED_DIR)/assets),)
$(shell mkdir -p $(foreach dir, \
                      $(SAMPLE_EXTRACT_DIRS) \
                      $(SAMPLEBANK_EXTRACT_DIRS) \
                      $(SOUNDFONT_EXTRACT_DIRS) \
                      $(SEQUENCE_EXTRACT_DIRS) \
                      $(ASSET_BIN_DIRS_EXTRACTED), \
                    $(dir:$(EXTRACTED_DIR)/%=$(BUILD_DIR)/%)))
endif

$(BUILD_DIR)/src/audio/internal/seqplayer.o: N64_C_AND_CXX_FLAGS += -DMML_VERSION=MML_VERSION_OOT

# Note that if adding additional assets directories for modding reasons these flags must also be used there
$(BUILD_DIR)/assets/%.o: CFLAGS += -fno-zero-initialized-in-bss -fno-toplevel-reorder
$(BUILD_DIR)/src/overlays/%.o: CFLAGS += -mno-explicit-relocs -mno-split-addresses

#### Main Targets ###

clean:
	$(RM) -r $(BUILD_DIR)

assetclean:
	$(RM) -r $(EXTRACTED_DIR)

distclean:
	$(RM) -r extracted/
	$(RM) -r build/
	$(MAKE) -C tools distclean

venv:
# Create the virtual environment if it doesn't exist.
# Delete the virtual environment directory if creation fails.
	test -d $(VENV) || python3 -m venv $(VENV) || { rm -rf $(VENV); false; }
	$(PYTHON) -m pip install -U pip
	$(PYTHON) -m pip install -U -r requirements.txt

setup: venv
	$(MAKE) -C tools
	$(PYTHON) tools/decompress_baserom.py $(VERSION)
	$(PYTHON) tools/extract_baserom.py $(BASEROM_DIR)/baserom-decompressed.z64 $(EXTRACTED_DIR)/baserom -v $(VERSION)
	$(PYTHON) -m tools.assets.extract $(EXTRACTED_DIR)/baserom $(EXTRACTED_DIR) -v $(VERSION) -j$(N_THREADS)
	$(PYTHON) tools/extract_incbins.py $(EXTRACTED_DIR)/baserom $(EXTRACTED_DIR)/incbin -v $(VERSION)
	$(PYTHON) tools/extract_text.py $(EXTRACTED_DIR)/baserom $(EXTRACTED_DIR)/text -v $(VERSION)
	$(PYTHON) tools/extract_audio.py -b $(EXTRACTED_DIR)/baserom -o $(EXTRACTED_DIR) -v $(VERSION) --read-xml

.PHONY: clean assetclean distclean venv setup
.DEFAULT_GOAL := $(ROM)

$(BUILD_DIR)/spec: $(SPEC) $(SPEC_INCLUDES)
	$(CPP) $(CPPFLAGS) -MD -MP -MF $@.d -MT $@ -I. $< | $(BUILD_DIR_REPLACE) > $@

$(BUILD_DIR)/undefined_syms.txt: undefined_syms.txt
	$(CPP) $(CPPFLAGS) $< > $(BUILD_DIR)/undefined_syms.txt

ifeq ($(PLATFORM),IQUE)
  NES_CHARMAP := assets/text/charmap.chn.txt
else
  NES_CHARMAP := assets/text/charmap.nes.txt
endif

$(BUILD_DIR)/assets/text/%.enc.nes.h: assets/text/%.h $(EXTRACTED_DIR)/text/%.h $(NES_CHARMAP)
	$(CPP) $(CPPFLAGS) -I$(EXTRACTED_DIR) -MD -MP -MF $(@:.o=.d) -MT $@ $< | $(PYTHON) tools/msgenc.py --encoding utf-8 --charmap $(NES_CHARMAP) - $@

$(BUILD_DIR)/assets/text/%.enc.jpn.h: assets/text/%.h $(EXTRACTED_DIR)/text/%.h assets/text/charmap.jpn.txt
	$(CPP) $(CPPFLAGS) -I$(EXTRACTED_DIR) -MD -MP -MF $(@:.o=.d) -MT $@ $< | $(PYTHON) tools/msgenc.py --encoding SHIFT-JIS --wchar --charmap assets/text/charmap.jpn.txt - $@

# Dependencies for encoded message headers. These dependencies are not automatic as these headers are generated
# as part of the build. A clean build must know to generate them before the relevant .d files are created.
$(BUILD_DIR)/assets/text/jpn_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.jpn.h
$(BUILD_DIR)/assets/text/nes_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.nes.h
$(BUILD_DIR)/assets/text/ger_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.nes.h
$(BUILD_DIR)/assets/text/fra_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.nes.h
$(BUILD_DIR)/assets/text/staff_message_data_static.o: $(BUILD_DIR)/assets/text/message_data_staff.enc.nes.h

N64_C_AND_CXX_FLAGS += $(CPP_DEFINES) $(GBI_DEFINES) $(INC)

N64_C_AND_CXX_FLAGS += -Wno-error=maybe-uninitialized
N64_C_AND_CXX_FLAGS += -Wno-missing-braces
N64_C_AND_CXX_FLAGS += -Wno-error=strict-aliasing -Wno-error=format=
N64_C_AND_CXX_FLAGS += -Wno-unknown-pragmas # #pragma increment_block_number
N64_C_AND_CXX_FLAGS += -Wno-error=array-bounds= # (at least) OS_K0_TO_PHYSICAL
N64_C_AND_CXX_FLAGS += -Wno-error=address -Wno-error=return-type -Wno-error=switch-unreachable
N64_C_AND_CXX_FLAGS += -Wno-unused-variable
N64_C_AND_CXX_FLAGS += -Wno-char-subscripts
N64_C_AND_CXX_FLAGS += -Wno-error=unused-value
N64_C_AND_CXX_FLAGS += -Wno-error=trigraphs
N64_C_AND_CXX_FLAGS += -Wno-error=format-extra-args
N64_C_AND_CXX_FLAGS += -Wno-format

# TODO enable small data
N64_C_AND_CXX_FLAGS += -G 0

# TODO-ootdragon figure out audio
N64_C_AND_CXX_FLAGS += -D STUB_AUDIO

code_SRCS := $(shell find src data \( -path src/elf_message -o -path src/overlays \) -prune -o \( \( -name '*.c' -not -name '*.inc.c' \) -o -name '*.[sS]' \) -print)
exclude_code_SRCS :=
exclude_code_SRCS += src/code/fault_n64.c  # keep the fault_gc.c one
exclude_code_SRCS += src/boot/is_debug_ique.c  # keep the is_debug.c one
exclude_code_SRCS += src/libc64/__osMalloc_n64.c  # keep the __osMalloc_gc.c one
exclude_code_SRCS += src/libu64/loadfragment2_n64.c  # keep the load_gc.c one and co
exclude_code_SRCS += data/gspF3DZEX2.NoN.fifo.s  # we use gspF3DZEX2_NoN_PosLight_fifo
code_SRCS := $(filter-out $(exclude_code_SRCS),$(code_SRCS))
code_OBJS := $(addprefix $(BUILD_DIR)/,$(patsubst %.S,%.o,$(patsubst %.s,%.o,$(code_SRCS:.c=.o))))
code_OBJS += $(BUILD_DIR)/rsp/rspboot.o  #

ifeq ($(wildcard $(EXTRACTED_DIR)/assets),)
assets_SRCS :=
else
# TODO-ootdragon not sure where to put src/elf_message. Here for now
assets_SRCS := $(shell find assets $(EXTRACTED_DIR)/assets src/elf_message \( -name '*.c' -not -name '*.inc.c' \))
endif
exclude_assets_SRCS :=
exclude_assets_SRCS += assets/textures/icon_item_jpn_static/icon_item_jpn_static.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/gameplay_keep_ntsc_0x46A40.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/gameplay_keep_ntsc_0x39850.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/gameplay_keep_ntsc_0x3FD80.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/unused_bomb_icon_model_ntsc.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/gameplay_keep_ntsc_0xC880.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/unused_heart_container_icon_model_ntsc.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/unused_arrow_icon_model_ntsc.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/unused_heart_icon_model_ntsc.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/gameplay_keep_ntsc_0x3C610.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/unused_heart_piece_icon_tex_ntsc.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/gameplay_keep_ntsc_0xCE60.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += assets/objects/gameplay_keep/item_drops_ntsc.c  # we're building gc-eu-mq-dbg for now
exclude_assets_SRCS += extracted/gc-eu-mq-dbg/assets/overlays/%  # included by overlays
exclude_assets_SRCS += extracted/gc-eu-mq-dbg/assets/textures/icon_item_static/icon_item_static.c  # workaround for this .o being passed twice to ld, causing it to be linked twice and causing duplicate syms errors
assets_SRCS := $(filter-out $(exclude_assets_SRCS),$(assets_SRCS))
assets_OBJS := $(addprefix $(BUILD_DIR)/,$(patsubst $(EXTRACTED_DIR)/%,%,$(assets_SRCS:.c=.o)))

others_SRCS := $(shell find src/overlays -not -name '*.inc.c' -name '*.c')
others_OBJS := $(others_SRCS:.c=.o)

assets_INCC := $(ASSET_FILES_OUT) $(TEXTURE_FILES_OUT)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	$(CC) -c $(CFLAGS) -o $@.tmp $<
# including libdragon.h brings RESOLUTION_256x240 and co as const data...
	$(N64_OBJCOPY) --remove-section='.rodata.RESOLUTION_*' $@.tmp $@

$(BUILD_DIR)/%.o: $(EXTRACTED_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	$(CC) -c $(CFLAGS) -o $@.tmp $<
# including libdragon.h brings RESOLUTION_256x240 and co as const data...
	$(N64_OBJCOPY) --remove-section='.rodata.RESOLUTION_*' $@.tmp $@

DSOS :=
ifeq ($(wildcard dsos.mk),)
  $(error Run tools/mkdsosmk.py to generate dsos.mk)
endif
include dsos.mk

DFS_FILES := $(DSOS)

assets_incc: $(assets_INCC)
$(code_OBJS) $(assets_OBJS) $(others_OBJS): | assets_incc

$(assets_OBJS): N64_C_AND_CXX_FLAGS := $(filter-out -g,$(N64_C_AND_CXX_FLAGS))

# like n64.mk's .S rule but lowercase .s and add -x assembler-with-cpp and -Wa,-I flags
$(BUILD_DIR)/src/%.o: src/%.s
	@mkdir -p $(dir $@)
	echo "    [AS] $<"
	$(CC) -c -x assembler-with-cpp $(INC) $(foreach i,$(INC),-Wa,$(i)) $(ASFLAGS) -o $@ $<
$(BUILD_DIR)/data/%.o: data/%.s
	@mkdir -p $(dir $@)
	echo "    [AS] $<"
	$(CC) -c -x assembler-with-cpp $(INC) $(foreach i,$(INC),-Wa,$(i)) $(ASFLAGS) -o $@ $<

$(ELF): $(ELF:.elf=.externs)
$(ELF:.elf=.externs): $(DSOS)
$(ROM): $(ELF:.elf=.msym)
$(ELF:.elf=.msym): $(ELF) $(ELF:.elf=.externs)

$(ROM): N64_ROM_TITLE = "oot-$(VERSION)"

$(ROM): $(BUILD_DIR)/dfs.dfs
$(BUILD_DIR)/dfs.dfs: $(DFS_FILES)

$(ELF): $(BUILD_DIR)/ldscript.ld $(code_OBJS) $(assets_OBJS) \
        $(SAMPLEBANK_O_FILES) $(SOUNDFONT_O_FILES) $(SEQUENCE_O_FILES) $(BUILD_DIR)/assets/audio/sequence_font_table.o
	@echo "    [LD] $@"
	@$(N64_CXX) -o $@ $(code_OBJS) $(assets_OBJS) -lc -mabi=o64 -T$(BUILD_DIR)/ldscript.ld -Tlinker_scripts/undefined_syms.ld $(patsubst %,-Wl$(COMMA)%,$(filter-out -Tn64.ld,$(LDFLAGS))) -Wl,-T"$(ELF:.elf=.externs)" -Wl,-Map=$(@:.elf=.map)
	$(N64_SIZE) -G $@

$(BUILD_DIR)/ldscript.ld: tools/mkldscript.py assets_list.toml
	@mkdir -p $(dir $@)
	@echo "    [mkldscript] $@"
	python3 tools/mkldscript.py $@

RSP_TEXT_SECTION := .text
RSP_DATA_SECTION := .rodata

.PRECIOUS: $(BUILD_DIR)/rsp/%.s
$(BUILD_DIR)/rsp/%.s: rsp/%.s
	@echo "    [CPP] $@"
	$(CPP) $(CPPFLAGS) -U_LANGUAGE_C -D_LANGUAGE_ASSEMBLY $(GBI_DEFINES) -MMD -MP -MT $@ -I include -I include/ultra64 -I rsp $< -o $@

# Instruct armips to output a symbol map and a processed view of the asm
ARMIPS_FLAGS = -sym2 $(<:.s=.sym) -temp $(<:.s=.tmp.s)

.PRECIOUS: $(BUILD_DIR)/rsp/%.text.bin $(BUILD_DIR)/rsp/%.data.bin
$(BUILD_DIR)/rsp/%.text.bin $(BUILD_DIR)/rsp/%.data.bin: $(BUILD_DIR)/rsp/%.s
# assemble to code and data binaries
	$(ARMIPS) -strequ CODE_FILE $(<:.s=.text.bin) -strequ DATA_FILE $(<:.s=.data.bin) $< $(ARMIPS_FLAGS)
# create an empty file if armips did not error but one of the files was not created
	touch $(<:.s=.text.bin) $(<:.s=.data.bin)

# The default microcode name equals the source file name with dots substituted for underscores.
UC_NAME = $(subst .,_,$(@F:.o=))

# Override the name for the antipiracy rspboot variant to just be "rspboot"
$(BUILD_DIR)/rsp/rspboot_ap.o: UC_NAME = rspboot

RSP2ELF_DEFS =                              \
    -D UC_NAME=$(UC_NAME)                   \
    -D UC_TEXT_SECTION=$(RSP_TEXT_SECTION)  \
    -D UC_DATA_SECTION=$(RSP_DATA_SECTION)  \
    -D UC_TEXT_BIN_PATH="$(@:.o=.text.bin)" \
    -D UC_DATA_BIN_PATH="$(@:.o=.data.bin)"

$(BUILD_DIR)/rsp/%.o: $(BUILD_DIR)/rsp/%.text.bin $(BUILD_DIR)/rsp/%.data.bin rsp/rsp2elf.s
	@echo "    [rsp2elf] $@"
	$(CPP) $(CPPFLAGS) $(RSP2ELF_DEFS) rsp/rsp2elf.s | $(CC) -c -x assembler $(ASFLAGS) -o $@ -

$(BUILD_DIR)/src/audio/game/session_init.o: $(BUILD_DIR)/assets/audio/soundfont_sizes.h $(BUILD_DIR)/assets/audio/sequence_sizes.h

# Assets from assets/

$(BUILD_DIR)/assets/%.inc.c: assets/%.png
	@echo "    [png] $@"
	$(BUILD_FROM_PNG) $< $(dir $@) assets/$(dir $*) $(wildcard $(EXTRACTED_DIR)/assets/$(dir $*))

$(BUILD_DIR)/assets/%.bin.inc.c: assets/%.bin
	@echo "    [bin] $@"
	$(BIN2C) -t 1 $< $@

$(BUILD_DIR)/assets/%.jpg.inc.c: assets/%.jpg
	@echo "    [jpg] $@"
	$(BUILD_JFIF) $< $@

# Assets from extracted/

$(BUILD_DIR)/assets/%.inc.c: $(EXTRACTED_DIR)/assets/%.png
	@echo "    [png] $@"
	$(BUILD_FROM_PNG) $< $(dir $@) $(wildcard assets/$(dir $*)) $(EXTRACTED_DIR)/assets/$(dir $*)

$(BUILD_DIR)/assets/%.bin.inc.c: $(EXTRACTED_DIR)/assets/%.bin
	@echo "    [bin] $@"
	$(BIN2C) -t 1 $< $@

$(BUILD_DIR)/assets/%.jpg.inc.c: $(EXTRACTED_DIR)/assets/%.jpg
	@echo "    [jpg] $@"
	$(BUILD_JFIF) $< $@

# Audio

AUDIO_BUILD_DEBUG ?= 0
ifeq ($(AUDIO_BUILD_DEBUG),1)
  # for debugging only, make soundfonts depend on samplebanks so they can be linked against
  $(BUILD_DIR)/assets/audio/soundfonts/%.o: $(SAMPLEBANK_O_FILES)
endif

# first build samples...

.PRECIOUS: $(BUILD_DIR)/assets/audio/samples/%.aifc
.PRECIOUS: $(BUILD_DIR)/assets/audio/samples/%.half.aifc

$(BUILD_DIR)/assets/audio/samples/%.half.aifc: assets/audio/samples/%.half.wav
	@echo "    [sampleconv] $@"
	$(SAMPLECONV) vadpcm-half $< $@

$(BUILD_DIR)/assets/audio/samples/%.half.aifc: $(EXTRACTED_DIR)/assets/audio/samples/%.half.wav
	@echo "    [sampleconv] $@"
	$(SAMPLECONV) vadpcm-half $< $@

$(BUILD_DIR)/assets/audio/samples/%.aifc: assets/audio/samples/%.wav
	@echo "    [sampleconv] $@"
	$(SAMPLECONV) vadpcm $< $@

$(BUILD_DIR)/assets/audio/samples/%.aifc: $(EXTRACTED_DIR)/assets/audio/samples/%.wav
	@echo "    [sampleconv] $@"
	$(SAMPLECONV) vadpcm $< $@

# then assemble the samplebanks...

.PRECIOUS: $(BUILD_DIR)/assets/audio/samplebanks/%.xml

$(BUILD_DIR)/assets/audio/samplebanks/%.xml: assets/audio/samplebanks/%.xml
	@echo "    [samplebank xml] $@"
	cat $< | $(BUILD_DIR_REPLACE) > $@

$(BUILD_DIR)/assets/audio/samplebanks/%.xml: $(EXTRACTED_DIR)/assets/audio/samplebanks/%.xml
	@echo "    [samplebank xml] $@"
	cat $< | $(BUILD_DIR_REPLACE) > $@

.PRECIOUS: $(BUILD_DIR)/assets/audio/samplebanks/%.s
$(BUILD_DIR)/assets/audio/samplebanks/%.s: $(BUILD_DIR)/assets/audio/samplebanks/%.xml | $(AIFC_FILES)
	@echo "    [samplebank .s] $@"
	$(SBC) $(SBCFLAGS) --makedepend $(@:.s=.d) $< $@

-include $(SAMPLEBANK_DEP_FILES)

$(BUILD_DIR)/assets/audio/samplebanks/%.o: $(BUILD_DIR)/assets/audio/samplebanks/%.s
	$(CC) -c $(ASFLAGS) $< -o $@

# also assemble the soundfonts and generate the associated headers...

$(BUILD_DIR)/assets/audio/soundfonts/%.xml: assets/audio/soundfonts/%.xml
	@echo "    [soundfont xml] $@"
	cat $< | $(BUILD_DIR_REPLACE) > $@

$(BUILD_DIR)/assets/audio/soundfonts/%.xml: $(EXTRACTED_DIR)/assets/audio/soundfonts/%.xml
	@echo "    [soundfont xml] $@"
	cat $< | $(BUILD_DIR_REPLACE) > $@

.PRECIOUS: $(BUILD_DIR)/assets/audio/soundfonts/%.c $(BUILD_DIR)/assets/audio/soundfonts/%.h $(BUILD_DIR)/assets/audio/soundfonts/%.name
$(BUILD_DIR)/assets/audio/soundfonts/%.c $(BUILD_DIR)/assets/audio/soundfonts/%.h $(BUILD_DIR)/assets/audio/soundfonts/%.name: $(BUILD_DIR)/assets/audio/soundfonts/%.xml | $(SAMPLEBANK_BUILD_XMLS) $(AIFC_FILES)
# This rule can be triggered for either the .c or .h file, so $@ may refer to either the .c or .h file. A simple
# substitution $(@:.c=.h) will fail ~50% of the time with -j. Instead, don't assume anything about the suffix of $@.
	@echo "    [soundfont .c] $@"
	$(SFC) $(SFCFLAGS) --makedepend $(basename $@).c.d $< $(basename $@).c $(basename $@).h $(basename $@).name

-include $(SOUNDFONT_DEP_FILES)

$(BUILD_DIR)/assets/audio/soundfonts/%.o: $(BUILD_DIR)/assets/audio/soundfonts/%.c $(BUILD_DIR)/assets/audio/soundfonts/%.name
	@echo "    [soundfont] $@"
	$(CPP) $(MIPS_BUILTIN_DEFS) $(CPPFLAGS) -x assembler-with-cpp $(INC) -I$(N64_INCLUDEDIR) -I include/audio -MD -MP -MF $(@:.o=.d) -MT $@ $< -o /dev/null
# compile c to unlinked object
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -I include/audio -o $(@:.o=.tmp) $<
# partial link
	$(LD) -r -T linker_scripts/soundfont.ld $(@:.o=.tmp) -o $(@:.o=.tmp2)
# patch defined symbols to be ABS symbols so that they remain file-relative offsets forever
	$(SFPATCH) $(@:.o=.tmp2) $(@:.o=.tmp2)
# write start and size symbols afterwards, filename != symbolic name so source symbolic name from the .name file written by sfc
# also write a .note.name section containing the symbolic name of the soundfont
	$(N64_OBJCOPY) --add-symbol $$(cat $(<:.c=.name) | tr -d '\0')_Start=.rodata:0,global --redefine-sym __LEN__=$$(cat $(<:.c=.name) | tr -d '\0')_Size --add-section .note.name=$(<:.c=.name) $(@:.o=.tmp2) $@
# cleanup temp files
	@$(RM) $(@:.o=.tmp) $(@:.o=.tmp2)

# then assemble the sequences...

$(BUILD_DIR)/assets/audio/sequences/%.o: assets/audio/sequences/%.seq include/audio/aseq.h $(SEQUENCE_TABLE) | $(SOUNDFONT_HEADERS)
	@echo "    [sequence] $@"
	$(SEQ_CPP) $(SEQ_CPPFLAGS) -MD -MP -MT $@ $< -o $(@:.o=.s)
	$(CC) -c $(ASFLAGS) -I $(BUILD_DIR)/assets/audio/soundfonts -I include/audio -I $(dir $<) $(@:.o=.s) -o $@

$(BUILD_DIR)/assets/audio/sequences/%.o: $(EXTRACTED_DIR)/assets/audio/sequences/%.seq include/audio/aseq.h $(SEQUENCE_TABLE) | $(SOUNDFONT_HEADERS)
	@echo "    [sequence] $@"
	$(SEQ_CPP) $(SEQ_CPPFLAGS) -MD -MP -MT $@ $< -o $(@:.o=.s)
	$(CC) -c $(ASFLAGS) -I $(BUILD_DIR)/assets/audio/soundfonts -I include/audio -I $(dir $<) $(@:.o=.s) -o $@

-include $(SEQUENCE_DEP_FILES)

# put together the tables

$(BUILD_DIR)/assets/audio/samplebank_table.h: $(SAMPLEBANK_BUILD_XMLS)
	@echo "    [atblgen] $@"
	$(ATBLGEN) --banks $@ $^

$(BUILD_DIR)/assets/audio/soundfont_table.h: $(SOUNDFONT_BUILD_XMLS) $(SAMPLEBANK_BUILD_XMLS)
	@echo "    [atblgen] $@"
	$(ATBLGEN) --fonts $@ $(SOUNDFONT_BUILD_XMLS)

SEQ_ORDER_DEFS := -DDEFINE_SEQUENCE_PTR\(name,seqId,_2,_3,_4\)=*\(name,seqId\) \
                  -DDEFINE_SEQUENCE\(name,seqId,_2,_3,_4\)=\(name,seqId\)
$(BUILD_DIR)/assets/audio/sequence_order.in: $(SEQUENCE_TABLE)
	@echo "    [CPP] $@"
	$(CPP) $(CPPFLAGS) $< $(SEQ_ORDER_DEFS) -o $@

$(BUILD_DIR)/assets/audio/sequence_font_table.s: $(BUILD_DIR)/assets/audio/sequence_order.in $(SEQUENCE_O_FILES)
	@echo "    [atblgen] $@"
	$(ATBLGEN) --sequences $@ $^

# build the tables into objects, move data -> rodata

$(BUILD_DIR)/src/audio/tables/samplebank_table.o: src/audio/tables/samplebank_table.c $(BUILD_DIR)/assets/audio/samplebank_table.h
$(BUILD_DIR)/src/audio/tables/soundfont_table.o: src/audio/tables/soundfont_table.c $(BUILD_DIR)/assets/audio/soundfont_table.h $(SOUNDFONT_HEADERS)
$(BUILD_DIR)/src/audio/tables/sequence_table.o: src/audio/tables/sequence_table.c $(SEQUENCE_TABLE)

$(BUILD_DIR)/src/audio/tables/sequence_table.o: CFLAGS += -I include/tables

$(BUILD_DIR)/assets/audio/sequence_font_table.o: $(BUILD_DIR)/assets/audio/sequence_font_table.s
	$(CC) -c $(ASFLAGS) $< -o $@

# make headers with file sizes and amounts

$(BUILD_DIR)/assets/audio/soundfont_sizes.h: $(SOUNDFONT_O_FILES)
	@echo "    [afile_sizes] $@"
	$(AFILE_SIZES) $@ NUM_SOUNDFONTS SOUNDFONT_SIZES .rodata $^

$(BUILD_DIR)/assets/audio/sequence_sizes.h: $(SEQUENCE_O_FILES)
	@echo "    [afile_sizes] $@"
	$(AFILE_SIZES) $@ NUM_SEQUENCES SEQUENCE_SIZES .data $^

-include $(shell find $(BUILD_DIR) -name '*.d')

# Print target for debugging
print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
