$(BUILD_DIR)/dfs/actors/ovl_player_actor.dso: $(BUILD_DIR)/src/overlays/actors/ovl_player_actor/z_player.o
DSOS += $(BUILD_DIR)/dfs/actors/ovl_player_actor.dso

$(BUILD_DIR)/dfs/misc/ovl_kaleido_scope.dso: $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_collect.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_debug.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_equipment.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_item.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_map.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_prompt.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_kaleido_scope.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_lmap_mark_data_mq.o $(BUILD_DIR)/src/overlays/misc/ovl_kaleido_scope/z_lmap_mark.o
DSOS += $(BUILD_DIR)/dfs/misc/ovl_kaleido_scope.dso
