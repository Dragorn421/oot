actor and object list for using this monstrosity

## sasatest_room_0.c

```c
    SCENE_CMD_OBJECT_LIST(2, sasatest_room_0ObjectList_00005C),
    SCENE_CMD_ACTOR_LIST(6, sasatest_room_0ActorList_000064),
    SCENE_CMD_END(),
}; 

SCmdBase* sasatest_room_0AlternateHeaders0x000050[] = {
    NULL,
    NULL,
    NULL,
}; 

s16 sasatest_room_0ObjectList_00005C[3] = {
    OBJECT_MIZU_OBJECTS,
    OBJECT_D_HSBLOCK,
    OBJECT_KIBAKO2,
}; 

ActorEntry sasatest_room_0ActorList_000064[] = {
    { 
        ACTOR_BG_MIZU_MOVEBG,
        { 0, 0, 0 },
        {      0,      0,      0 },
        0x0421
    },
    { 
        ACTOR_BG_MIZU_MOVEBG,
        { 0, 0, 0 },
        {      0,      0,      0 },
        0x1421
    },
    { 
        ACTOR_BG_MIZU_MOVEBG,
        { 0, 0, 0 },
        {      0,      0,      0 },
        0x2421
    },
    { 
        ACTOR_BG_MIZU_MOVEBG,
        { 0, 0, 0 },
        {      0,      0,      0 },
        0x3421
    },
    { 
        ACTOR_BG_MIZU_MOVEBG,
        { 0, 0, 0 },
        {      0,      0,      0 },
        0x4421
    },
    { 
        ACTOR_BG_MIZU_MOVEBG,
        { 0, 0, 0 },
        {      0,      0,      0 },
        0x5421
    },
}; 
```

## sasatest_scene.c

```c
SCmdBase* sasatest_sceneAlternateHeaders0x000058[] = {
    NULL,
    NULL,
    NULL,
}; 

ActorEntry sasatest_sceneStartPositionList0x000064[] = {
    { 
        ACTOR_PLAYER,
        {      0,      0,      -300 },
        {      0,      0,      0 },
        0x0FFF
    },
}; 
```
