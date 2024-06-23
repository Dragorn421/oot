

# Build options can either be changed by modifying the makefile, or by building with 'make SETTING=value'

# Target game version. Currently the following versions are supported:
#   gc-eu-mq       GameCube Europe/PAL Master Quest
#   gc-eu-mq-dbg   GameCube Europe/PAL Master Quest Debug (default)
# The following versions are work-in-progress and not yet matching:
#   gc-eu          GameCube Europe/PAL
VERSION := gc-eu-mq-dbg
# Number of threads to extract and compress with
N_THREADS := $(shell nproc)

CFLAGS ?=
CPPFLAGS ?=
CPP_DEFINES ?=

CPP_DEFINES += -DCOMPILER_GCC
CPP_DEFINES += -DNON_MATCHING -DAVOID_UB

# Version-specific settings
ifeq ($(VERSION),gc-eu)
  DEBUG := 0
else ifeq ($(VERSION),gc-eu-mq)
  DEBUG := 0
  CPP_DEFINES += -DOOT_MQ
else ifeq ($(VERSION),gc-eu-mq-dbg)
  DEBUG := 1
  CPP_DEFINES += -DOOT_MQ
else
$(error Unsupported version $(VERSION))
endif

BUILD_DIR := build/$(VERSION)
BASEROM_DIR := baseroms/$(VERSION)
EXTRACTED_DIR := extracted/$(VERSION)
VENV := .venv

MAKE = make
CPPFLAGS += -P -xc -fno-dollars-in-identifiers

ifeq ($(DEBUG),1)
  CPP_DEFINES += -DOOT_DEBUG=1
  OPTFLAGS := -O2
else
  CPP_DEFINES += -DNDEBUG -DOOT_DEBUG=0
  OPTFLAGS := -O2 -g3
endif

INC := -Iinclude -Iinclude/libc -Isrc -I$(BUILD_DIR) -I. -I$(EXTRACTED_DIR)

# The `cpp` command behaves differently on macOS (it behaves as if
# `-traditional-cpp` was passed) so we use `gcc -E` instead.
CPP        := gcc -E
PYTHON     ?= $(VENV)/bin/python3

CFLAGS += $(CPP_DEFINES)
CPPFLAGS += $(CPP_DEFINES)

CFLAGS += $(INC)

#### Files ####

# ROM image
# due to n64.mk's `%.z64: $(BUILD_DIR)/%.elf` rule, the rom must be at the root and the elf in the build folder.
ROM := oot-$(VERSION).z64
ELF := $(BUILD_DIR)/$(ROM:.z64=.elf)

# Note that if adding additional assets directories for modding reasons these flags must also be used there
$(BUILD_DIR)/assets/%.o: CFLAGS += -fno-zero-initialized-in-bss -fno-toplevel-reorder
$(BUILD_DIR)/src/%.o: CFLAGS += -fexec-charset=euc-jp
$(BUILD_DIR)/src/libultra/libc/ll.o: OPTFLAGS := -Ofast
$(BUILD_DIR)/src/overlays/%.o: CFLAGS += -fno-merge-constants -mno-explicit-relocs -mno-split-addresses

#### Main Targets ###

all: $(ROM)

clean:
	$(RM) -r $(BUILD_DIR)

assetclean:
	$(RM) -r $(ASSET_BIN_DIRS)
	$(RM) -r $(EXTRACTED_DIR)
	$(RM) -r $(BUILD_DIR)/assets
	$(RM) -r .extracted-assets.json

distclean: assetclean
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
	$(PYTHON) tools/extract_baserom.py $(BASEROM_DIR)/baserom-decompressed.z64 --oot-version $(VERSION) -o $(EXTRACTED_DIR)/baserom
	mkdir -p $(EXTRACTED_DIR)/text
	$(PYTHON) tools/msgdis.py --oot-version $(VERSION) --text-out $(EXTRACTED_DIR)/text/message_data.h --staff-text-out $(EXTRACTED_DIR)/text/message_data_staff.h
# TODO: for now, we only extract assets from the Debug ROM
ifeq ($(VERSION),gc-eu-mq-dbg)
	$(PYTHON) extract_assets.py -j$(N_THREADS)
endif

.PHONY: all clean assetclean distclean venv setup

$(BUILD_DIR)/undefined_syms.txt: undefined_syms.txt
	$(CPP) $(CPPFLAGS) $< > $@

$(BUILD_DIR)/baserom/%.o: $(EXTRACTED_DIR)/baserom/%
	$(OBJCOPY) -I binary -O elf32-big $< $@

$(BUILD_DIR)/assets/text/%.enc.h: assets/text/%.h $(EXTRACTED_DIR)/text/%.h assets/text/charmap.txt
	mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -I$(EXTRACTED_DIR) $< | $(PYTHON) tools/msgenc.py - --output $@ --charmap assets/text/charmap.txt

# Dependencies for files including message data headers
# TODO remove when full header dependencies are used.
$(BUILD_DIR)/assets/text/fra_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.h
$(BUILD_DIR)/assets/text/ger_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.h
$(BUILD_DIR)/assets/text/nes_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.h
$(BUILD_DIR)/assets/text/staff_message_data_static.o: $(BUILD_DIR)/assets/text/message_data_staff.enc.h
$(BUILD_DIR)/src/code/z_message_PAL.o: $(BUILD_DIR)/assets/text/message_data.enc.h $(BUILD_DIR)/assets/text/message_data_staff.enc.h

# Dependencies for files including from include/tables/
# TODO remove when full header dependencies are used.
$(BUILD_DIR)/src/code/graph.o: include/tables/gamestate_table.h
$(BUILD_DIR)/src/code/object_table.o: include/tables/object_table.h
$(BUILD_DIR)/src/code/z_actor.o: include/tables/actor_table.h # so uses of ACTOR_ID_MAX update when the table length changes
$(BUILD_DIR)/src/code/z_actor_dlftbls.o: include/tables/actor_table.h
$(BUILD_DIR)/src/code/z_effect_soft_sprite_dlftbls.o: include/tables/effect_ss_table.h
$(BUILD_DIR)/src/code/z_game_dlftbls.o: include/tables/gamestate_table.h
$(BUILD_DIR)/src/code/z_scene_table.o: include/tables/scene_table.h include/tables/entrance_table.h

# for build_assets_jank.sh
$(BUILD_DIR)/%.inc.c: %.png
	mkdir -p $(dir $@)
	tools/ZAPD/ZAPD.out btex -eh -tt $(subst .,,$(suffix $*)) -i $< -o $@


export N64_INST := build/libdragon
$(info Using N64_INST = $(N64_INST))
ifeq ($(wildcard $(N64_INST)/include/n64.mk),)
  $(error run ./build_libdragon.sh)
endif

include $(N64_INST)/include/n64.mk

CFLAGS += -Wno-error=maybe-uninitialized -Wno-error=missing-braces -Wno-error=strict-aliasing -Wno-error=format=
CFLAGS += -Wno-error=unknown-pragmas # #pragma increment_block_number
CFLAGS += -Wno-error=array-bounds= # (at least) OS_K0_TO_PHYSICAL
CFLAGS += -Wno-error=address -Wno-error=return-type -Wno-error=switch-unreachable

code_SRCS = $(shell find src/libultra src/boot src/code -name '*.c' -not -name '*.inc.c')
code_OBJS = $(addprefix $(BUILD_DIR)/,$(code_SRCS:.c=.o))

$(ROM): N64_ROM_TITLE = "oot-$(VERSION)"

$(ELF): $(code_OBJS)

#-include $(wildcard $(BUILD_DIR)/*.d)
