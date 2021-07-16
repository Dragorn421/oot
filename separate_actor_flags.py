import os
import re

for root, dirs, files in os.walk('src'):
    for file in files:
        if not file.endswith('.c'):
            continue
        path = os.path.join(root, file)
        with open(path) as f:
            contents = f.read()
        idx_ActorInit = contents.find('ActorInit')
        has_ActorInit = not idx_ActorInit < 0
        if has_ActorInit:
            FLAGS_in_ActorInit = contents[idx_ActorInit:].split(',', maxsplit=3)[2].strip() == 'FLAGS'
        # sanity checks
        if has_ActorInit and not FLAGS_in_ActorInit:
            if path not in (
                'src/code/z_actor.c',
            ):
                print(path, 'has_ActorInit and not FLAGS_in_ActorInit')
        if not has_ActorInit and 'FLAGS' in contents:
            if path not in (
                'src/overlays/actors/ovl_kaleido_scope/z_kaleido_map_PAL.c',
                'src/overlays/actors/ovl_kaleido_scope/z_kaleido_scope_PAL.c',
                'src/libultra_boot_O2/_Litob.c',
                'src/libultra_boot_O2/_Printf.c',
                'src/libultra_boot_O2/_Ldtob.c',
            ):
                print(path, "not has_ActorInit and 'FLAGS' in contents")
        if not has_ActorInit or not FLAGS_in_ActorInit:
            has_define_FLAGS = False
        else:
            idx_define_FLAGS = contents.find('#define FLAGS ')
            has_define_FLAGS = not idx_define_FLAGS < 0
            if not has_define_FLAGS:
                print(path, 'not has_define_FLAGS')
        if has_define_FLAGS:
            define_FLAGS_value = contents[idx_define_FLAGS+len('#define FLAGS '):].split('\n', maxsplit=1)[0].strip()
            def get_flags_from_value(flags_val_int):
                flags = []
                flag_idx = 0
                while flags_val_int != 0:
                    if flags_val_int & 1:
                        flags.append(f'ACTOR_FLAG_{flag_idx}')
                    flags_val_int >>= 1
                    flag_idx += 1
                if not flags:
                    flags = ['ACTOR_FLAG_NONE']
                return ' | '.join(flags)
            new_define_FLAGS_value = get_flags_from_value(int(define_FLAGS_value, 16))
            #print(path, define_FLAGS_value, new_define_FLAGS_value)
            contents_before_flags_define = contents[:idx_define_FLAGS]
            contents_after_flags_define = contents[idx_define_FLAGS:].split('\n', maxsplit=1)[1]
            contents = (
                contents_before_flags_define
                + f'#define FLAGS {new_define_FLAGS_value}\n'
                + contents_after_flags_define
            )

        def replace_flags_change(match):
            try:
                def int_dec_or_hex(str):
                    return int(str, 16) if str.startswith('0x') or str.startswith('-0x') else int(str)
                right_assign_val = match.group(3).strip()
                right_assign_val_otherstuff = right_assign_val[-1]
                right_assign_val = right_assign_val[:-1]
                if right_assign_val[0] == '~':
                    new_right_assign_val = get_flags_from_value(int_dec_or_hex(right_assign_val[1:]))
                    if '|' in new_right_assign_val:
                        new_right_assign_val = '(' + new_right_assign_val + ')'
                    new_right_assign_val = '~' + new_right_assign_val
                elif right_assign_val[0] == '-':
                    new_right_assign_val = get_flags_from_value((int_dec_or_hex(right_assign_val) + 0x100000000) ^ 0xFFFFFFFF)
                    if '|' in new_right_assign_val:
                        new_right_assign_val = '(' + new_right_assign_val + ')'
                    new_right_assign_val = '~' + new_right_assign_val
                else:
                    new_right_assign_val = get_flags_from_value(int_dec_or_hex(right_assign_val))
                    if '|' in new_right_assign_val and '=' not in match.group(2):
                        new_right_assign_val = '(' + new_right_assign_val + ')'
                return match.group(1) + ' ' + match.group(2) + ' ' + new_right_assign_val + right_assign_val_otherstuff
            except Exception as e:
                print('skipping replace in', path, e)
                return match.group(0)

        contents = re.sub(r'((?:thisx->|this->actor\.|this->dyna\.actor\.|actor->)flags)\s*((?:|&|\|)=?)([^&|=][^);]+(?:\)|;))', replace_flags_change, contents)
        
        with open(path, 'w') as f:
            f.write(contents)
