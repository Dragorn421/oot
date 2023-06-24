/*
 * ROM spec file
 */

beginseg
    name "makerom"
    include "build/src/makerom/rom_header.o"
    include "build/src/makerom/ipl3.o"
    include "build/src/makerom/entry.o"
endseg

beginseg
    name "boot"
    address 0x80000460
    include "build/src/boot/boot_main.o"
    include "build/data/unk_800093F0.data.o"
    include "build/data/unk_80009410.data.o"
    include "build/src/boot/idle.o"
    include "build/src/boot/viconfig.o"
    include "build/src/boot/z_std_dma.o"
    include "build/src/boot/yaz0.o"
    include "build/src/boot/z_locale.o"
    include "build/src/boot/assert.o"
    include "build/src/boot/is_debug.o"
    include "build/src/libultra/io/driverominit.o"
    include "build/src/boot/mio0.o"
    include "build/src/boot/stackcheck.o"
    include "build/src/boot/logutils.o"
    include "build/src/libultra/rmon/sprintf.o"
    include "build/src/libultra/io/piacs.o"
    include "build/src/libultra/os/sendmesg.o"
    include "build/src/libultra/os/stopthread.o"
    include "build/src/libultra/io/viextendvstart.o"
    include "build/src/libultra/io/vimodepallan1.o"
    include "build/src/libultra/os/recvmesg.o"
    include "build/src/libultra/os/initialize.o"
    include "build/src/libultra/libc/ll.o"
    include "build/src/libultra/os/exceptasm.o"
    include "build/src/libultra/os/thread.o"
    include "build/src/libultra/os/destroythread.o"
    include "build/src/libultra/libc/bzero.o"
    include "build/src/libultra/os/parameters.o"
    include "build/src/libultra/os/createthread.o"
    include "build/src/libultra/os/setsr.o"
    include "build/src/libultra/os/getsr.o"
    include "build/src/libultra/os/writebackdcache.o"
    include "build/src/libultra/io/vigetnextframebuf.o"
    include "build/src/libultra/io/pimgr.o"
    include "build/src/libultra/io/devmgr.o"
    include "build/src/libultra/io/pirawdma.o"
    include "build/src/libultra/os/virtualtophysical.o"
    include "build/src/libultra/io/viblack.o"
    include "build/src/libultra/io/sirawread.o"
    include "build/src/libultra/os/getthreadid.o"
    include "build/src/libultra/os/setintmask.o"
    include "build/src/libultra/io/visetmode.o"
    include "build/src/libultra/os/probetlb.o"
    include "build/src/libultra/os/getmemsize.o"
    include "build/src/libultra/os/seteventmesg.o"
    include "build/src/libultra/rmon/xprintf.o"
    include "build/src/libultra/os/unmaptlball.o"
    include "build/src/libultra/io/epidma.o"
    include "build/src/libultra/libc/string.o"
    include "build/src/libultra/os/invalicache.o"
    include "build/src/libultra/os/createmesgqueue.o"
    include "build/src/libultra/os/invaldcache.o"
    include "build/src/libultra/io/si.o"
    include "build/src/libultra/os/jammesg.o"
    include "build/src/libultra/os/setthreadpri.o"
    include "build/src/libultra/os/getthreadpri.o"
    include "build/src/libultra/io/epirawread.o"
    include "build/src/libultra/io/viswapbuf.o"
    include "build/src/libultra/io/epirawdma.o"
    include "build/src/libultra/libc/bcmp.o"
    include "build/src/libultra/os/gettime.o"
    include "build/src/libultra/os/timerintr.o"
    include "build/src/libultra/os/getcount.o"
    include "build/src/libultra/os/setglobalintmask.o"
    include "build/src/libultra/os/setcompare.o"
    include "build/src/libultra/libc/bcopy.o"
    include "build/src/libultra/os/resetglobalintmask.o"
    include "build/src/libultra/os/interrupt.o"
    include "build/src/libultra/io/vimodentsclan1.o"
    include "build/src/libultra/io/vimodempallan1.o"
    include "build/src/libultra/io/vi.o"
    include "build/src/libultra/io/viswapcontext.o"
    include "build/src/libultra/io/pigetcmdq.o"
    include "build/src/libultra/io/epiread.o"
    include "build/src/libultra/io/visetspecial.o"
    include "build/src/libultra/io/cartrominit.o"
    include "build/src/libultra/io/vimodefpallan1.o"
    include "build/src/libultra/os/setfpccsr.o"
    include "build/src/libultra/os/getfpccsr.o"
    include "build/src/libultra/io/epiwrite.o"
    include "build/src/libultra/os/maptlbrdb.o"
    include "build/src/libultra/os/yieldthread.o"
    include "build/src/libultra/os/getcause.o"
    include "build/src/libultra/io/epirawwrite.o"
    include "build/src/libultra/rmon/xlitob.o"
    include "build/src/libultra/libc/ldiv.o"
    include "build/src/libultra/rmon/xldtob.o"
    include "build/src/boot/build.o"
    include "build/src/libultra/io/sirawwrite.o"
    include "build/src/libultra/io/vimgr.o"
    include "build/src/libultra/io/vigetcurrcontext.o"
    include "build/src/libultra/os/startthread.o"
    include "build/src/libultra/io/visetyscale.o"
    include "build/src/libultra/io/visetxscale.o"
    include "build/src/libultra/os/sethwintrroutine.o"
    include "build/src/libultra/os/gethwintrroutine.o"
    include "build/src/libultra/os/setwatchlo.o"
    include "build/data/rsp_boot.text.o"
#ifdef COMPILER_GCC
    include "build/src/gcc_fix/missing_gcc_functions.o"
#endif
endseg

beginseg
    name "dmadata"
    include "build/src/dmadata/dmadata.o"
endseg

beginseg
    name "Audiobank"
    address 0x10 // fake RAM address to avoid map lookup inaccuracies
    include "build/baserom/Audiobank.o"
endseg

beginseg
    name "Audioseq"
    include "build/baserom/Audioseq.o"
endseg

beginseg
    name "Audiotable"
    include "build/baserom/Audiotable.o"
endseg

beginseg
    name "link_animetion"
    include "build/assets/misc/link_animetion/link_animetion.o"
    number 7
endseg

#include "assets/textures/icon_item_static/spec_fragment"
#include "assets/textures/icon_item_24_static/spec_fragment"
#include "assets/textures/icon_item_field_static/spec_fragment"
#include "assets/textures/icon_item_dungeon_static/spec_fragment"
#include "assets/textures/icon_item_gameover_static/spec_fragment"
#include "assets/textures/icon_item_nes_static/spec_fragment"
#include "assets/textures/icon_item_ger_static/spec_fragment"
#include "assets/textures/icon_item_fra_static/spec_fragment"
#include "assets/textures/item_name_static/spec_fragment"
#include "assets/textures/map_name_static/spec_fragment"
#include "assets/textures/do_action_static/spec_fragment"
#include "assets/textures/message_static/spec_fragment"
#include "assets/textures/message_texture_static/spec_fragment"
#include "assets/textures/nes_font_static/spec_fragment"

beginseg
    name "nes_message_data_static"
    romalign 0x1000
    include "build/assets/text/nes_message_data_static.o"
    number 7
endseg

beginseg
    name "ger_message_data_static"
    romalign 0x1000
    include "build/assets/text/ger_message_data_static.o"
    number 7
endseg

beginseg
    name "fra_message_data_static"
    romalign 0x1000
    include "build/assets/text/fra_message_data_static.o"
    number 7
endseg

beginseg
    name "staff_message_data_static"
    romalign 0x1000
    include "build/assets/text/staff_message_data_static.o"
    number 7
endseg

#include "assets/textures/map_grand_static/spec_fragment"
#include "assets/textures/map_48x85_static/spec_fragment"
#include "assets/textures/map_i_static/spec_fragment"

beginseg
    name "code"
    after "dmadata"
    include "build/src/code/z_en_a_keep.o"
    include "build/src/code/z_en_item00.o"
    include "build/src/code/z_eff_blure.o"
    include "build/src/code/z_eff_shield_particle.o"
    include "build/src/code/z_eff_spark.o"
    include "build/src/code/z_eff_ss_dead.o"
    include "build/src/code/z_effect.o"
    include "build/src/code/z_effect_soft_sprite.o"
    include "build/src/code/z_effect_soft_sprite_old_init.o"
    include "build/src/code/z_effect_soft_sprite_dlftbls.o"
    include "build/src/code/flg_set.o"
    include "build/src/code/z_DLF.o"
    include "build/src/code/z_actor.o"
    include "build/src/code/z_actor_dlftbls.o"
    include "build/src/code/z_bgcheck.o"
    include "build/src/code/code_800430A0.o"
    include "build/src/code/code_80043480.o"
    include "build/src/code/z_camera.o"
    include "build/src/code/z_collision_btltbls.o"
    include "build/src/code/z_collision_check.o"
    include "build/src/code/z_common_data.o"
    include "build/src/code/z_debug.o"
    include "build/src/code/z_debug_display.o"
    include "build/src/code/z_demo.o"
    include "build/src/code/code_80069420.o"
    include "build/src/code/z_draw.o"
    include "build/src/code/z_sfx_source.o"
    include "build/src/code/z_elf_message.o"
    include "build/src/code/z_face_reaction.o"
    include "build/src/code/code_8006C3A0.o"
    include "build/src/code/z_fcurve_data.o"
    include "build/src/code/z_fcurve_data_skelanime.o"
    include "build/src/code/z_game_dlftbls.o"
    include "build/src/code/z_horse.o"
    include "build/src/code/z_jpeg.o"
    include "build/src/code/z_kaleido_setup.o"
    include "build/src/code/z_kanfont.o"
    include "build/src/code/z_kankyo.o"
    include "build/src/code/z_lib.o"
    include "build/src/code/z_lifemeter.o"
    include "build/src/code/z_lights.o"
    include "build/src/code/z_malloc.o"
    include "build/src/code/z_map_mark.o"
    include "build/src/code/z_moji.o"
    include "build/src/code/z_prenmi_buff.o"
    include "build/src/code/z_nulltask.o"
    include "build/src/code/z_olib.o"
    include "build/src/code/z_onepointdemo.o"
    include "build/src/code/z_map_exp.o"
    include "build/src/code/z_map_data.o"
    include "build/src/code/z_parameter.o"
    include "build/src/code/z_path.o"
    include "build/src/code/z_frame_advance.o"
    include "build/src/code/z_player_lib.o"
    include "build/src/code/z_prenmi.o"
    include "build/src/code/z_quake.o"
    include "build/src/code/z_rcp.o"
    include "build/src/code/z_room.o"
    include "build/src/code/z_sample.o"
    include "build/src/code/code_80097A00.o"
    include "build/src/code/z_scene.o"
    include "build/src/code/object_table.o"
    include "build/src/code/z_scene_table.o"
    include "build/src/code/z_skelanime.o"
    include "build/src/code/z_skin.o"
    include "build/src/code/z_skin_awb.o"
    include "build/src/code/z_skin_matrix.o"
    include "build/src/code/z_sram.o"
    include "build/src/code/z_ss_sram.o"
    include "build/src/code/z_rumble.o"
    include "build/data/z_text.data.o"
    include "build/data/unk_8012ABC0.data.o"
    include "build/src/code/z_view.o"
    include "build/src/code/z_vimode.o"
    include "build/src/code/code_800ACE70.o"
    include "build/src/code/z_vismono.o"
    include "build/src/code/code_800AD920.o"
    include "build/src/code/z_vr_box.o"
    include "build/src/code/z_vr_box_draw.o"
    include "build/src/code/z_player_call.o"
    include "build/src/code/z_fbdemo.o"
    include "build/src/code/z_fbdemo_triforce.o"
    include "build/src/code/z_fbdemo_wipe1.o"
    include "build/src/code/z_fbdemo_circle.o"
    include "build/src/code/z_fbdemo_fade.o"
    include "build/src/code/shrink_window.o"
    include "build/src/code/db_camera.o"
    include "build/src/code/code_800BB0A0.o"
    include "build/src/code/mempak.o"
    include "build/src/code/z_kaleido_manager.o"
    include "build/src/code/z_kaleido_scope_call.o"
    include "build/src/code/z_play.o"
    include "build/src/code/PreRender.o"
    include "build/src/code/TwoHeadGfxArena.o"
    include "build/src/code/TwoHeadArena.o"
    include "build/src/code/code_800C3C20.o"
    include "build/src/code/audioMgr.o"
    include "build/src/code/title_setup.o"
    include "build/src/code/game.o"
    include "build/src/code/gamealloc.o"
    include "build/src/code/graph.o"
    include "build/src/code/listalloc.o"
    include "build/src/code/main.o"
    include "build/src/code/padmgr.o"
    include "build/src/code/sched.o"
    include "build/src/code/speed_meter.o"
    include "build/src/code/sys_cfb.o"
    include "build/src/code/sys_math.o"
    include "build/src/code/sys_math3d.o"
    include "build/src/code/sys_math_atan.o"
    include "build/src/code/sys_matrix.o"
    include "build/src/code/sys_ucode.o"
    include "build/src/code/sys_rumble.o"
    include "build/src/code/code_800D31A0.o"
    include "build/src/code/irqmgr.o"
    include "build/src/code/debug_malloc.o"
    include "build/src/code/fault.o"
    include "build/src/code/fault_drawer.o"
#ifndef NON_MATCHING
    include "build/data/fault.bss.o"
    include "build/data/fault_drawer.bss.o"
#endif
    include "build/src/code/kanread.o"
    include "build/src/code/ucode_disas.o"
    pad_text // audio library aligned to 32 bytes?
    include "build/src/code/audio_data.o"
    include "build/src/code/audio_synthesis.o"
    include "build/src/code/audio_heap.o"
    include "build/src/code/audio_load.o"
    include "build/src/code/code_800E4FE0.o"
    include "build/src/code/code_800E6840.o"
    include "build/src/libultra/io/aisetnextbuf.o"
    include "build/src/code/audio_playback.o"
    include "build/src/code/audio_effects.o"
    include "build/src/code/audio_seqplayer.o"
    include "build/src/code/code_800EC960.o"
    include "build/src/code/audio_sfx_params.o"
    include "build/src/code/code_800F7260.o"
    include "build/src/code/code_800F9280.o"
    include "build/src/code/audio_external_data.o"
    include "build/src/code/audio_init_params.o"
    include "build/src/code/logseverity.o"
    include "build/src/code/gfxprint.o"
    include "build/src/code/rcp_utils.o"
    include "build/src/code/loadfragment2.o"
    include "build/src/code/mtxuty-cvt.o"
    include "build/src/code/relocation.o"
    include "build/src/code/load.o"
    include "build/src/code/code_800FC620.o"
    include "build/src/code/padutils.o"
    include "build/src/code/padsetup.o"
    include "build/src/code/code_800FCE80.o"
    include "build/src/code/fp.o"
    include "build/src/code/system_malloc.o"
    include "build/src/code/code_800FD970.o"
    include "build/src/code/__osMalloc.o"
    include "build/src/code/printutils.o"
    include "build/src/code/sleep.o"
    include "build/src/code/jpegutils.o"
    include "build/src/code/jpegdecoder.o"
    include "build/src/libultra/io/pfsfreeblocks.o"
    include "build/src/libultra/mgu/scale.o"
    include "build/src/libultra/gu/sinf.o"
    include "build/src/libultra/gu/sins.o"
    include "build/src/libultra/io/sptask.o"
    include "build/src/libultra/io/motor.o"
    include "build/src/libultra/io/siacs.o"
    include "build/src/libultra/io/controller.o"
    include "build/src/libultra/io/contreaddata.o"
    include "build/src/libultra/gu/perspective.o"
    include "build/src/libultra/io/sprawdma.o"
    include "build/src/libultra/io/sirawdma.o"
    include "build/src/libultra/io/sptaskyield.o"
    include "build/src/libultra/io/pfsreadwritefile.o"
    include "build/src/libultra/io/pfsgetstatus.o"
    include "build/src/libultra/mgu/mtxidentf.o"
    include "build/src/libultra/gu/lookat.o"
    include "build/src/libultra/io/pfsallocatefile.o"
    include "build/src/libultra/os/stoptimer.o"
    include "build/src/libultra/io/contpfs.o"
    include "build/src/libultra/mgu/mtxl2f.o"
    include "build/src/libultra/io/pfsfindfile.o"
    include "build/src/libultra/gu/sqrtf.o"
    include "build/src/libultra/os/afterprenmi.o"
    include "build/src/libultra/io/contquery.o"
    include "build/src/libultra/gu/lookathil.o"
    include "build/src/libultra/io/sp.o"
    include "build/src/libultra/mgu/mtxident.o"
    include "build/src/libultra/gu/position.o"
    include "build/src/libultra/io/sptaskyielded.o"
    include "build/src/libultra/gu/rotate.o"
    include "build/src/libultra/io/aisetfreq.o"
    include "build/src/libultra/os/getactivequeue.o"
    include "build/src/libultra/mgu/normalize.o"
    include "build/src/libultra/io/dpgetstat.o"
    include "build/src/libultra/io/dpsetstat.o"
    include "build/src/libultra/io/pfsdeletefile.o"
    include "build/src/libultra/gu/ortho.o"
    include "build/src/libultra/gu/cosf.o"
    include "build/src/libultra/gu/libm_vals.o"
    include "build/src/libultra/gu/coss.o"
    include "build/src/libultra/io/visetevent.o"
    include "build/src/libultra/io/pfsisplug.o"
    include "build/src/libultra/gu/us2dex.o"
    include "build/src/libultra/io/pfsselectbank.o"
    include "build/src/libultra/io/contsetch.o"
    include "build/src/libultra/io/pfsfilestate.o"
    include "build/src/libultra/io/pfsinitpak.o"
    include "build/src/libultra/io/pfschecker.o"
    include "build/src/libultra/io/aigetlen.o"
    include "build/src/libultra/mgu/translate.o"
    include "build/src/libultra/io/contramwrite.o"
    include "build/src/libultra/io/contramread.o"
    include "build/src/libultra/io/crc.o"
    include "build/src/libultra/os/settimer.o"
    include "build/src/libultra/io/spgetstat.o"
    include "build/src/libultra/io/spsetstat.o"
    include "build/src/libultra/os/writebackdcacheall.o"
    include "build/src/libultra/os/getcurrfaultedthread.o"
    include "build/src/libultra/mgu/mtxf2l.o"
    include "build/src/libultra/libc/llcvt.o"
    include "build/src/libultra/io/vigetcurrframebuf.o"
    include "build/src/libultra/io/spsetpc.o"
    include "build/src/libultra/libc/sqrt.o"
    include "build/src/libultra/libc/absf.o"
    include "build/src/code/fmodf.o"
    include "build/src/code/__osMemset.o"
    include "build/src/code/__osMemmove.o"
    include_data_with_rodata "build/src/code/z_message_PAL.o"
    include "build/src/code/z_game_over.o"
    include "build/src/code/z_construct.o"
    include "build/data/audio_tables.rodata.o"
    include "build/data/rsp.text.o"
    include "build/data/rsp.rodata.o"
endseg

beginseg
    name "buffers"
    align 0x40
    include "build/src/buffers/zbuffer.o"
    include "build/src/buffers/gfxbuffers.o"
    include "build/src/buffers/heaps.o"
endseg

#include "src/overlays/spec_part"

#include "assets/objects/spec_part"
#include "assets/textures/place_title_cards/spec_fragment"

beginseg
    name "z_select_static"
    romalign 0x1000
    include "build/assets/misc/z_select_static/z_select_static.o"
    number 1
endseg

#include "assets/textures/nintendo_rogo_static/spec_fragment"
#include "assets/textures/title_static/spec_fragment"
#include "assets/textures/parameter_static/spec_fragment"
#include "assets/textures/skyboxes/spec_fragment"
#include "assets/textures/backgrounds/spec_fragment"

beginseg
    name "elf_message_field"
    romalign 0x1000
    include "build/src/elf_message/elf_message_field.o"
    number 0
endseg

beginseg
    name "elf_message_ydan"
    romalign 0x1000
    include "build/src/elf_message/elf_message_ydan.o"
    number 0
endseg

#include "assets/scenes/spec_part"

beginseg
    name "bump_texture_static"
    romalign 0x1000
    include "build/baserom/bump_texture_static.o"
endseg

beginseg
    name "anime_model_1_static"
    romalign 0x1000
    include "build/baserom/anime_model_1_static.o"
endseg

beginseg
    name "anime_model_2_static"
    romalign 0x1000
    include "build/baserom/anime_model_2_static.o"
endseg

beginseg
    name "anime_model_3_static"
    romalign 0x1000
    include "build/baserom/anime_model_3_static.o"
endseg

beginseg
    name "anime_model_4_static"
    romalign 0x1000
    include "build/baserom/anime_model_4_static.o"
endseg

beginseg
    name "anime_model_5_static"
    romalign 0x1000
    include "build/baserom/anime_model_5_static.o"
endseg

beginseg
    name "anime_model_6_static"
    romalign 0x1000
    include "build/baserom/anime_model_6_static.o"
endseg

beginseg
    name "anime_texture_1_static"
    romalign 0x1000
    include "build/baserom/anime_texture_1_static.o"
endseg

beginseg
    name "anime_texture_2_static"
    romalign 0x1000
    include "build/baserom/anime_texture_2_static.o"
endseg

beginseg
    name "anime_texture_3_static"
    romalign 0x1000
    include "build/baserom/anime_texture_3_static.o"
endseg

beginseg
    name "anime_texture_4_static"
    romalign 0x1000
    include "build/baserom/anime_texture_4_static.o"
endseg

beginseg
    name "anime_texture_5_static"
    romalign 0x1000
    include "build/baserom/anime_texture_5_static.o"
endseg

beginseg
    name "anime_texture_6_static"
    romalign 0x1000
    include "build/baserom/anime_texture_6_static.o"
endseg

beginseg
    name "softsprite_matrix_static"
    romalign 0x1000
    include "build/baserom/softsprite_matrix_static.o"
endseg
