#include "ultra64.h"
#include "global.h"
#include "z64collision_check.h"

void func_8005B280(GraphicsContext* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3) {
    func_8005B2AC(arg0, arg1, arg2, arg3, 255, 0, 0);
}

#pragma GLOBAL_ASM("asm/non_matchings/code/z_collision_check/func_8005B2AC.s")

static Collider sColliderInit = {
    NULL, NULL, NULL, NULL, AT_NONE, AC_NONE, OC1_NONE, OC2_NONE, COL_MATERIAL_HIT3, COLTYPE_MAX,
};

s32 Collider_InitCollider(GlobalContext* globalCtx, Collider* collider) {
    *collider = sColliderInit;
    return 1;
}

s32 Collider_DestroyCollider(GlobalContext* globalCtx, Collider* collider) {
    return 1;
}

s32 Collider_LoadColliderAltWithActor(GlobalContext* globalCtx, Collider* collider, ColliderSrcAltWithActor* src) {
    collider->actor = src->actor;
    collider->atFlags = src->atFlags;
    collider->acFlags = src->acFlags;
    collider->ocFlags1 = src->ocFlags1;
    collider->ocFlags2 = OC2_TYPE_1;
    collider->colType = src->colType;
    return 1;
}

s32 Collider_LoadColliderAlt(GlobalContext* globalCtx, Collider* collider, Actor* actor, ColliderSrcAlt* src) {
    collider->actor = actor;
    collider->colMaterial = src->colMaterial;
    collider->atFlags = src->atFlags;
    collider->acFlags = src->acFlags;
    collider->ocFlags1 = src->ocFlags1;
    collider->ocFlags2 = OC2_TYPE_1;
    collider->colType = src->colType;
    return 1;
}

s32 Collider_LoadCollider(GlobalContext* globalCtx, Collider* collider, Actor* actor, ColliderSrc* src) {
    collider->actor = actor;
    collider->colMaterial = src->colMaterial;
    collider->atFlags = src->atFlags;
    collider->acFlags = src->acFlags;
    collider->ocFlags1 = src->ocFlags1;
    collider->ocFlags2 = src->ocFlags2;
    collider->colType = src->colType;
    return 1;
}

void Collider_ClearColliderATHit(GlobalContext* globalCtx, Collider* collider) {
    collider->at = NULL;
    collider->atFlags &= ~(AT_HIT | AT_BOUNCED);
}

void Collider_ClearColliderACHit(GlobalContext* globalCtx, Collider* collider) {
    collider->ac = NULL;
    collider->acFlags &= ~(AC_HIT | AC_BOUNCED);
}

void Collider_ClearColliderOCHit(GlobalContext* globalCtx, Collider* collider) {
    collider->oc = NULL;
    collider->ocFlags1 &= ~OC1_HIT;
    collider->ocFlags2 &= ~OC2_HIT_PLAYER;
}

static ColliderElementDamageInfoAT sATDmgInfoInit = {
    0,
    HIT_SPECIAL_EFFECT_NONE,
    0,
};

s32 Collider_InitATDmgInfo(GlobalContext* globalCtx, ColliderElementDamageInfoAT* atDmgInfo) {
    *atDmgInfo = sATDmgInfoInit;
    return 1;
}

s32 Collider_DestroyATDmgInfo(GlobalContext* globalCtx, ColliderElementDamageInfoAT* atDmgInfo) {
    return 1;
}

s32 Collider_LoadATDmgInfo(GlobalContext* globalCtx, ColliderElementDamageInfoAT* atDmgInfo,
                           ColliderElementDamageInfoAT* atDmgInfoSrc) {
    atDmgInfo->dmgFlags = atDmgInfoSrc->dmgFlags;
    atDmgInfo->hitSpecialEffect = atDmgInfoSrc->hitSpecialEffect;
    atDmgInfo->damage = atDmgInfoSrc->damage;
    return 1;
}

void Collider_ClearATDmgInfoATHit(GlobalContext* globalCtx, ColliderElementDamageInfoAT* atDmgInfo) {
}

static ColliderElementDamageInfoAC sACDmgInfoInit = {
    0xFFCFFFFF,
    HIT_BACKLASH_NONE,
    0,
    { 0, 0, 0 },
};

s32 Collider_InitACDmgInfo(GlobalContext* globalCtx, ColliderElementDamageInfoAC* acDmgInfo) {
    *acDmgInfo = sACDmgInfoInit;
    return 1;
}

s32 Collider_DestroyACDmgInfo(GlobalContext* globalCtx, ColliderElementDamageInfoAC* acDmgInfo) {
    return 1;
}

s32 Collider_LoadACDmgInfo(GlobalContext* globalCtx, ColliderElementDamageInfoAC* acDmgInfo,
                           ColliderElementDamageInfoACSrc* acDmgInfoSrc) {
    acDmgInfo->dmgFlags = acDmgInfoSrc->dmgFlags;
    acDmgInfo->hitBacklash = acDmgInfoSrc->hitBacklash;
    acDmgInfo->unk_05 = acDmgInfoSrc->unk5;
    return 1;
}

static ColliderElement sElemInit = {
    { 0, HIT_SPECIAL_EFFECT_NONE, 0 },
    {
        0xFFCFFFFF,
        HIT_BACKLASH_NONE,
        0,
        { 0, 0, 0 },
    },
    ELEM_MATERIAL_UNK0,
    ATELEM_NONE,
    ACELEM_NONE,
    OCELEM_NONE,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32 Collider_InitElement(GlobalContext* globalCtx, ColliderElement* elem) {
    *elem = sElemInit;
    Collider_InitATDmgInfo(globalCtx, &elem->atDmgInfo);
    Collider_InitACDmgInfo(globalCtx, &elem->acDmgInfo);
    return 1;
}

s32 Collider_DestroyElement(GlobalContext* globalCtx, ColliderElement* elem) {
    Collider_DestroyATDmgInfo(globalCtx, &elem->atDmgInfo);
    Collider_DestroyACDmgInfo(globalCtx, &elem->acDmgInfo);
    return 1;
}

s32 Collider_LoadElement(GlobalContext* globalCtx, ColliderElement* elem, ColliderElementSrc* elemSrc) {
    elem->elemMaterial = elemSrc->elemMaterial;
    Collider_LoadATDmgInfo(globalCtx, &elem->atDmgInfo, &elemSrc->atDmgInfo);
    Collider_LoadACDmgInfo(globalCtx, &elem->acDmgInfo, &elemSrc->acDmgInfo);
    elem->atElemFlags = elemSrc->atElemFlags;
    elem->acElemFlags = elemSrc->acElemFlags;
    elem->ocElemFlags = elemSrc->ocElemFlags;
    return 1;
}

void Collider_ClearElementATHit(GlobalContext* globalCtx, ColliderElement* elem) {
    elem->atHit = NULL;
    elem->atHitElem = NULL;
    elem->atElemFlags &= ~ATELEM_HIT;
    elem->atElemFlags &= ~ATELEM_DREW_HITMARK;
    Collider_ClearATDmgInfoATHit(globalCtx, &elem->atDmgInfo);
}

void Collider_ClearElementACHit(GlobalContext* globalCtx, ColliderElement* elem) {
    elem->acElemFlags &= ~ACELEM_HIT;
    elem->acElemFlags &= ~ACELEM_DRAW_HITMARK;
    elem->acHit = NULL;
    elem->acHitElem = NULL;
    elem->acDmgInfo.hitPos.x = elem->acDmgInfo.hitPos.y = elem->acDmgInfo.hitPos.z = 0;
}

void Collider_ClearElementOCHit(GlobalContext* globalCtx, ColliderElement* elem) {
    elem->ocElemFlags &= ~OCELEM_HIT;
}

static ColliderSpheresElementShape sSpheresElementShapeInit = {
    {
        { 0, 0, 0 },
        0,
    },
    {
        { 0, 0, 0 },
        0,
    },
    0.0f,
    0,
};

s32 Collider_InitSpheresElementShape(GlobalContext* globalCtx, ColliderSpheresElementShape* shape) {
    *shape = sSpheresElementShapeInit;
    return 1;
}

s32 Collider_DestroySpheresElementShape(GlobalContext* globalCtx, ColliderSpheresElementShape* shape) {
    return 1;
}

s32 Collider_LoadSpheresElementShape(GlobalContext* globalCtx, ColliderSpheresElementShape* shape,
                                     ColliderSpheresElementShapeSrc* shapeSrc) {
    shape->unk14 = shapeSrc->unk0;
    shape->model = shapeSrc->unk2;
    shape->unkRadiusScale = shapeSrc->unkA * 0.01f;
    return 1;
}

s32 Collider_InitSpheresElement(GlobalContext* globalCtx, ColliderSpheresElement* spheresElem) {
    Collider_InitElement(globalCtx, &spheresElem->base);
    Collider_InitSpheresElementShape(globalCtx, &spheresElem->shape);
    return 1;
}

s32 Collider_DestroySpheresElement(GlobalContext* globalCtx, ColliderSpheresElement* spheresElem) {
    Collider_DestroyElement(globalCtx, &spheresElem->base);
    Collider_DestroySpheresElementShape(globalCtx, &spheresElem->shape);
    return 1;
}

s32 Collider_LoadSpheresElement(GlobalContext* globalCtx, ColliderSpheresElement* spheresElem,
                                ColliderSpheresElementSrc* spheresElemSrc) {
    Collider_LoadElement(globalCtx, &spheresElem->base, &spheresElemSrc->base);
    Collider_LoadSpheresElementShape(globalCtx, &spheresElem->shape, &spheresElemSrc->shape);
    return 1;
}

s32 Collider_ClearSpheresElementATHit(GlobalContext* globalCtx, ColliderSpheresElement* spheresElem) {
    Collider_ClearElementATHit(globalCtx, &spheresElem->base);
    return 1;
}

s32 Collider_ClearSpheresElementACHit(GlobalContext* globalCtx, ColliderSpheresElement* spheresElem) {
    Collider_ClearElementACHit(globalCtx, &spheresElem->base);
    return 1;
}

s32 Collider_ClearSpheresElementOCHit(GlobalContext* globalCtx, ColliderSpheresElement* spheresElem) {
    Collider_ClearElementOCHit(globalCtx, &spheresElem->base);
    return 1;
}

s32 Collider_InitSpheres(GlobalContext* globalCtx, ColliderSpheres* spheres) {
    Collider_InitCollider(globalCtx, &spheres->base);
    spheres->nElements = 0;
    spheres->elements = NULL;
    return 1;
}

// only used by EnNwc
s32 Collider_DestroySpheresMalloc(GlobalContext* globalCtx, ColliderSpheres* spheres) {
    ColliderSpheresElement* spheresElem;

    Collider_DestroyCollider(globalCtx, &spheres->base);
    for (spheresElem = spheres->elements; spheresElem < (spheres->elements + spheres->nElements); spheresElem++) {
        Collider_DestroySpheresElement(globalCtx, spheresElem);
    }
    spheres->nElements = 0;
    if (spheres->elements != NULL) {
        ZeldaArena_FreeDebug(spheres->elements, "../z_collision_check.c", 1393);
    }
    spheres->elements = NULL;
    return 1;
}

s32 Collider_DestroySpheres(GlobalContext* globalCtx, ColliderSpheres* spheres) {
    ColliderSpheresElement* spheresElem;

    Collider_DestroyCollider(globalCtx, &spheres->base);
    for (spheresElem = spheres->elements; spheresElem < (spheres->elements + spheres->nElements); spheresElem++) {
        Collider_DestroySpheresElement(globalCtx, spheresElem);
    }
    spheres->nElements = 0;
    spheres->elements = NULL;
    return 1;
}

// unused
s32 Collider_LoadSpheresAltWithActorMalloc(GlobalContext* globalCtx, ColliderSpheres* spheres,
                                           ColliderSpheresSrcAltWithActor* spheresSrcAltWithActor) {
    ColliderSpheresElement* spheresElem;
    ColliderSpheresElementSrc* spheresElemSrc;

    Collider_LoadColliderAltWithActor(globalCtx, &spheres->base, &spheresSrcAltWithActor->base);
    spheres->nElements = spheresSrcAltWithActor->nElements;
    spheres->elements = ZeldaArena_MallocDebug(spheresSrcAltWithActor->nElements * sizeof(ColliderSpheresElement),
                                               "../z_collision_check.c", 1443);
    if (spheres->elements == NULL) {
        spheres->nElements = 0;
        osSyncPrintf("\x1b[31m");
        osSyncPrintf("ClObjJntSph_set():zelda_malloc()出来ません。\n");
        osSyncPrintf("\x1b[m");
        return 0;
    }
    for (spheresElem = spheres->elements, spheresElemSrc = spheresSrcAltWithActor->elements;
         spheresElem < (spheres->elements + spheres->nElements); spheresElem++, spheresElemSrc++) {
        Collider_InitSpheresElement(globalCtx, spheresElem);
        Collider_LoadSpheresElement(globalCtx, spheresElem, spheresElemSrc);
    }
    return 1;
}

// only used by EnNwc
s32 Collider_LoadSpheresAltMalloc(GlobalContext* globalCtx, ColliderSpheres* spheres, Actor* actor,
                                  ColliderSpheresSrcAlt* spheresSrcAlt) {
    ColliderSpheresElement* spheresElem;
    ColliderSpheresElementSrc* spheresElemSrc;

    Collider_LoadColliderAlt(globalCtx, &spheres->base, actor, &spheresSrcAlt->base);
    spheres->nElements = spheresSrcAlt->nElements;
    spheres->elements = ZeldaArena_MallocDebug(spheresSrcAlt->nElements * sizeof(ColliderSpheresElement),
                                               "../z_collision_check.c", 0x5D2);
    if (spheres->elements == 0) {
        spheres->nElements = 0;
        osSyncPrintf("\x1b[31m");
        osSyncPrintf("ClObjJntSph_set3():zelda_malloc_出来ません。\n");
        osSyncPrintf("\x1b[m");
        return 0;
    }
    for (spheresElem = spheres->elements, spheresElemSrc = spheresSrcAlt->elements;
         spheresElem < (spheres->elements + spheres->nElements); spheresElem++, spheresElemSrc++) {
        Collider_InitSpheresElement(globalCtx, spheresElem);
        Collider_LoadSpheresElement(globalCtx, spheresElem, spheresElemSrc);
    }
    return 1;
}

// unused
s32 Collider_LoadSpheresMalloc(GlobalContext* globalCtx, ColliderSpheres* spheres, Actor* actor,
                               ColliderSpheresSrc* spheresSrc) {
    ColliderSpheresElement* spheresElem;
    ColliderSpheresElementSrc* spheresElemSrc;

    Collider_LoadCollider(globalCtx, &spheres->base, actor, &spheresSrc->base);
    spheres->nElements = spheresSrc->nElements;
    spheres->elements =
        ZeldaArena_MallocDebug(spheresSrc->nElements * sizeof(ColliderSpheresElement), "../z_collision_check.c", 1551);
    if (spheres->elements == NULL) {
        spheres->nElements = 0;
        osSyncPrintf("\x1b[31m");
        osSyncPrintf("ClObjJntSph_set5():zelda_malloc出来ません\n");
        osSyncPrintf("\x1b[m");
        return 0;
    }
    for (spheresElem = spheres->elements, spheresElemSrc = spheresSrc->elements;
         spheresElem < (spheres->elements + spheres->nElements); spheresElem++, spheresElemSrc++) {
        Collider_InitSpheresElement(globalCtx, spheresElem);
        Collider_LoadSpheresElement(globalCtx, spheresElem, spheresElemSrc);
    }
    return 1;
}

s32 Collider_LoadSpheres(GlobalContext* globalCtx, ColliderSpheres* spheres, Actor* actor,
                         ColliderSpheresSrc* spheresSrc, ColliderSpheresElement* spheresElements) {
    ColliderSpheresElement* spheresElem;
    ColliderSpheresElementSrc* spheresElemSrc;

    Collider_LoadCollider(globalCtx, &spheres->base, actor, &spheresSrc->base);
    spheres->nElements = spheresSrc->nElements;
    spheres->elements = spheresElements;
    if (spheres->elements == NULL) {
        __assert("pclobj_jntsph->elem_tbl != NULL", "../z_collision_check.c", 1603);
    }
    for (spheresElem = spheres->elements, spheresElemSrc = spheresSrc->elements;
         spheresElem < (spheres->elements + spheres->nElements); spheresElem++, spheresElemSrc++) {
        Collider_InitSpheresElement(globalCtx, spheresElem);
        Collider_LoadSpheresElement(globalCtx, spheresElem, spheresElemSrc);
    }
    return 1;
}

s32 Collider_ClearSpheresATHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderSpheres* spheres = (ColliderSpheres*)collider;
    ColliderSpheresElement* spheresElem;

    Collider_ClearColliderATHit(globalCtx, &spheres->base);
    for (spheresElem = spheres->elements; spheresElem < (spheres->elements + spheres->nElements); spheresElem++) {
        Collider_ClearSpheresElementATHit(globalCtx, spheresElem);
    }

    return 1;
}

s32 Collider_ClearSpheresACHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderSpheres* spheres = (ColliderSpheres*)collider;
    ColliderSpheresElement* spheresElem;

    Collider_ClearColliderACHit(globalCtx, &spheres->base);
    for (spheresElem = spheres->elements; spheresElem < (spheres->elements + spheres->nElements); spheresElem++) {
        Collider_ClearSpheresElementACHit(globalCtx, spheresElem);
    }
    return 1;
}

s32 Collider_ClearSpheresOCHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderSpheres* spheres = (ColliderSpheres*)collider;
    ColliderSpheresElement* spheresElem;

    Collider_ClearColliderOCHit(globalCtx, &spheres->base);
    for (spheresElem = spheres->elements; spheresElem < (spheres->elements + spheres->nElements); spheresElem++) {
        Collider_ClearSpheresElementOCHit(globalCtx, spheresElem);
    }
    return 1;
}

static Cylinder16 sCylinderShapeInit = {
    0,
    0,
    0,
    { 0, 0, 0 },
};

s32 Collider_InitCylinderShape(GlobalContext* globalCtx, Cylinder16* cylinderShape) {
    Cylinder16 sp4;

    sp4 = sCylinderShapeInit;
    *cylinderShape = sp4;
    return 1;
}

s32 Collider_DestroyCylinderShape(GlobalContext* globalCtx, Cylinder16* cylinderShape) {
    return 1;
}

s32 Collider_LoadCylinderShape(GlobalContext* globalCtx, Cylinder16* dest, Cylinder16* src) {
    *dest = *src;
    return 1;
}

s32 Collider_InitCylinder(GlobalContext* globalCtx, ColliderCylinder* cylinder) {
    Collider_InitCollider(globalCtx, &cylinder->base);
    Collider_InitElement(globalCtx, &cylinder->elem);
    Collider_InitCylinderShape(globalCtx, &cylinder->shape);
    return 1;
}

s32 Collider_DestroyCylinder(GlobalContext* globalCtx, ColliderCylinder* cylinder) {
    Collider_DestroyCollider(globalCtx, &cylinder->base);
    Collider_DestroyElement(globalCtx, &cylinder->elem);
    Collider_DestroyCylinderShape(globalCtx, &cylinder->shape);
    return 1;
}

// only used by ObjDekujr
s32 Collider_LoadCylinderAltWithActor(GlobalContext* globalCtx, ColliderCylinder* cylinder,
                                      ColliderCylinderSrcAltWithActor* src) {
    Collider_LoadColliderAltWithActor(globalCtx, &cylinder->base, &src->base);
    Collider_LoadElement(globalCtx, &cylinder->elem, &src->elem);
    Collider_LoadCylinderShape(globalCtx, &cylinder->shape, &src->shape);
    return 1;
}

s32 Collider_LoadCylinderAlt(GlobalContext* globalCtx, ColliderCylinder* cylinder, Actor* actor,
                             ColliderCylinderSrcAlt* src) {
    Collider_LoadColliderAlt(globalCtx, &cylinder->base, actor, &src->base);
    Collider_LoadElement(globalCtx, &cylinder->elem, &src->elem);
    Collider_LoadCylinderShape(globalCtx, &cylinder->shape, &src->shape);
    return 1;
}

s32 Collider_LoadCylinder(GlobalContext* globalCtx, ColliderCylinder* cylinder, Actor* actor,
                          ColliderCylinderSrc* src) {
    Collider_LoadCollider(globalCtx, &cylinder->base, actor, &src->base);
    Collider_LoadElement(globalCtx, &cylinder->elem, &src->elem);
    Collider_LoadCylinderShape(globalCtx, &cylinder->shape, &src->shape);
    return 1;
}

s32 Collider_ClearCylinderATHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderCylinder* cylinder = (ColliderCylinder*)collider;

    Collider_ClearColliderATHit(globalCtx, &cylinder->base);
    Collider_ClearElementATHit(globalCtx, &cylinder->elem);
    return 1;
}

s32 Collider_ClearCylinderACHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderCylinder* cylinder = (ColliderCylinder*)collider;

    Collider_ClearColliderACHit(globalCtx, &cylinder->base);
    Collider_ClearElementACHit(globalCtx, &cylinder->elem);
    return 1;
}

s32 Collider_ClearCylinderOCHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderCylinder* cylinder = (ColliderCylinder*)collider;

    Collider_ClearColliderOCHit(globalCtx, &cylinder->base);
    Collider_ClearElementOCHit(globalCtx, &cylinder->elem);
    return 1;
}

static TriNorm sTrisElementShapeInit = {
    {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    },
    {
        { 0.0f, 0.0f, 0.0f },
        0.0f,
    },
};

s32 Collider_InitTrisElementShape(GlobalContext* globalCtx, TriNorm* shape) {
    *shape = sTrisElementShapeInit;
    return 1;
}

s32 func_8005C5F8(GlobalContext* globalCtx, UNK_PTR arg1) {
    return 1;
}

s32 Collider_LoadTrisElementShape(GlobalContext* globalCtx, TriNorm* shape, Vec3f* verticesSrc) {
    Vec3f* vtxSrc;
    Vec3f* vtx;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 nd;

    for (vtx = &shape->vtx[0], vtxSrc = &verticesSrc[0]; vtx < (shape->vtx + 3); vtx++, vtxSrc++) {
        *vtx = *vtxSrc;
    }

    func_800CC8B4(&verticesSrc[0], &verticesSrc[1], &verticesSrc[2], &nx, &ny, &nz, &nd);
    shape->plane.normal.x = nx;
    shape->plane.normal.y = ny;
    shape->plane.normal.z = nz;
    shape->plane.originDist = nd;
    return 1;
}

s32 Collider_InitTrisElement(GlobalContext* globalCtx, ColliderTrisElement* arg1) {
    Collider_InitElement(globalCtx, &arg1->base);
    Collider_InitTrisElementShape(globalCtx, &arg1->shape);
    return 1;
}

typedef struct struct_8005C6F8 {
    ColliderElement base;
    char unk_28[0x5C - 0x28];
} struct_8005C6F8; /* size = 0x5C */

s32 func_8005C6F8(GlobalContext* globalCtx, struct_8005C6F8* arg1) {
    Collider_DestroyElement(globalCtx, &arg1->base);
    func_8005C5F8(globalCtx, arg1->unk_28);
    return 1;
}

s32 Collider_LoadTrisElement(GlobalContext* globalCtx, ColliderTrisElement* trisElem,
                             ColliderTrisElementSrc* trisElemSrc) {
    Collider_LoadElement(globalCtx, &trisElem->base, &trisElemSrc->base);
    Collider_LoadTrisElementShape(globalCtx, &trisElem->shape, trisElemSrc->vertices);
    return 1;
}

s32 Collider_ClearTrisElementATHit(GlobalContext* globalCtx, ColliderTrisElement* trisElem) {
    Collider_ClearElementATHit(globalCtx, &trisElem->base);
    return 1;
}

s32 Collider_ClearTrisElementACHit(GlobalContext* globalCtx, ColliderTrisElement* trisElem) {
    Collider_ClearElementACHit(globalCtx, &trisElem->base);
    return 1;
}

s32 Collider_ClearTrisElementOCHit(GlobalContext* globalCtx, ColliderTrisElement* trisElem) {
    Collider_ClearElementOCHit(globalCtx, &trisElem->base);
    return 1;
}

typedef struct struct_8005C7E0 {
    Collider unk0;
    s32 unk18;
    s32 unk1C;
} struct_8005C7E0;

// uses not decompiled
s32 func_8005C7E0(GlobalContext* globalCtx, struct_8005C7E0* arg1) {
    Collider_InitCollider(globalCtx, &arg1->unk0);
    arg1->unk18 = 0;
    arg1->unk1C = 0;
    return 1;
}

typedef struct struct_8005C810 {
    Collider unk0;
    s32 unk18;
    struct_8005C6F8* unk1C;
} struct_8005C810;

// unused
s32 func_8005C810(GlobalContext* globalCtx, struct_8005C810* arg1) {
    struct_8005C6F8* var_s0;

    Collider_DestroyCollider(globalCtx, &arg1->unk0);
    for (var_s0 = arg1->unk1C; var_s0 < (arg1->unk1C + arg1->unk18); var_s0++) {
        func_8005C6F8(globalCtx, var_s0);
    }
    arg1->unk18 = 0;
    if (arg1->unk1C != NULL) {
        ZeldaArena_FreeDebug(arg1->unk1C, "../z_collision_check.c", 0x833);
    }
    arg1->unk1C = 0;
    return 1;
}

typedef struct struct_8005C8C8 {
    Collider unk0;
    s32 unk18;
    struct_8005C6F8* unk1C;
} struct_8005C8C8;

// uses not decompiled
s32 func_8005C8C8(GlobalContext* globalCtx, struct_8005C8C8* arg1) {
    struct_8005C6F8* var_s0;

    Collider_DestroyCollider(globalCtx, &arg1->unk0);
    for (var_s0 = arg1->unk1C; var_s0 < &arg1->unk1C[arg1->unk18]; var_s0++) {
        func_8005C6F8(globalCtx, var_s0);
    }
    arg1->unk18 = 0;
    arg1->unk1C = NULL;
    return 1;
}

// unused
s32 Collider_LoadTrisAltMalloc(GlobalContext* globalCtx, ColliderTris* tris, Actor* actor,
                               ColliderTrisSrcAlt* trisSrcAlt) {
    ColliderTrisElement* trisElem;
    ColliderTrisElementSrc* trisElemSrc;

    Collider_LoadColliderAlt(globalCtx, &tris->base, actor, &trisSrcAlt->base);
    tris->nElements = trisSrcAlt->nElements;
    tris->elements =
        ZeldaArena_MallocDebug(tris->nElements * sizeof(ColliderTrisElement), "../z_collision_check.c", 2156);
    if (tris->elements == NULL) {
        tris->nElements = 0;
        osSyncPrintf("\x1b[31m");
        osSyncPrintf("ClObjTris_set3():zelda_malloc()出来ません\n");
        osSyncPrintf("\x1b[m");
        return 0;
    }
    for (trisElem = tris->elements, trisElemSrc = trisSrcAlt->elements; trisElem < (tris->elements + tris->nElements);
         trisElem++, trisElemSrc++) {
        Collider_InitTrisElement(globalCtx, trisElem);
        Collider_LoadTrisElement(globalCtx, trisElem, trisElemSrc);
    }
    return 1;
}

// unused
s32 Collider_LoadTrisMalloc(GlobalContext* globalCtx, ColliderTris* tris, Actor* actor, ColliderTrisSrc* trisSrc) {
    ColliderTrisElement* trisElem;
    ColliderTrisElementSrc* trisElemSrc;

    Collider_LoadCollider(globalCtx, &tris->base, actor, &trisSrc->base);
    tris->nElements = trisSrc->nElements;
    tris->elements =
        ZeldaArena_MallocDebug(tris->nElements * sizeof(ColliderTrisElement), "../z_collision_check.c", 2207);
    if (tris->elements == NULL) {
        osSyncPrintf("\x1b[31m");
        osSyncPrintf("ClObjTris_set5():zelda_malloc出来ません\n");
        osSyncPrintf("\x1b[m");
        tris->nElements = 0;
        return 0;
    }
    for (trisElem = tris->elements, trisElemSrc = trisSrc->elements; trisElem < &tris->elements[tris->nElements];
         trisElem++, trisElemSrc++) {
        Collider_InitTrisElement(globalCtx, trisElem);
        Collider_LoadTrisElement(globalCtx, trisElem, trisElemSrc);
    }
    return 1;
}

// uses not decompiled
s32 Collider_LoadTris(GlobalContext* globalCtx, ColliderTris* tris, Actor* actor, ColliderTrisSrc* trisSrc,
                      ColliderTrisElement* trisElements) {
    ColliderTrisElement* trisElem;
    ColliderTrisElementSrc* trisElemSrc;

    Collider_LoadCollider(globalCtx, &tris->base, actor, &trisSrc->base);
    tris->nElements = trisSrc->nElements;
    tris->elements = trisElements;
    if (tris->elements == NULL) {
        __assert("pclobj_tris->elem_tbl != NULL", "../z_collision_check.c", 2258);
    }
    for (trisElem = tris->elements, trisElemSrc = trisSrc->elements; trisElem < &tris->elements[tris->nElements];
         trisElem++, trisElemSrc++) {
        Collider_InitTrisElement(globalCtx, trisElem);
        Collider_LoadTrisElement(globalCtx, trisElem, trisElemSrc);
    }
    return 1;
}

s32 Collider_ClearTrisATHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderTrisElement* trisElem;
    ColliderTris* tris = (ColliderTris*)collider;

    Collider_ClearColliderATHit(globalCtx, &tris->base);
    for (trisElem = tris->elements; trisElem < (tris->elements + tris->nElements); trisElem++) {
        Collider_ClearTrisElementATHit(globalCtx, trisElem);
    }
    return 1;
}

s32 Collider_ClearTrisACHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderTrisElement* trisElem;
    ColliderTris* tris = (ColliderTris*)collider;

    Collider_ClearColliderACHit(globalCtx, &tris->base);
    for (trisElem = tris->elements; trisElem < (tris->elements + tris->nElements); trisElem++) {
        Collider_ClearTrisElementACHit(globalCtx, trisElem);
    }
    return 1;
}

s32 Collider_ClearTrisOCHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderTris* tris = (ColliderTris*)collider;
    ColliderTrisElement* trisElem;

    Collider_ClearColliderOCHit(globalCtx, &tris->base);

    for (trisElem = tris->elements; trisElem < (tris->elements + tris->nElements); trisElem++) {
        Collider_ClearTrisElementOCHit(globalCtx, trisElem);
    }
    return 1;
}

static ColliderQuadShape sColliderQuadShapeInit = {
    {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    },
    { 0, 0, 0 },
    { 0, 0, 0 },
    1e38f,
};

s32 Collider_InitQuadShape(GlobalContext* globalCtx, ColliderQuadShape* quadShape) {
    *quadShape = sColliderQuadShapeInit;
    return 1;
}

s32 Collider_DestroyQuadShape(GlobalContext* globalCtx, ColliderQuadShape* quadShape) {
    return 1;
}

s32 Collider_ClearQuadShapeATHit(GlobalContext* globalCtx, ColliderQuadShape* quadShape) {
    quadShape->unk3C = 1e38f;
    return 1;
}

void Collider_QuadShapeUpdateMidPoints(ColliderQuadShape* quadShape) {
    quadShape->middleAB.x = (quadShape->corners.cornerA.x + quadShape->corners.cornerB.x) * 0.5f;
    quadShape->middleAB.y = (quadShape->corners.cornerA.y + quadShape->corners.cornerB.y) * 0.5f;
    quadShape->middleAB.z = (quadShape->corners.cornerA.z + quadShape->corners.cornerB.z) * 0.5f;
    quadShape->middleCD.x = (quadShape->corners.cornerD.x + quadShape->corners.cornerC.x) * 0.5f;
    quadShape->middleCD.y = (quadShape->corners.cornerD.y + quadShape->corners.cornerC.y) * 0.5f;
    quadShape->middleCD.z = (quadShape->corners.cornerD.z + quadShape->corners.cornerC.z) * 0.5f;
}

s32 Collider_LoadQuadShape(GlobalContext* globalCtx, ColliderQuadShape* quadShape,
                           ColliderQuadShapeCorners* cornersSrc) {
    quadShape->corners.cornerD = cornersSrc->cornerD;
    quadShape->corners.cornerC = cornersSrc->cornerC;
    quadShape->corners.cornerA = cornersSrc->cornerA;
    quadShape->corners.cornerB = cornersSrc->cornerB;
    Collider_QuadShapeUpdateMidPoints(quadShape);
    return 1;
}

s32 Collider_InitQuad(GlobalContext* globalCtx, ColliderQuad* quad) {
    Collider_InitCollider(globalCtx, &quad->base);
    Collider_InitElement(globalCtx, &quad->elem);
    Collider_InitQuadShape(globalCtx, &quad->shape);
    return 1;
}

s32 Collider_DestroyQuad(GlobalContext* globalCtx, ColliderQuad* quad) {
    Collider_DestroyCollider(globalCtx, &quad->base);
    Collider_DestroyElement(globalCtx, &quad->elem);
    Collider_DestroyQuadShape(globalCtx, &quad->shape);
    return 1;
}

// unused
s32 Collider_LoadQuadAlt(GlobalContext* globalCtx, ColliderQuad* quad, Actor* actor, ColliderQuadSrcAlt* quadSrcAlt) {
    Collider_LoadColliderAlt(globalCtx, &quad->base, actor, &quadSrcAlt->base);
    Collider_LoadElement(globalCtx, &quad->elem, &quadSrcAlt->elem);
    Collider_LoadQuadShape(globalCtx, &quad->shape, &quadSrcAlt->corners);
    return 1;
}

s32 Collider_LoadQuad(GlobalContext* globalCtx, ColliderQuad* quad, Actor* actor, ColliderQuadSrc* quadSrc) {
    Collider_LoadCollider(globalCtx, &quad->base, actor, &quadSrc->base);
    Collider_LoadElement(globalCtx, &quad->elem, &quadSrc->elem);
    Collider_LoadQuadShape(globalCtx, &quad->shape, &quadSrc->corners);
    return 1;
}

s32 Collider_ClearQuadATHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderQuad* quad = (ColliderQuad*)collider;

    Collider_ClearColliderATHit(globalCtx, &quad->base);
    Collider_ClearElementATHit(globalCtx, &quad->elem);
    Collider_ClearQuadShapeATHit(globalCtx, &quad->shape);
    return 1;
}

s32 Collider_ClearQuadACHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderQuad* quad = (ColliderQuad*)collider;

    Collider_ClearColliderACHit(globalCtx, &quad->base);
    Collider_ClearElementACHit(globalCtx, &quad->elem);
    return 1;
}

s32 Collider_ClearQuadOCHit(GlobalContext* globalCtx, Collider* collider) {
    ColliderQuad* quad = (ColliderQuad*)collider;

    Collider_ClearColliderOCHit(globalCtx, &quad->base);
    Collider_ClearElementOCHit(globalCtx, &quad->elem);
    return 1;
}

s32 func_8005D218(GlobalContext* globalCtx, ColliderQuad* arg1, Vec3f* arg2) {
    f32 temp_fv0;
    Vec3f sp20;

    if (!(arg1->elem.atElemFlags & ATELEM_NEAREST)) {
        return true;
    }
    Math_Vec3s_ToVec3f(&sp20, &arg1->shape.middleAB);
    temp_fv0 = func_800CB650(&sp20, arg2);
    if (temp_fv0 < arg1->shape.unk3C) {
        arg1->shape.unk3C = temp_fv0;
        if (arg1->elem.atHit != NULL) {
            Collider_ClearColliderACHit(globalCtx, arg1->elem.atHit);
        }
        if (arg1->elem.atHitElem != NULL) {
            Collider_ClearElementACHit(globalCtx, arg1->elem.atHitElem);
        }
        return true;
    } else {
        return false;
    }
}

// unused
s32 func_8005D2C4(s32 arg0, Vec3f* arg1) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    Math_Vec3f_Copy(&arg1[0], &zeroVec);
    Math_Vec3f_Copy(&arg1[1], &zeroVec);
    return 1;
}

// unused
s32 func_8005D324(s32 arg0, s32 arg1) {
    return 1;
}

s32 func_8005D334(s32 arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3) {
    Math_Vec3f_Copy(&arg1[0], arg2);
    Math_Vec3f_Copy(&arg1[1], arg3);
    return 1;
}

typedef struct struct_8005D378_arg1 {
    Vec3f unk0[2];
    u16 unk18;
} struct_8005D378_arg1;

typedef struct struct_8005D378_arg2 {
    Vec3f unk0;
    Vec3f unkC;
    u16 unk18;
} struct_8005D378_arg2;

// unused
s32 func_8005D378(s32 arg0, struct_8005D378_arg1* arg1, struct_8005D378_arg2* arg2) {
    arg1->unk18 = arg2->unk18;
    func_8005D334(arg0, arg1->unk0, &arg2->unk0, &arg2->unkC);
    return 1;
}

s32 func_8005D3A4(GlobalContext* globalCtx, struct_ColliderCtx_290* arg1) {
    arg1->unk18 &= ~1;
    return 1;
}

void func_8005D40C(GlobalContext* globalCtx, ColliderContext* colliderCtx);

void func_8005D3BC(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    colliderCtx->unk2 = 0;
    func_8005D40C(globalCtx, colliderCtx);
    gGameInfo->data[0x735] = 1;
    gGameInfo->data[0x736] = 1;
    gGameInfo->data[0x737] = 1;
}

void func_8005D400(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
}

void func_8005D40C(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    Collider** colliderP;
    struct_ColliderCtx_290** var;

    if (!(colliderCtx->unk2 & 1)) {
        colliderCtx->nATColliders = 0;
        colliderCtx->nACColliders = 0;
        colliderCtx->nOCColliders = 0;
        colliderCtx->unk28C = 0;
        for (colliderP = colliderCtx->atColliders;
             colliderP < (colliderCtx->atColliders + ARRAY_COUNT(colliderCtx->atColliders)); colliderP++) {
            *colliderP = NULL;
        }

        for (colliderP = colliderCtx->acColliders;
             colliderP < (colliderCtx->acColliders + ARRAY_COUNT(colliderCtx->acColliders)); colliderP++) {
            *colliderP = NULL;
        }
        for (colliderP = colliderCtx->ocColliders;
             colliderP < (colliderCtx->ocColliders + ARRAY_COUNT(colliderCtx->ocColliders)); colliderP++) {
            *colliderP = NULL;
        }
        for (var = colliderCtx->unk290; var < (colliderCtx->unk290 + ARRAY_COUNT(colliderCtx->unk290)); var++) {
            *var = NULL;
        }
    }
}

// unused
void func_8005D4B4(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    colliderCtx->unk2 |= 1;
}

// unused
void func_8005D4C8(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    colliderCtx->unk2 &= ~1;
}

void func_8005D4DC(GlobalContext* globalCtx, Collider* collider) {
    ColliderSpheres* spheres;
    ColliderCylinder* cylinder;
    ColliderTris* tris;
    ColliderQuad* quad;
    s32 i;

    if (collider != NULL) {
        switch (collider->colType) {
            case COLTYPE_SPHERES:
                spheres = (ColliderSpheres*)collider;

                for (i = 0; i < spheres->nElements; i++) {
                    func_800D05D0(globalCtx, &spheres->elements[i].shape.world);
                }
                break;

            case COLTYPE_CYLINDER:
                cylinder = (ColliderCylinder*)collider;
                func_800D05DC(globalCtx, &cylinder->shape);
                break;

            case COLTYPE_TRIANGLES:
                tris = (ColliderTris*)collider;
                for (i = 0; i < tris->nElements; i++) {
                    func_8005B280(globalCtx->state.gfxCtx, &tris->elements[i].shape.vtx[0],
                                  &tris->elements[i].shape.vtx[1], &tris->elements[i].shape.vtx[2]);
                }
                break;

            case COLTYPE_QUAD:
                quad = (ColliderQuad*)collider;
                func_8005B280(globalCtx->state.gfxCtx, &quad->shape.corners.cornerA, &quad->shape.corners.cornerB,
                              &quad->shape.corners.cornerC);
                func_8005B280(globalCtx->state.gfxCtx, &quad->shape.corners.cornerC, &quad->shape.corners.cornerD,
                              &quad->shape.corners.cornerA);
                break;
        }
    }
}

void func_8005D62C(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    s32 i;

    if (gGameInfo->data[0x72F] != 0) {
        if (gGameInfo->data[0x735] != 0) {
            for (i = 0; i < colliderCtx->nATColliders; i++) {
                func_8005D4DC(globalCtx, colliderCtx->atColliders[i]);
            }
        }
        if (gGameInfo->data[0x736] != 0) {
            for (i = 0; i < colliderCtx->nACColliders; i++) {
                func_8005D4DC(globalCtx, colliderCtx->acColliders[i]);
            }
        }
        if (gGameInfo->data[0x737] != 0) {
            for (i = 0; i < colliderCtx->nOCColliders; i++) {
                if (colliderCtx->ocColliders[i]->ocFlags1 & OC1_ON) {
                    func_8005D4DC(globalCtx, colliderCtx->ocColliders[i]);
                }
            }
        }
        if (gGameInfo->data[0x738] != 0) {
            func_80042C3C(globalCtx, &globalCtx->colCtx);
        }
        if (gGameInfo->data[0x739] != 0) {
            func_80042FC4(globalCtx, &globalCtx->colCtx);
        }
    }
}

typedef s32 (*ClearATHitFunc)(GlobalContext*, Collider*);

ClearATHitFunc sClearATHitFuncs[] = {
    Collider_ClearSpheresATHit,  // COLTYPE_SPHERES
    Collider_ClearCylinderATHit, // COLTYPE_CYLINDER
    Collider_ClearTrisATHit,     // COLTYPE_TRIANGLES
    Collider_ClearQuadATHit,     // COLTYPE_QUAD
};

s32 Collider_AddAT(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    s16 i;

    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    if (collider->colType >= COLTYPE_MAX) {
        __assert("pcl_obj->data_type <= CL_DATA_LBL_SWRD", "../z_collision_check.c", 2997);
    }
    sClearATHitFuncs[collider->colType](globalCtx, collider);
    if ((collider->actor != NULL) && (collider->actor->update == NULL)) {
        return -1;
    }
    i = colliderCtx->nATColliders;
    if (i >= ARRAY_COUNT(colliderCtx->atColliders)) {
        osSyncPrintf("CollisionCheck_setAT():インデックスがオーバーして追加不能\n");
        return -1;
    }
    if (colliderCtx->unk2 & 1) {
        return -1;
    }
    colliderCtx->atColliders[i] = collider;
    colliderCtx->nATColliders++;
    return i;
}

// unused
s32 func_8005D8AC(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider, s32 i) {
    if (collider->colType >= COLTYPE_MAX) {
        __assert("pcl_obj->data_type <= CL_DATA_LBL_SWRD", "../z_collision_check.c", 3037);
    }
    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    sClearATHitFuncs[collider->colType](globalCtx, collider);
    if ((collider->actor != NULL) && ((collider->actor)->update == 0)) {
        return -1;
    }
    if (colliderCtx->unk2 & 1) {
        if (i >= colliderCtx->nATColliders) {
            osSyncPrintf("CollisionCheck_setAT_SAC():全データ数より大きいところに登録しようとしている。\n");
            return -1;
        }
        colliderCtx->atColliders[i] = collider;
    } else {
        if (colliderCtx->nATColliders >= ARRAY_COUNT(colliderCtx->atColliders)) {
            osSyncPrintf("CollisionCheck_setAT():インデックスがオーバーして追加不能\n");
            return -1;
        }
        i = colliderCtx->nATColliders;
        colliderCtx->atColliders[colliderCtx->nATColliders] = collider;
        colliderCtx->nATColliders++;
    }
    return i;
}

typedef s32 (*ClearACHitFunc)(GlobalContext*, Collider*);

ClearACHitFunc sClearACHitFuncs[] = {
    Collider_ClearSpheresACHit,  // COLTYPE_SPHERES
    Collider_ClearCylinderACHit, // COLTYPE_CYLINDER
    Collider_ClearTrisACHit,     // COLTYPE_TRIANGLES
    Collider_ClearQuadACHit,     // COLTYPE_QUAD
};

s32 Collider_AddAC(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    s32 i;

    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    if (collider->colType >= COLTYPE_MAX) {
        __assert("pcl_obj->data_type <= CL_DATA_LBL_SWRD", "../z_collision_check.c", 0xC2A);
    }
    sClearACHitFuncs[collider->colType](globalCtx, collider);
    if ((collider->actor != NULL) && (collider->actor->update == NULL)) {
        return -1;
    }
    i = colliderCtx->nACColliders;
    if (i >= ARRAY_COUNT(colliderCtx->acColliders)) {
        osSyncPrintf("CollisionCheck_setAC():インデックスがオーバして追加不能\n");
        return -1;
    }
    if (colliderCtx->unk2 & 1) {
        return -1;
    }
    colliderCtx->acColliders[i] = collider;
    colliderCtx->nACColliders++;
    return i;
}

// unused
s32 func_8005DB04(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider, s32 i) {
    if (collider->colType >= COLTYPE_MAX) {
        __assert("pcl_obj->data_type <= CL_DATA_LBL_SWRD", "../z_collision_check.c", 3153);
    }
    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    sClearACHitFuncs[collider->colType](globalCtx, collider);
    if ((collider->actor != NULL) && (collider->actor->update == NULL)) {
        return -1;
    }
    if (colliderCtx->unk2 & 1) {
        if (i >= colliderCtx->nACColliders) {
            osSyncPrintf("CollisionCheck_setAC_SAC():全データ数より大きいところに登録しようとしている。\n");
            return -1;
        }
        colliderCtx->acColliders[i] = collider;
    } else {
        if (colliderCtx->nACColliders >= ARRAY_COUNT(colliderCtx->acColliders)) {
            osSyncPrintf("CollisionCheck_setAC():インデックスがオーバして追加不能\n");
            return -1;
        }
        i = colliderCtx->nACColliders;
        colliderCtx->acColliders[i] = collider;
        colliderCtx->nACColliders++;
    }
    return i;
}

typedef s32 (*ClearOCHitFunc)(GlobalContext*, Collider*);

ClearOCHitFunc sClearOCHitFuncs[] = {
    Collider_ClearSpheresOCHit,  // COLTYPE_SPHERES
    Collider_ClearCylinderOCHit, // COLTYPE_CYLINDER
    Collider_ClearTrisOCHit,     // COLTYPE_TRIANGLES
    Collider_ClearQuadOCHit,     // COLTYPE_QUAD
};

s32 Collider_AddOC(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    s32 i;

    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    if (collider->colType >= COLTYPE_MAX) {
        __assert("pcl_obj->data_type <= CL_DATA_LBL_SWRD", "../z_collision_check.c", 3229);
    }
    sClearOCHitFuncs[collider->colType](globalCtx, collider);
    if ((collider->actor != NULL) && (collider->actor->update == NULL)) {
        return -1;
    }
    i = colliderCtx->nOCColliders;
    if (i >= ARRAY_COUNT(colliderCtx->ocColliders)) {
        osSyncPrintf("CollisionCheck_setOC():インデックスがオーバして追加不能\n");
        return -1;
    }
    if (colliderCtx->unk2 & 1) {
        return -1;
    }
    colliderCtx->ocColliders[i] = collider;
    colliderCtx->nOCColliders++;
    return i;
}

// unused
s32 func_8005DD5C(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider, s32 i) {
    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    if (collider->colType >= COLTYPE_MAX) {
        __assert("pcl_obj->data_type <= CL_DATA_LBL_SWRD", "../z_collision_check.c", 3274);
        if (1) {}
    }
    sClearOCHitFuncs[collider->colType](globalCtx, collider);
    if ((collider->actor != NULL) && (collider->actor->update == NULL)) {
        return -1;
    }
    if (colliderCtx->unk2 & 1) {
        if (i >= colliderCtx->nOCColliders) {
            osSyncPrintf("CollisionCheck_setOC_SAC():全データ数より大きいところに登録しようとしている。\n");
            return -1;
        }
        colliderCtx->atColliders[i] = collider;
    } else {
        if (colliderCtx->nOCColliders >= ARRAY_COUNT(colliderCtx->ocColliders)) {
            osSyncPrintf("CollisionCheck_setOC():インデックスがオーバして追加不能\n");
            return -1;
        }
        i = colliderCtx->nOCColliders;
        colliderCtx->ocColliders[i] = collider;
        colliderCtx->nOCColliders++;
    }
    return i;
}

// unused
s32 func_8005DE9C(GlobalContext* globalCtx, ColliderContext* colliderCtx, struct_ColliderCtx_290* arg2) {
    s32 i;

    if (func_800C0D28(globalCtx) == 1) {
        return -1;
    }
    func_8005D3A4(globalCtx, arg2);
    i = colliderCtx->unk28C;
    if (i >= ARRAY_COUNT(colliderCtx->unk290)) {
        osSyncPrintf("CollisionCheck_setOCLine():インデックスがオーバして追加不能\n");
        return -1;
    }
    colliderCtx->unk290[i] = arg2;
    colliderCtx->unk28C++;
    return i;
}

s32 Collider_IsElementATOff(ColliderElement* elem) {
    if (!(elem->atElemFlags & ATELEM_ON)) {
        return 1;
    }
    return 0;
}

s32 Collider_IsElementACOff(ColliderElement* elem) {
    if (!(elem->acElemFlags & ACELEM_ON)) {
        return 1;
    }
    return 0;
}

s32 Collider_AreDmgFlagsDisjoint(ColliderElement* atElem, ColliderElement* acElem) {
    if ((atElem->atDmgInfo.dmgFlags & acElem->acDmgInfo.dmgFlags) == 0) {
        return 1;
    }

    return 0;
}

void Collider_NoBlood(GlobalContext* globalCtx, Collider* collider, Vec3f* arg2) {
}

#ifndef GLUE
#define GLUE(a, b) a##b
#endif
#ifndef GLUE2
#define GLUE2(a, b) GLUE(a, b)
#endif
#define BSS_DUMMY GLUE2(s8 sBssDummy, __LINE__)

void Collider_BlueBlood(GlobalContext* globalCtx, Collider* collider, Vec3f* arg2) {
    static struct_8015D8A0 D_8015D8A0;

    s32 sp24;

    D_8015D8A0.unk0.x = (s16)(s32)arg2->x;
    D_8015D8A0.unk0.y = (s16)(s32)arg2->y;
    D_8015D8A0.unk0.z = (s16)(s32)arg2->z;
    D_8015D8A0.unk494 = 5;
    D_8015D8A0.unk498 = 5;
    D_8015D8A0.unk49C[0].r = 0xA;
    D_8015D8A0.unk49C[0].g = 0xA;
    D_8015D8A0.unk49C[0].b = 0xC8;
    D_8015D8A0.unk49C[0].a = 0xFF;
    D_8015D8A0.unk49C[1].r = 0;
    D_8015D8A0.unk49C[1].g = 0;
    D_8015D8A0.unk49C[1].b = 128;
    D_8015D8A0.unk49C[1].a = 0xFF;
    D_8015D8A0.unk49C[2].r = 0;
    D_8015D8A0.unk49C[2].g = 0;
    D_8015D8A0.unk49C[2].b = 128;
    D_8015D8A0.unk49C[2].a = 0xFF;
    D_8015D8A0.unk49C[3].r = 0;
    D_8015D8A0.unk49C[3].g = 0;
    D_8015D8A0.unk49C[3].b = 128;
    D_8015D8A0.unk49C[3].a = 0xFF;
    D_8015D8A0.unk4AC[0].r = 0;
    D_8015D8A0.unk4AC[0].g = 0;
    D_8015D8A0.unk4AC[0].b = 0x20;
    D_8015D8A0.unk4AC[0].a = 0;
    D_8015D8A0.unk4AC[1].r = 0;
    D_8015D8A0.unk4AC[1].g = 0;
    D_8015D8A0.unk4AC[1].b = 0x20;
    D_8015D8A0.unk4AC[1].a = 0;
    D_8015D8A0.unk4AC[2].r = 0;
    D_8015D8A0.unk4AC[2].g = 0;
    D_8015D8A0.unk4AC[2].b = 0x40;
    D_8015D8A0.unk4AC[2].a = 0;
    D_8015D8A0.unk4AC[3].r = 0;
    D_8015D8A0.unk4AC[3].g = 0;
    D_8015D8A0.unk4AC[3].b = 0x40;
    D_8015D8A0.unk4AC[3].a = 0;
    D_8015D8A0.unk4BC = 0;
    D_8015D8A0.unk4C0 = 0x10;
    D_8015D8A0.unk48C = 8.0f;
    D_8015D8A0.unk490 = -1.0f;
    Effect_Add(globalCtx, &sp24, 0, 0U, 1U, &D_8015D8A0);
}

BSS_DUMMY;
BSS_DUMMY;
BSS_DUMMY;
BSS_DUMMY;

void Collider_GreenBlood(GlobalContext* globalCtx, Collider* collider, Vec3f* arg2) {
    static struct_8015D8A0 D_8015DD68;
    s32 sp24;

    D_8015DD68.unk0.x = arg2->x;
    D_8015DD68.unk0.y = arg2->y;
    D_8015DD68.unk0.z = arg2->z;
    D_8015DD68.unk494 = 5;
    D_8015DD68.unk498 = 5;
    D_8015DD68.unk49C[0].r = 0xA;
    D_8015DD68.unk49C[0].g = 0xC8;
    D_8015DD68.unk49C[0].b = 0xA;
    D_8015DD68.unk49C[0].a = 0xFF;
    D_8015DD68.unk49C[1].r = 0;
    D_8015DD68.unk49C[1].g = 128;
    D_8015DD68.unk49C[1].b = 0;
    D_8015DD68.unk49C[1].a = 0xFF;
    D_8015DD68.unk49C[2].r = 0;
    D_8015DD68.unk49C[2].g = 128;
    D_8015DD68.unk49C[2].b = 0;
    D_8015DD68.unk49C[2].a = 0xFF;
    D_8015DD68.unk49C[3].r = 0;
    D_8015DD68.unk49C[3].g = 128;
    D_8015DD68.unk49C[3].b = 0;
    D_8015DD68.unk49C[3].a = 0xFF;
    D_8015DD68.unk4AC[0].r = 0;
    D_8015DD68.unk4AC[0].g = 0x20;
    D_8015DD68.unk4AC[0].b = 0;
    D_8015DD68.unk4AC[0].a = 0;
    D_8015DD68.unk4AC[1].r = 0;
    D_8015DD68.unk4AC[1].g = 0x20;
    D_8015DD68.unk4AC[1].b = 0;
    D_8015DD68.unk4AC[1].a = 0;
    D_8015DD68.unk4AC[2].r = 0;
    D_8015DD68.unk4AC[2].g = 0x40;
    D_8015DD68.unk4AC[2].b = 0;
    D_8015DD68.unk4AC[2].a = 0;
    D_8015DD68.unk4AC[3].r = 0;
    D_8015DD68.unk4AC[3].g = 0x40;
    D_8015DD68.unk4AC[3].b = 0;
    D_8015DD68.unk4AC[3].a = 0;
    D_8015DD68.unk4BC = 0;
    D_8015DD68.unk4C0 = 0x10;
    D_8015DD68.unk48C = 8.0f;
    D_8015DD68.unk490 = -1.0f;
    Effect_Add(globalCtx, &sp24, 0, 0U, 1U, &D_8015DD68);
}

void func_80062B80(GlobalContext* globalCtx, Vec3f* arg1);

void Collider_WaterBlood(GlobalContext* globalCtx, Collider* collider, Vec3f* arg2) {
    func_800299AC(globalCtx, arg2);
    func_80062B80(globalCtx, arg2);
}

void func_80062A28(GlobalContext* globalCtx, Vec3f* arg1);

void Collider_RedBlood(GlobalContext* globalCtx, Collider* collider, Vec3f* arg3) {
    func_80062A28(globalCtx, arg3);
}

void Collider_RedBloodUnused(GlobalContext* globalCtx, Collider* collider, Vec3f* arg3) {
    func_80062A28(globalCtx, arg3);
}

void func_80062D60(GlobalContext* globalCtx, Vec3f* arg1);
void func_80062DAC(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2);

void func_8005E2EC(GlobalContext* globalCtx, ColliderElement* arg1, Collider* collider, Vec3f* arg3) {
    if (((arg1->atElemFlags & ATELEM_SFX_MASK) == ATELEM_SFX_NORMAL) && (collider->colMaterial != COL_MATERIAL_METAL)) {
        func_80029CA4(globalCtx, 0, arg3);
        if (collider->actor == NULL) {
            Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_BOUND, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_BOUND, &collider->actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    } else if ((arg1->atElemFlags & ATELEM_SFX_MASK) == ATELEM_SFX_NORMAL) {
        func_80029CA4(globalCtx, 3, arg3);
        if (collider->actor == NULL) {
            func_80062D60(globalCtx, arg3);
        } else {
            func_80062DAC(globalCtx, arg3, &collider->actor->unk_E4);
        }
    } else if ((arg1->atElemFlags & ATELEM_SFX_MASK) == ATELEM_SFX_HARD) {
        func_80029CA4(globalCtx, 0, arg3);
        if (collider->actor == NULL) {
            Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_BOUND, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_BOUND, &collider->actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    } else if ((arg1->atElemFlags & ATELEM_SFX_MASK) == ATELEM_SFX_WOOD) {
        func_80029CA4(globalCtx, 1, arg3);
        if (collider->actor == NULL) {
            Audio_PlaySoundGeneral(NA_SE_IT_REFLECTION_WOOD, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            Audio_PlaySoundGeneral(NA_SE_IT_REFLECTION_WOOD, &collider->actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    }
}

s32 func_8005E4F8(Collider* collider, ColliderElement* arg1) {
    Actor* actor;

    actor = collider->actor;
    if ((actor != NULL) && (actor->type == ACTORTYPE_PLAYER)) {
        if (arg1->elemMaterial == 0) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_STRIKE, &actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        } else if (arg1->elemMaterial == 1) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_STRIKE_HARD, &actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        } else if (arg1->elemMaterial == 2) {
            Audio_PlaySoundGeneral(0, &actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        } else if (arg1->elemMaterial == 3) {
            Audio_PlaySoundGeneral(0, &actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    }
    return 1;
}

typedef void (*ColliderBloodFunc)(GlobalContext*, Collider*, Vec3f*);

static ColliderBloodFunc sBloodFuncs[] = {
    Collider_NoBlood,        // BLOOD_NONE
    Collider_BlueBlood,      // BLOOD_BLUE
    Collider_GreenBlood,     // BLOOD_GREEN
    Collider_WaterBlood,     // BLOOD_WATER
    Collider_RedBlood,       // BLOOD_RED
    Collider_RedBloodUnused, // BLOOD_RED2
};

typedef enum ColliderBloodType {
    /* 0 */ BLOOD_NONE,
    /* 1 */ BLOOD_BLUE,
    /* 2 */ BLOOD_GREEN,
    /* 3 */ BLOOD_WATER,
    /* 4 */ BLOOD_RED,
    /* 5 */ BLOOD_RED2
} ColliderBloodType;

typedef enum ColliderHitType {
    /* 0 */ HIT_WHITE,
    /* 1 */ HIT_DUST,
    /* 2 */ HIT_RED,
    /* 3 */ HIT_SOLID,
    /* 4 */ HIT_WOOD,
    /* 5 */ HIT_NONE
} ColliderHitType;

typedef struct HitInfo {
    u8 blood;
    u8 effect;
} HitInfo;

static HitInfo sHitInfo[] = {
    { BLOOD_BLUE, HIT_WHITE },  // COL_MATERIAL_HIT0
    { BLOOD_NONE, HIT_DUST },   // COL_MATERIAL_HIT1
    { BLOOD_GREEN, HIT_DUST },  // COL_MATERIAL_HIT2
    { BLOOD_NONE, HIT_WHITE },  // COL_MATERIAL_HIT3
    { BLOOD_WATER, HIT_NONE },  // COL_MATERIAL_HIT4
    { BLOOD_NONE, HIT_RED },    // COL_MATERIAL_HIT5
    { BLOOD_GREEN, HIT_WHITE }, // COL_MATERIAL_HIT6
    { BLOOD_RED, HIT_WHITE },   // COL_MATERIAL_HIT7
    { BLOOD_BLUE, HIT_RED },    // COL_MATERIAL_HIT8
    { BLOOD_NONE, HIT_SOLID },  // COL_MATERIAL_METAL
    { BLOOD_NONE, HIT_NONE },   // COL_MATERIAL_NONE
    { BLOOD_NONE, HIT_SOLID },  // COL_MATERIAL_WOOD
    { BLOOD_NONE, HIT_SOLID },  // COL_MATERIAL_HARD
    { BLOOD_NONE, HIT_WOOD },   // COL_MATERIAL_TREE
};

void func_80062CD4(GlobalContext* globalCtx, Vec3f* arg1);
void func_80062E14(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2);

void func_8005E604(GlobalContext* globalCtx, Collider* atCollider, ColliderElement* atElem, Collider* acCollider,
                   ColliderElement* acElem, Vec3f* arg5) {
    if (!(acElem->acElemFlags & ACELEM_NO_HITMARK) &&
        ((atElem->atElemFlags & ATELEM_AT_HITMARK) || !(atElem->atElemFlags & ATELEM_DREW_HITMARK))) {
        if (acCollider->actor != NULL) {
            sBloodFuncs[sHitInfo[acCollider->colMaterial].blood](globalCtx, acCollider, arg5);
        }
        do {
        } while (0);
        if (acCollider->actor != NULL) {
            if (sHitInfo[acCollider->colMaterial].effect == 3) {
                func_8005E2EC(globalCtx, atElem, acCollider, arg5);
            } else if (sHitInfo[acCollider->colMaterial].effect == 4) {
                if (atCollider->actor == NULL) {
                    func_80062CD4(globalCtx, arg5);
                    Audio_PlaySoundGeneral(NA_SE_IT_REFLECTION_WOOD, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
                } else {
                    func_80062E14(globalCtx, arg5, &atCollider->actor->unk_E4);
                }
            } else if (sHitInfo[acCollider->colMaterial].effect != 5) {
                func_80029CA4(globalCtx, sHitInfo[acCollider->colMaterial].effect, arg5);
                if (!(acElem->acElemFlags & ACELEM_NO_SWORD_SFX)) {
                    func_8005E4F8(atCollider, acElem);
                }
            }
        } else {
            func_80029CA4(globalCtx, 0U, arg5);
            if (acCollider->actor == NULL) {
                Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_BOUND, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
            } else {
                Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_BOUND, &acCollider->actor->unk_E4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
            }
        }
    }
}

void func_8005E800(Collider* arg0, Collider* arg1) {
    arg0->atFlags |= AT_BOUNCED;
    arg1->acFlags |= AC_BOUNCED;
}

s32 Collider_ATVsACHit(GlobalContext* globalCtx, Collider* atCollider, ColliderElement* atElem, Vec3f* atElemPos,
                       Collider* acCollider, ColliderElement* acElem, Vec3f* acElemPos, Vec3f* hitPos) {
    if ((acCollider->acFlags & AC_HARD) && (atCollider->actor != NULL) && (acCollider->actor != NULL)) {
        func_8005E800(atCollider, acCollider);
    }
    if (!(acElem->acElemFlags & ACELEM_NO_AT_INFO)) {
        atCollider->atFlags |= AT_HIT;
        atCollider->at = acCollider->actor;
        atElem->atHit = acCollider;
        atElem->atHitElem = acElem;
        atElem->atElemFlags |= ATELEM_HIT;
        if (atCollider->actor != NULL) {
            atCollider->actor->collideData.atHitBacklash = acElem->acDmgInfo.hitBacklash;
        }
    }
    acCollider->acFlags |= AC_HIT;
    acCollider->ac = atCollider->actor;
    acElem->acHit = atCollider;
    acElem->acHitElem = atElem;
    acElem->acElemFlags |= ACELEM_HIT;
    if (acCollider->actor != NULL) {
        acCollider->actor->collideData.acHitSpecialEffect = atElem->atDmgInfo.hitSpecialEffect;
    }
    acElem->acDmgInfo.hitPos.x = hitPos->x;
    acElem->acDmgInfo.hitPos.y = hitPos->y;
    acElem->acDmgInfo.hitPos.z = hitPos->z;
    if (!(atElem->atElemFlags & ATELEM_AT_HITMARK) && ((acCollider->colMaterial != COL_MATERIAL_METAL)) &&
        (acCollider->colMaterial != COL_MATERIAL_WOOD) && (acCollider->colMaterial != COL_MATERIAL_HARD)) {
        acElem->acElemFlags |= ACELEM_DRAW_HITMARK;
    } else {
        func_8005E604(globalCtx, atCollider, atElem, acCollider, acElem, hitPos);
        atElem->atElemFlags |= ATELEM_DREW_HITMARK;
    }
    return 1;
}

void Collider_ATSpheresVsACSpheres(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                   Collider* acCollider) {
    ColliderSpheres* atSpheres = (ColliderSpheres*)atCollider;
    ColliderSpheres* acSpheres = (ColliderSpheres*)acCollider;
    ColliderSpheresElement* atSpheresElem;
    ColliderSpheresElement* acSpheresElem;
    f32 sp8C;
    f32 sp88;
    f32 temp_fv0_2;
    Vec3f hitPos;
    Vec3f atSpheresElemPos;
    Vec3f acSpheresElemPos;

    if ((((atSpheres->nElements > 0) && (atSpheres->elements != NULL)) && (acSpheres->nElements > 0)) &&
        (acSpheres->elements != NULL)) {
        for (atSpheresElem = atSpheres->elements; atSpheresElem < (atSpheres->elements + atSpheres->nElements);
             atSpheresElem++) {
            if (Collider_IsElementATOff(&atSpheresElem->base) != 1) {
                for (acSpheresElem = acSpheres->elements; acSpheresElem < (acSpheres->elements + acSpheres->nElements);
                     acSpheresElem++) {
                    if (((Collider_IsElementACOff(&acSpheresElem->base) != 1) &&
                         (Collider_AreDmgFlagsDisjoint(&atSpheresElem->base, &acSpheresElem->base) != 1)) &&
                        (Math3D_SpheresTouchingSurfaceCenter(&atSpheresElem->shape.world, &acSpheresElem->shape.world,
                                                             &sp8C, &sp88) == 1)) {
                        atSpheresElemPos.x = atSpheresElem->shape.world.center.x;
                        atSpheresElemPos.y = atSpheresElem->shape.world.center.y;
                        atSpheresElemPos.z = atSpheresElem->shape.world.center.z;
                        acSpheresElemPos.x = acSpheresElem->shape.world.center.x;
                        acSpheresElemPos.y = acSpheresElem->shape.world.center.y;
                        acSpheresElemPos.z = acSpheresElem->shape.world.center.z;
                        if (!(fabsf(sp88) < 0.008f)) {
                            temp_fv0_2 = (f32)acSpheresElem->shape.world.radius / sp88;
                            hitPos.x = ((atSpheresElemPos.x - acSpheresElemPos.x) * temp_fv0_2) + acSpheresElemPos.x;
                            hitPos.y = ((atSpheresElemPos.y - acSpheresElemPos.y) * temp_fv0_2) + acSpheresElemPos.y;
                            hitPos.z = ((atSpheresElemPos.z - acSpheresElemPos.z) * temp_fv0_2) + acSpheresElemPos.z;
                        } else {
                            Math_Vec3f_Copy(&hitPos, &atSpheresElemPos);
                        }
                        Collider_ATVsACHit(globalCtx, &atSpheres->base, &atSpheresElem->base, &atSpheresElemPos,
                                           &acSpheres->base, &acSpheresElem->base, &acSpheresElemPos, &hitPos);
                        if (!(acSpheres->base.ocFlags2 & OC2_FIRST_ONLY)) {
                            return;
                        }
                    }
                }
            }
        }
    }
}

void Collider_ATSpheresVsACCylinder(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                    Collider* acCollider) {
    ColliderSpheres* atSpheres = (ColliderSpheres*)atCollider;
    ColliderCylinder* acCylinder = (ColliderCylinder*)acCollider;
    ColliderSpheresElement* atSpheresElem;
    f32 sp80;
    f32 sp7C;
    Vec3f hitPos;
    Vec3f atSpheresElemPos;
    Vec3f acCylinderPos;
    f32 temp_fv0_2;

    if ((atSpheres->nElements > 0) && (atSpheres->elements != NULL) && (acCylinder->shape.radius > 0) &&
        (acCylinder->shape.height > 0) && (Collider_IsElementACOff(&acCylinder->elem) != 1)) {
        if (&atSpheresElem->shape && &atSpheresElem->shape) {}
        for (atSpheresElem = atSpheres->elements; atSpheresElem < &atSpheres->elements[atSpheres->nElements];
             atSpheresElem++) {
            if ((Collider_IsElementATOff(&atSpheresElem->base) != 1) &&
                (Collider_AreDmgFlagsDisjoint(&atSpheresElem->base, &acCylinder->elem) != 1) &&
                (func_800CFDA4(&atSpheresElem->shape.world, &acCylinder->shape, &sp80, &sp7C) != 0)) {
                atSpheresElemPos.x = atSpheresElem->shape.world.center.x;
                atSpheresElemPos.y = atSpheresElem->shape.world.center.y;
                atSpheresElemPos.z = atSpheresElem->shape.world.center.z;
                acCylinderPos.x = acCylinder->shape.pos.x;
                acCylinderPos.y = acCylinder->shape.pos.y;
                acCylinderPos.z = acCylinder->shape.pos.z;
                if (!(fabsf(sp7C) < 0.008f)) {
                    temp_fv0_2 = (f32)acCylinder->shape.radius / sp7C;
                    if (temp_fv0_2 <= 1.0f) {
                        hitPos.x = ((atSpheresElemPos.x - acCylinderPos.x) * temp_fv0_2) + acCylinderPos.x;
                        hitPos.y = ((atSpheresElemPos.y - acCylinderPos.y) * temp_fv0_2) + acCylinderPos.y;
                        hitPos.z = ((atSpheresElemPos.z - acCylinderPos.z) * temp_fv0_2) + acCylinderPos.z;
                    } else {
                        Math_Vec3f_Copy(&hitPos, &atSpheresElemPos);
                    }
                } else {
                    Math_Vec3f_Copy(&hitPos, &atSpheresElemPos);
                }
                Collider_ATVsACHit(globalCtx, &atSpheres->base, &atSpheresElem->base, &atSpheresElemPos,
                                   &acCylinder->base, &acCylinder->elem, &acCylinderPos, &hitPos);
                return;
            }
        }
    }
}

void Collider_ATCylinderVsACSpheres(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                    Collider* acCollider) {
    ColliderCylinder* atCylinder = (ColliderCylinder*)atCollider;
    ColliderSpheres* acSpheres = (ColliderSpheres*)acCollider;
    f32 sp9C;
    f32 sp98;
    ColliderSpheresElement* acSpheresElem;
    Vec3f hitPos;
    Vec3f atCylinderPos;
    Vec3f acSpheresElemPos;
    f32 temp_fv0_2;

    if ((acSpheres->nElements > 0) && (acSpheres->elements != 0) && (atCylinder->shape.radius > 0)) {
        if ((atCylinder->shape.height > 0) && (Collider_IsElementATOff(&atCylinder->elem) != 1)) {
            for (acSpheresElem = acSpheres->elements; acSpheresElem < (acSpheres->elements + (acSpheres->nElements));
                 acSpheresElem++) {
                if ((Collider_IsElementACOff(&acSpheresElem->base) != 1) &&
                    (Collider_AreDmgFlagsDisjoint(&atCylinder->elem, &acSpheresElem->base) != 1) &&
                    (func_800CFDA4(&acSpheresElem->shape.world, &atCylinder->shape, &sp9C, &sp98) != 0)) {
                    atCylinderPos.x = (f32)atCylinder->shape.pos.x;
                    atCylinderPos.y = (f32)atCylinder->shape.pos.y;
                    atCylinderPos.z = (f32)atCylinder->shape.pos.z;
                    acSpheresElemPos.x = (f32)acSpheresElem->shape.world.center.x;
                    acSpheresElemPos.y = (f32)acSpheresElem->shape.world.center.y;
                    acSpheresElemPos.z = (f32)acSpheresElem->shape.world.center.z;
                    if (!(fabsf(sp98) < 0.008f)) {
                        temp_fv0_2 = (f32)acSpheresElem->shape.world.radius / sp98;
                        if (temp_fv0_2 <= 1.0f) {
                            hitPos.x = ((atCylinderPos.x - acSpheresElemPos.x) * temp_fv0_2) + acSpheresElemPos.x;
                            hitPos.y = ((atCylinderPos.y - acSpheresElemPos.y) * temp_fv0_2) + acSpheresElemPos.y;
                            hitPos.z = ((atCylinderPos.z - acSpheresElemPos.z) * temp_fv0_2) + acSpheresElemPos.z;
                        } else {
                            Math_Vec3f_Copy(&hitPos, &atCylinderPos);
                        }
                    } else {
                        Math_Vec3f_Copy(&hitPos, &atCylinderPos);
                    }
                    Collider_ATVsACHit(globalCtx, &atCylinder->base, &atCylinder->elem, &atCylinderPos,
                                       &acSpheres->base, &acSpheresElem->base, &acSpheresElemPos, &hitPos);
                    if (!(acSpheres->base.ocFlags2 & OC2_FIRST_ONLY)) {
                        return;
                    }
                }
            }
        }
    }
}

void Collider_ATSpheresVsACTris(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                Collider* acCollider) {
    ColliderSpheres* atSpheres = (ColliderSpheres*)atCollider;
    ColliderTris* acTris = (ColliderTris*)acCollider;
    ColliderSpheresElement* atSpheresElem;
    ColliderTrisElement* acTrisElem;
    Vec3f hitPos;
    Vec3f sp60;
    Vec3f sp54;

    if ((atSpheres->nElements > 0)) {
        if ((atSpheres->elements != NULL) && (acTris->nElements > 0) && (acTris->elements != NULL)) {
            for (atSpheresElem = atSpheres->elements; atSpheresElem < (atSpheres->elements + atSpheres->nElements);
                 atSpheresElem++) {
                if (Collider_IsElementATOff(&atSpheresElem->base) != 1) {
                    for (acTrisElem = acTris->elements; acTrisElem < (acTris->elements + acTris->nElements);
                         acTrisElem++) {
                        if ((Collider_IsElementACOff(&acTrisElem->base) != 1) &&
                            (Collider_AreDmgFlagsDisjoint(&atSpheresElem->base, &acTrisElem->base) != 1) &&
                            (func_800CE934(&atSpheresElem->shape.world, &acTrisElem->shape, &hitPos) == 1)) {
                            sp60.x = atSpheresElem->shape.world.center.x;
                            sp60.y = atSpheresElem->shape.world.center.y;
                            sp60.z = atSpheresElem->shape.world.center.z;
                            sp54.x =
                                (acTrisElem->shape.vtx[0].x + acTrisElem->shape.vtx[1].x + acTrisElem->shape.vtx[2].x) *
                                (1.0f / 3.0f);
                            sp54.y =
                                (acTrisElem->shape.vtx[0].y + acTrisElem->shape.vtx[1].y + acTrisElem->shape.vtx[2].y) *
                                (1.0f / 3.0f);
                            sp54.z =
                                (acTrisElem->shape.vtx[0].z + acTrisElem->shape.vtx[1].z + acTrisElem->shape.vtx[2].z) *
                                (1.0f / 3.0f);
                            Collider_ATVsACHit(globalCtx, &atSpheres->base, &atSpheresElem->base, &sp60, &acTris->base,
                                               &acTrisElem->base, &sp54, &hitPos);
                            return;
                        }
                    }
                }
                if (0) {}
            }
        }
    }
}

void Collider_ATTrisVsACSpheres(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                Collider* acCollider) {
    ColliderTris* atTris = (ColliderTris*)atCollider;
    ColliderTrisElement* atTrisElem;
    ColliderSpheres* acSpheres = (ColliderSpheres*)acCollider;
    ColliderSpheresElement* acSpheresElem;
    Vec3f hitPos;
    Vec3f atTrisElemPos;
    Vec3f acSpheresElemPos;

    if (acSpheres->nElements > 0) {
        if ((acSpheres->elements != NULL) && (atTris->nElements > 0) && (atTris->elements != 0)) {
            for (acSpheresElem = acSpheres->elements; acSpheresElem < (acSpheres->elements + acSpheres->nElements);
                 acSpheresElem++) {
                if ((Collider_IsElementACOff(&acSpheresElem->base) != 1)) {
                    for (atTrisElem = atTris->elements; atTrisElem < (atTris->elements + atTris->nElements);
                         atTrisElem++) {
                        if (((Collider_IsElementATOff(&atTrisElem->base) != 1))) {
                            if (((Collider_AreDmgFlagsDisjoint(&atTrisElem->base, &acSpheresElem->base) != 1) &&
                                 (func_800CE934(&acSpheresElem->shape.world, &atTrisElem->shape, &hitPos) == 1))) {
                                Math_Vec3s_ToVec3f(&acSpheresElemPos, &acSpheresElem->shape.world.center);
                                atTrisElemPos.x = (atTrisElem->shape.vtx[0].x + atTrisElem->shape.vtx[1].x +
                                                   atTrisElem->shape.vtx[2].x) *
                                                  (1.0f / 3.0f);
                                atTrisElemPos.y = (atTrisElem->shape.vtx[0].y + atTrisElem->shape.vtx[1].y +
                                                   atTrisElem->shape.vtx[2].y) *
                                                  (1.0f / 3.0f);
                                atTrisElemPos.z = (atTrisElem->shape.vtx[0].z + atTrisElem->shape.vtx[1].z +
                                                   atTrisElem->shape.vtx[2].z) *
                                                  (1.0f / 3.0f);
                                Collider_ATVsACHit(globalCtx, &atTris->base, &atTrisElem->base, &atTrisElemPos,
                                                   &acSpheres->base, &acSpheresElem->base, &acSpheresElemPos, &hitPos);
                                if (!(acSpheres->base.ocFlags2 & OC2_FIRST_ONLY)) {
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

TriNorm D_8015E230;
BSS_DUMMY;
BSS_DUMMY;
BSS_DUMMY;
BSS_DUMMY;
TriNorm D_8015E268;
BSS_DUMMY;
BSS_DUMMY;
BSS_DUMMY;
BSS_DUMMY;

void Collider_ATSpheresVsACQuad(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                Collider* acCollider) {
    ColliderSpheres* atSpheres = (ColliderSpheres*)atCollider;
    ColliderQuad* acQuad = (ColliderQuad*)acCollider;
    Vec3f hitPos;
    ColliderSpheresElement* atSpheresElem;
    Vec3f atSpheresElemPos;
    Vec3f acQuadPos;

    if ((atSpheres->nElements > 0) && ((atSpheres->elements != NULL)) &&
        (Collider_IsElementACOff(&acQuad->elem) != 1)) {
        Math3D_TriNorm(&D_8015E230, &acQuad->shape.corners.cornerA, &acQuad->shape.corners.cornerB,
                       &acQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E268, &acQuad->shape.corners.cornerC, &acQuad->shape.corners.cornerD,
                       &acQuad->shape.corners.cornerA);
        for (atSpheresElem = atSpheres->elements; atSpheresElem < &atSpheres->elements[atSpheres->nElements];
             atSpheresElem++) {
            if (Collider_IsElementATOff(&atSpheresElem->base) != 1) {
                if ((Collider_AreDmgFlagsDisjoint(&atSpheresElem->base, &acQuad->elem) != 1) &&
                    ((func_800CE934(&atSpheresElem->shape.world, &D_8015E230, &hitPos) == 1) ||
                     (func_800CE934(&atSpheresElem->shape.world, &D_8015E268, &hitPos) == 1))) {
                    Math_Vec3s_ToVec3f(&atSpheresElemPos, &atSpheresElem->shape.world.center);
                    acQuadPos.x = (acQuad->shape.corners.cornerA.x + acQuad->shape.corners.cornerB.x +
                                   acQuad->shape.corners.cornerC.x + acQuad->shape.corners.cornerD.x) *
                                  0.25f;
                    acQuadPos.y = (acQuad->shape.corners.cornerA.y + acQuad->shape.corners.cornerB.y +
                                   acQuad->shape.corners.cornerC.y + acQuad->shape.corners.cornerD.y) *
                                  0.25f;
                    acQuadPos.z = (acQuad->shape.corners.cornerA.z + acQuad->shape.corners.cornerB.z +
                                   acQuad->shape.corners.cornerC.z + acQuad->shape.corners.cornerD.z) *
                                  0.25f;
                    Collider_ATVsACHit(globalCtx, &atSpheres->base, &atSpheresElem->base, &atSpheresElemPos,
                                       &acQuad->base, &acQuad->elem, &acQuadPos, &hitPos);
                    return;
                }
            }
            if (1) {}
        }
    }
}

TriNorm D_8015E2A0;
TriNorm D_8015E2D8;

void Collider_ATQuadVsACSpheres(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                Collider* acCollider) {
    ColliderQuad* atQuad = (ColliderQuad*)atCollider;
    Vec3f hitPos;
    ColliderSpheres* acSpheres = (ColliderSpheres*)acCollider;
    ColliderSpheresElement* acSpheresElem;
    Vec3f atQuadPos;
    Vec3f acSpheresElemPos;

    if (acSpheres->nElements > 0) {
        if ((acSpheres->elements != NULL) && (Collider_IsElementATOff(&atQuad->elem) != 1)) {
            Math3D_TriNorm(&D_8015E2A0, &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerB,
                           &atQuad->shape.corners.cornerC);
            Math3D_TriNorm(&D_8015E2D8, &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerC,
                           &atQuad->shape.corners.cornerD);

            for (acSpheresElem = acSpheres->elements; acSpheresElem < &acSpheres->elements[acSpheres->nElements];
                 acSpheresElem++) {
                if ((Collider_IsElementACOff(&acSpheresElem->base) != 1) &&
                    ((Collider_AreDmgFlagsDisjoint(&atQuad->elem, &acSpheresElem->base) != 1)) &&
                    ((func_800CE934(&acSpheresElem->shape.world, &D_8015E2A0, &hitPos) == 1) ||
                     (func_800CE934(&acSpheresElem->shape.world, &D_8015E2D8, &hitPos) == 1)) &&
                    func_8005D218(globalCtx, atQuad, &hitPos)) {
                    acSpheresElemPos.x = acSpheresElem->shape.world.center.x;
                    acSpheresElemPos.y = acSpheresElem->shape.world.center.y;
                    acSpheresElemPos.z = acSpheresElem->shape.world.center.z;
                    atQuadPos.x = (atQuad->shape.corners.cornerA.x + atQuad->shape.corners.cornerB.x +
                                   atQuad->shape.corners.cornerC.x + atQuad->shape.corners.cornerD.x) *
                                  0.25f;
                    atQuadPos.y = (atQuad->shape.corners.cornerA.y + atQuad->shape.corners.cornerB.y +
                                   atQuad->shape.corners.cornerC.y + atQuad->shape.corners.cornerD.y) *
                                  0.25f;
                    atQuadPos.z = (atQuad->shape.corners.cornerA.z + atQuad->shape.corners.cornerB.z +
                                   atQuad->shape.corners.cornerC.z + atQuad->shape.corners.cornerD.z) *
                                  0.25f;
                    Collider_ATVsACHit(globalCtx, &atQuad->base, &atQuad->elem, &atQuadPos, &acSpheres->base,
                                       &acSpheresElem->base, &acSpheresElemPos, &hitPos);
                    if (!(acSpheres->base.ocFlags2 & OC2_FIRST_ONLY)) {
                        return;
                    }
                }
                if (1) {}
            }
        }
    }
}

void Collider_ATCylinderVsACCylinder(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                     Collider* acCollider) {
    ColliderCylinder* atCylinder = (ColliderCylinder*)atCollider;
    ColliderCylinder* acCylinder = (ColliderCylinder*)acCollider;
    f32 sp6C;
    f32 sp68;
    Vec3f hitPos;
    Vec3f atCylinderPos;
    Vec3f acCylinderPos;
    f32 temp_fv0;

    if ((atCylinder->shape.radius > 0) && (atCylinder->shape.height > 0) && (acCylinder->shape.radius > 0) &&
        (acCylinder->shape.height > 0)) {
        if (Collider_IsElementACOff(&acCylinder->elem) != 1) {
            if ((Collider_IsElementATOff(&atCylinder->elem) != 1) &&
                (Collider_AreDmgFlagsDisjoint(&atCylinder->elem, &acCylinder->elem) != 1) &&
                (Math3D_CylinderOutCylinderDist(&atCylinder->shape, &acCylinder->shape, &sp6C, &sp68) == 1)) {
                Math_Vec3s_ToVec3f(&atCylinderPos, &atCylinder->shape.pos);
                Math_Vec3s_ToVec3f(&acCylinderPos, &acCylinder->shape.pos);
                if (!(fabsf(sp68) < 0.008f)) {
                    temp_fv0 = (f32)acCylinder->shape.radius / sp68;
                    hitPos.y = (f32)acCylinder->shape.pos.y + (f32)acCylinder->shape.yShift +
                               ((f32)acCylinder->shape.height * 0.5f);
                    hitPos.x = (((f32)atCylinder->shape.pos.x - (f32)acCylinder->shape.pos.x) * temp_fv0) +
                               (f32)acCylinder->shape.pos.x;
                    hitPos.z = (((f32)atCylinder->shape.pos.z - (f32)acCylinder->shape.pos.z) * temp_fv0) +
                               (f32)acCylinder->shape.pos.z;
                } else {
                    Math_Vec3s_ToVec3f(&hitPos, &acCylinder->shape.pos);
                }
                Collider_ATVsACHit(globalCtx, &atCylinder->base, &atCylinder->elem, &atCylinderPos, &acCylinder->base,
                                   &acCylinder->elem, &acCylinderPos, &hitPos);
            }
        }
    }
}

void Collider_ATCylinderVsACTris(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                 Collider* acCollider) {
    ColliderCylinder* atCylinder = (ColliderCylinder*)atCollider;
    ColliderTris* acTris = (ColliderTris*)acCollider;
    ColliderTrisElement* acTrisElem;
    Vec3f hitPos;
    Vec3f atCylinderPos;
    Vec3f acTrisElemPos;

    if ((atCylinder->shape.radius > 0) && (atCylinder->shape.height > 0) && (acTris->nElements > 0) &&
        ((acTris->elements != NULL)) && (Collider_IsElementATOff(&atCylinder->elem) != 1)) {
        for (acTrisElem = acTris->elements; acTrisElem < &acTris->elements[acTris->nElements]; acTrisElem++) {
            if ((Collider_IsElementACOff(&acTrisElem->base) != 1) &&
                (Collider_AreDmgFlagsDisjoint(&atCylinder->elem, &acTrisElem->base) != 1) &&
                (Math3D_CylTriTouchingIntersect(&atCylinder->shape, &acTrisElem->shape, &hitPos) == 1)) {
                Math_Vec3s_ToVec3f(&atCylinderPos, &atCylinder->shape.pos);
                acTrisElemPos.x =
                    (acTrisElem->shape.vtx[0].x + acTrisElem->shape.vtx[1].x + acTrisElem->shape.vtx[2].x) *
                    (1.0f / 3.0f);
                acTrisElemPos.y =
                    (acTrisElem->shape.vtx[0].y + acTrisElem->shape.vtx[1].y + acTrisElem->shape.vtx[2].y) *
                    (1.0f / 3.0f);
                acTrisElemPos.z =
                    (acTrisElem->shape.vtx[0].z + acTrisElem->shape.vtx[1].z + acTrisElem->shape.vtx[2].z) *
                    (1.0f / 3.0f);
                Collider_ATVsACHit(globalCtx, &atCylinder->base, &atCylinder->elem, &atCylinderPos, &acTris->base,
                                   &acTrisElem->base, &acTrisElemPos, &hitPos);
                return;
            }
            if (1) {}
            if (1) {}
        }
    }
}

void Collider_ATTrisVsACCylinder(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                 Collider* acCollider) {
    static Vec3f sHitPos;

    ColliderTris* atTris = (ColliderTris*)atCollider;
    ColliderCylinder* acCylinder = (ColliderCylinder*)acCollider;
    ColliderTrisElement* atTrisElem;
    Vec3f atTrisElemPos;
    Vec3f acCylinderPos;

    if ((acCylinder->shape.radius > 0) && (acCylinder->shape.height > 0) && (atTris->nElements > 0) &&
        ((atTris->elements != NULL)) && (Collider_IsElementACOff(&acCylinder->elem) != 1)) {

        for (atTrisElem = atTris->elements; atTrisElem < &atTris->elements[atTris->nElements]; atTrisElem++) {
            if ((Collider_IsElementATOff(&atTrisElem->base) != 1) &&
                (Collider_AreDmgFlagsDisjoint(&atTrisElem->base, &acCylinder->elem) != 1) &&
                (Math3D_CylTriTouchingIntersect(&acCylinder->shape, &atTrisElem->shape, &sHitPos) == 1)) {
                atTrisElemPos.x =
                    (atTrisElem->shape.vtx[0].x + atTrisElem->shape.vtx[1].x + atTrisElem->shape.vtx[2].x) *
                    (1.0f / 3.0f);
                atTrisElemPos.y =
                    (atTrisElem->shape.vtx[0].y + atTrisElem->shape.vtx[1].y + atTrisElem->shape.vtx[2].y) *
                    (1.0f / 3.0f);
                atTrisElemPos.z =
                    (atTrisElem->shape.vtx[0].z + atTrisElem->shape.vtx[1].z + atTrisElem->shape.vtx[2].z) *
                    (1.0f / 3.0f);
                Math_Vec3s_ToVec3f(&acCylinderPos, &acCylinder->shape.pos);
                Collider_ATVsACHit(globalCtx, &atTris->base, &atTrisElem->base, &atTrisElemPos, &acCylinder->base,
                                   &acCylinder->elem, &acCylinderPos, &sHitPos);
                return;
            }
            if (1) {}
            if (1) {}
        }
    }
}

void Collider_ATCylinderVsACQuad(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                 Collider* acCollider) {
    static TriNorm D_8015E320;
    static TriNorm D_8015E358;
    static Vec3f sHitPos;

    ColliderCylinder* atCylinder = (ColliderCylinder*)atCollider;
    ColliderQuad* acQuad = (ColliderQuad*)acCollider;

    if ((atCylinder->shape.height > 0) && (atCylinder->shape.radius > 0) &&
        (Collider_IsElementATOff(&atCylinder->elem) != 1) && (Collider_IsElementACOff(&acQuad->elem) != 1) &&
        (Collider_AreDmgFlagsDisjoint(&atCylinder->elem, &acQuad->elem) != 1)) {
        Math3D_TriNorm(&D_8015E320, &acQuad->shape.corners.cornerA, &acQuad->shape.corners.cornerB,
                       &acQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E358, &acQuad->shape.corners.cornerC, &acQuad->shape.corners.cornerD,
                       &acQuad->shape.corners.cornerA);
        if (Math3D_CylTriTouchingIntersect(&atCylinder->shape, &D_8015E320, &sHitPos) == 1) {
            Vec3f atCylinderPos;
            Vec3f acQuadPos;

            Math_Vec3s_ToVec3f(&atCylinderPos, &atCylinder->shape.pos);
            acQuadPos.x = (acQuad->shape.corners.cornerA.x + acQuad->shape.corners.cornerB.x +
                           acQuad->shape.corners.cornerC.x + acQuad->shape.corners.cornerD.x) *
                          0.25f;
            acQuadPos.y = (acQuad->shape.corners.cornerA.y + acQuad->shape.corners.cornerB.y +
                           acQuad->shape.corners.cornerC.y + acQuad->shape.corners.cornerD.y) *
                          0.25f;
            acQuadPos.z = (acQuad->shape.corners.cornerA.z + acQuad->shape.corners.cornerB.z +
                           acQuad->shape.corners.cornerC.z + acQuad->shape.corners.cornerD.z) *
                          0.25f;
            Collider_ATVsACHit(globalCtx, &atCylinder->base, &atCylinder->elem, &atCylinderPos, &acQuad->base,
                               &acQuad->elem, &acQuadPos, &sHitPos);
        } else if (Math3D_CylTriTouchingIntersect(&atCylinder->shape, &D_8015E358, &sHitPos) == 1) {
            Vec3f atCylinderPos;
            Vec3f acQuadPos;

            Math_Vec3s_ToVec3f(&atCylinderPos, &atCylinder->shape.pos);
            acQuadPos.x = (acQuad->shape.corners.cornerA.x + acQuad->shape.corners.cornerB.x +
                           acQuad->shape.corners.cornerC.x + acQuad->shape.corners.cornerD.x) *
                          0.25f;
            acQuadPos.y = (acQuad->shape.corners.cornerA.y + acQuad->shape.corners.cornerB.y +
                           acQuad->shape.corners.cornerC.y + acQuad->shape.corners.cornerD.y) *
                          0.25f;
            acQuadPos.z = (acQuad->shape.corners.cornerA.z + acQuad->shape.corners.cornerB.z +
                           acQuad->shape.corners.cornerC.z + acQuad->shape.corners.cornerD.z) *
                          0.25f;
            Collider_ATVsACHit(globalCtx, &atCylinder->base, &atCylinder->elem, &atCylinderPos, &acQuad->base,
                               &acQuad->elem, &acQuadPos, &sHitPos);
        }
    }
}

void Collider_ATQuadVsACCylinder(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                                 Collider* acCollider) {
    static TriNorm D_8015E3A0;
    static TriNorm D_8015E3D8;
    static Vec3f sHitPos;

    ColliderQuad* atQuad = (ColliderQuad*)atCollider;
    ColliderCylinder* acCylinder = (ColliderCylinder*)acCollider;

    if ((acCylinder->shape.height > 0) && ((acCylinder->shape.radius > 0)) &&
        (Collider_IsElementACOff(&acCylinder->elem) != 1) && (Collider_IsElementATOff(&atQuad->elem) != 1) &&
        (Collider_AreDmgFlagsDisjoint(&atQuad->elem, &acCylinder->elem) != 1)) {
        Math3D_TriNorm(&D_8015E3A0, &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerB,
                       &atQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E3D8, &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerC,
                       &atQuad->shape.corners.cornerD);
        if ((Math3D_CylTriTouchingIntersect(&acCylinder->shape, &D_8015E3A0, &sHitPos) == 1) &&
            func_8005D218(globalCtx, atQuad, &sHitPos)) {
            Vec3f atQuadPos;
            Vec3f acCylinderPos;

            atQuadPos.x = (atQuad->shape.corners.cornerA.x + atQuad->shape.corners.cornerB.x +
                           atQuad->shape.corners.cornerC.x + atQuad->shape.corners.cornerD.x) *
                          0.25f;
            atQuadPos.y = (atQuad->shape.corners.cornerA.y + atQuad->shape.corners.cornerB.y +
                           atQuad->shape.corners.cornerC.y + atQuad->shape.corners.cornerD.y) *
                          0.25f;
            atQuadPos.z = (atQuad->shape.corners.cornerA.z + atQuad->shape.corners.cornerB.z +
                           atQuad->shape.corners.cornerC.z + atQuad->shape.corners.cornerD.z) *
                          0.25f;
            Math_Vec3s_ToVec3f(&acCylinderPos, &acCylinder->shape.pos);
            Collider_ATVsACHit(globalCtx, &atQuad->base, &atQuad->elem, &atQuadPos, &acCylinder->base,
                               &acCylinder->elem, &acCylinderPos, &sHitPos);
        } else if ((Math3D_CylTriTouchingIntersect(&acCylinder->shape, &D_8015E3D8, &sHitPos) == 1) &&
                   func_8005D218(globalCtx, atQuad, &sHitPos)) {
            Vec3f atQuadPos;
            Vec3f acCylinderPos;

            atQuadPos.x = (atQuad->shape.corners.cornerA.x + atQuad->shape.corners.cornerB.x +
                           atQuad->shape.corners.cornerC.x + atQuad->shape.corners.cornerD.x) *
                          0.25f;
            atQuadPos.y = (atQuad->shape.corners.cornerA.y + atQuad->shape.corners.cornerB.y +
                           atQuad->shape.corners.cornerC.y + atQuad->shape.corners.cornerD.y) *
                          0.25f;
            atQuadPos.z = (atQuad->shape.corners.cornerA.z + atQuad->shape.corners.cornerB.z +
                           atQuad->shape.corners.cornerC.z + atQuad->shape.corners.cornerD.z) *
                          0.25f;
            Math_Vec3s_ToVec3f(&acCylinderPos, &acCylinder->shape.pos);
            Collider_ATVsACHit(globalCtx, &atQuad->base, &atQuad->elem, &atQuadPos, &acCylinder->base,
                               &acCylinder->elem, &acCylinderPos, &sHitPos);
        }
    }
}

void Collider_ATTrisVsACTris(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                             Collider* acCollider) {
    static Vec3f sHitPos;

    ColliderTris* atTris = (ColliderTris*)atCollider;
    ColliderTris* acTris = (ColliderTris*)acCollider;
    ColliderTrisElement* atTrisElem;
    ColliderTrisElement* acTrisElem;
    Vec3f atTrisElemPos;
    Vec3f acTrisElemPos;

    if ((acTris->nElements > 0) && (acTris->elements != NULL) && (atTris->nElements > 0) &&
        (atTris->elements != NULL)) {

        for (acTrisElem = acTris->elements; acTrisElem < &acTris->elements[acTris->nElements]; acTrisElem++) {
            if (Collider_IsElementACOff(&acTrisElem->base) != 1) {
                for (atTrisElem = atTris->elements; atTrisElem < &atTris->elements[atTris->nElements]; atTrisElem++) {
                    if (0) {}
                    if ((Collider_IsElementATOff(&atTrisElem->base) != 1) &&
                        (Collider_AreDmgFlagsDisjoint(&atTrisElem->base, &acTrisElem->base) != 1) &&
                        (Math3D_TrisIntersect(&atTrisElem->shape, &acTrisElem->shape, &sHitPos) == 1)) {
                        atTrisElemPos.x =
                            (atTrisElem->shape.vtx[0].x + atTrisElem->shape.vtx[1].x + atTrisElem->shape.vtx[2].x) *
                            (1.0f / 3.0f);
                        atTrisElemPos.y =
                            (atTrisElem->shape.vtx[0].y + atTrisElem->shape.vtx[1].y + atTrisElem->shape.vtx[2].y) *
                            (1.0f / 3.0f);
                        atTrisElemPos.z =
                            (atTrisElem->shape.vtx[0].z + atTrisElem->shape.vtx[1].z + atTrisElem->shape.vtx[2].z) *
                            (1.0f / 3.0f);
                        acTrisElemPos.x =
                            (acTrisElem->shape.vtx[0].x + acTrisElem->shape.vtx[1].x + acTrisElem->shape.vtx[2].x) *
                            (1.0f / 3.0f);
                        acTrisElemPos.y =
                            (acTrisElem->shape.vtx[0].y + acTrisElem->shape.vtx[1].y + acTrisElem->shape.vtx[2].y) *
                            (1.0f / 3.0f);
                        acTrisElemPos.z =
                            (acTrisElem->shape.vtx[0].z + acTrisElem->shape.vtx[1].z + acTrisElem->shape.vtx[2].z) *
                            (1.0f / 3.0f);
                        Collider_ATVsACHit(globalCtx, &atTris->base, &atTrisElem->base, &atTrisElemPos, &acTris->base,
                                           &acTrisElem->base, &acTrisElemPos, &sHitPos);
                        return;
                    }
                }
            }
        }
    }
}

void Collider_ATTrisVsACQuad(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                             Collider* acCollider) {
    static Vec3f sHitPos;
    static TriNorm D_8015E440;
    static TriNorm D_8015E478;

    ColliderTris* atTris = (ColliderTris*)atCollider;
    ColliderQuad* acQuad = (ColliderQuad*)acCollider;
    ColliderTrisElement* atTrisElem;
    Vec3f acTrisElemPos;
    Vec3f acQuadPos;

    if ((atTris->nElements > 0) && (atTris->elements != NULL) && (Collider_IsElementACOff(&acQuad->elem) != 1)) {
        Math3D_TriNorm(&D_8015E440, &acQuad->shape.corners.cornerA, &acQuad->shape.corners.cornerB,
                       &acQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E478, &acQuad->shape.corners.cornerC, &acQuad->shape.corners.cornerD,
                       &acQuad->shape.corners.cornerA);
        for (atTrisElem = atTris->elements; atTrisElem < &atTris->elements[atTris->nElements]; atTrisElem++) {
            if ((Collider_IsElementATOff(&atTrisElem->base) != 1) &&
                (Collider_AreDmgFlagsDisjoint(&atTrisElem->base, &acQuad->elem) != 1) &&
                ((Math3D_TrisIntersect(&D_8015E440, &atTrisElem->shape, &sHitPos) == 1) ||
                 (Math3D_TrisIntersect(&D_8015E478, &atTrisElem->shape, &sHitPos) == 1))) {
                acTrisElemPos.x =
                    (atTrisElem->shape.vtx[0].x + atTrisElem->shape.vtx[1].x + atTrisElem->shape.vtx[2].x) *
                    (1.0f / 3.0f);
                acTrisElemPos.y =
                    (atTrisElem->shape.vtx[0].y + atTrisElem->shape.vtx[1].y + atTrisElem->shape.vtx[2].y) *
                    (1.0f / 3.0f);
                acTrisElemPos.z =
                    (atTrisElem->shape.vtx[0].z + atTrisElem->shape.vtx[1].z + atTrisElem->shape.vtx[2].z) *
                    (1.0f / 3.0f);
                acQuadPos.x = (acQuad->shape.corners.cornerA.x + acQuad->shape.corners.cornerB.x +
                               acQuad->shape.corners.cornerC.x + acQuad->shape.corners.cornerD.x) *
                              0.25f;
                acQuadPos.y = (acQuad->shape.corners.cornerA.y + acQuad->shape.corners.cornerB.y +
                               acQuad->shape.corners.cornerC.y + acQuad->shape.corners.cornerD.y) *
                              0.25f;
                acQuadPos.z = (acQuad->shape.corners.cornerA.z + acQuad->shape.corners.cornerB.z +
                               acQuad->shape.corners.cornerC.z + acQuad->shape.corners.cornerD.z) *
                              0.25f;
                Collider_ATVsACHit(globalCtx, &atTris->base, &atTrisElem->base, &acTrisElemPos, &acQuad->base,
                                   &acQuad->elem, &acQuadPos, &sHitPos);
                return;
            }
            if (1) {}
            if (1) {}
            if (1) {}
            if (1) {}
            if (1) {}
            if (1) {}
        }
    }
}

void Collider_ATQuadVsACTris(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                             Collider* acCollider) {
    static Vec3f sHitPos;
    static TriNorm D_8015E4C0;
    static TriNorm D_8015E4F8;

    ColliderQuad* atQuad = (ColliderQuad*)atCollider;
    ColliderTris* acTris = (ColliderTris*)acCollider;
    ColliderTrisElement* acTrisElem;
    Vec3f atQuadPos;
    Vec3f acTrisElemPos;

    if ((acTris->nElements > 0) && (acTris->elements != NULL) && (Collider_IsElementATOff(&atQuad->elem) != 1)) {
        Math3D_TriNorm(&D_8015E4C0, &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerB,
                       &atQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E4F8, &atQuad->shape.corners.cornerC, &atQuad->shape.corners.cornerD,
                       &atQuad->shape.corners.cornerA);
        if (acTrisElem && acTrisElem) {}
        for (acTrisElem = acTris->elements; acTrisElem < &acTris->elements[acTris->nElements]; acTrisElem++) {
            if ((Collider_IsElementACOff(&acTrisElem->base) != 1) &&
                (Collider_AreDmgFlagsDisjoint(&atQuad->elem, &acTrisElem->base) != 1) &&
                ((Math3D_TrisIntersect(&D_8015E4C0, &acTrisElem->shape, &sHitPos) == 1) ||
                 (Math3D_TrisIntersect(&D_8015E4F8, &acTrisElem->shape, &sHitPos) == 1)) &&
                func_8005D218(globalCtx, atQuad, &sHitPos)) {
                acTrisElemPos.x =
                    (acTrisElem->shape.vtx[0].x + acTrisElem->shape.vtx[1].x + acTrisElem->shape.vtx[2].x) *
                    (1.0f / 3.0f);
                acTrisElemPos.y =
                    (acTrisElem->shape.vtx[0].y + acTrisElem->shape.vtx[1].y + acTrisElem->shape.vtx[2].y) *
                    (1.0f / 3.0f);
                acTrisElemPos.z =
                    (acTrisElem->shape.vtx[0].z + acTrisElem->shape.vtx[1].z + acTrisElem->shape.vtx[2].z) *
                    (1.0f / 3.0f);
                atQuadPos.x = (atQuad->shape.corners.cornerA.x + atQuad->shape.corners.cornerB.x +
                               atQuad->shape.corners.cornerC.x + atQuad->shape.corners.cornerD.x) *
                              0.25f;
                atQuadPos.y = (atQuad->shape.corners.cornerA.y + atQuad->shape.corners.cornerB.y +
                               atQuad->shape.corners.cornerC.y + atQuad->shape.corners.cornerD.y) *
                              0.25f;
                atQuadPos.z = (atQuad->shape.corners.cornerA.z + atQuad->shape.corners.cornerB.z +
                               atQuad->shape.corners.cornerC.z + atQuad->shape.corners.cornerD.z) *
                              0.25f;
                Collider_ATVsACHit(globalCtx, &atQuad->base, &atQuad->elem, &atQuadPos, &acTris->base,
                                   &acTrisElem->base, &acTrisElemPos, &sHitPos);
                return;
            }
        }
    }
}

void Collider_ATQuadVsACQuad(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider,
                             Collider* acCollider) {
    static TriNorm D_8015E530[2];
    static Vec3f sHitPos;
    static TriNorm D_8015E5A8[2];

    ColliderQuad* atQuad = (ColliderQuad*)atCollider;
    ColliderQuad* acQuad = (ColliderQuad*)acCollider;
    s32 i;
    s32 j;
    Vec3f atQuadPos;
    Vec3f acQuadPos;

    if ((Collider_IsElementATOff(&atQuad->elem) != 1) && (Collider_IsElementACOff(&acQuad->elem) != 1) &&
        (Collider_AreDmgFlagsDisjoint(&atQuad->elem, &acQuad->elem) != 1)) {
        Math3D_TriNorm(&D_8015E5A8[0], &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerB,
                       &atQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E5A8[1], &atQuad->shape.corners.cornerA, &atQuad->shape.corners.cornerC,
                       &atQuad->shape.corners.cornerD);
        Math3D_TriNorm(&D_8015E530[0], &acQuad->shape.corners.cornerA, &acQuad->shape.corners.cornerB,
                       &acQuad->shape.corners.cornerC);
        Math3D_TriNorm(&D_8015E530[1], &acQuad->shape.corners.cornerA, &acQuad->shape.corners.cornerC,
                       &acQuad->shape.corners.cornerD);

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                if ((Math3D_TrisIntersect(&D_8015E5A8[j], &D_8015E530[i], &sHitPos) == 1) &&
                    func_8005D218(globalCtx, atQuad, &sHitPos)) {
                    atQuadPos.x = (atQuad->shape.corners.cornerA.x + atQuad->shape.corners.cornerB.x +
                                   atQuad->shape.corners.cornerC.x + atQuad->shape.corners.cornerD.x) *
                                  0.25f;
                    atQuadPos.y = (atQuad->shape.corners.cornerA.y + atQuad->shape.corners.cornerB.y +
                                   atQuad->shape.corners.cornerC.y + atQuad->shape.corners.cornerD.y) *
                                  0.25f;
                    atQuadPos.z = (atQuad->shape.corners.cornerA.z + atQuad->shape.corners.cornerB.z +
                                   atQuad->shape.corners.cornerC.z + atQuad->shape.corners.cornerD.z) *
                                  0.25f;
                    acQuadPos.x = (acQuad->shape.corners.cornerA.x + acQuad->shape.corners.cornerB.x +
                                   acQuad->shape.corners.cornerC.x + acQuad->shape.corners.cornerD.x) *
                                  0.25f;
                    acQuadPos.y = (acQuad->shape.corners.cornerA.y + acQuad->shape.corners.cornerB.y +
                                   acQuad->shape.corners.cornerC.y + acQuad->shape.corners.cornerD.y) *
                                  0.25f;
                    acQuadPos.z = (acQuad->shape.corners.cornerA.z + acQuad->shape.corners.cornerB.z +
                                   acQuad->shape.corners.cornerC.z + acQuad->shape.corners.cornerD.z) *
                                  0.25f;
                    Collider_ATVsACHit(globalCtx, &atQuad->base, &atQuad->elem, &atQuadPos, &acQuad->base,
                                       &acQuad->elem, &acQuadPos, &sHitPos);
                    return;
                }
            }
        }
    }
}

void func_80060EBC_Type0_processAC_(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderSpheres* spheres = (ColliderSpheres*)collider;
    ColliderSpheresElement* spheresElem;
    Vec3f sp24;

    for (spheresElem = spheres->elements; spheresElem < (spheres->elements + spheres->nElements); spheresElem++) {
        if ((spheresElem->base.acElemFlags & ACELEM_DRAW_HITMARK) && (spheresElem->base.acHitElem != NULL) &&
            !(spheresElem->base.acHitElem->atElemFlags & ATELEM_DREW_HITMARK)) {
            Math_Vec3s_ToVec3f(&sp24, &spheresElem->base.acDmgInfo.hitPos);
            func_8005E604(globalCtx, spheresElem->base.acHit, spheresElem->base.acHitElem, &spheres->base,
                          &spheresElem->base, &sp24);
            spheresElem->base.acHitElem->atElemFlags |= ATELEM_DREW_HITMARK;
            return;
        }
    }
}

void func_80060F94_Type1_processAC_(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderCylinder* arg2 = (ColliderCylinder*)collider;
    Vec3f sp28;

    if ((arg2->elem.acElemFlags & ACELEM_DRAW_HITMARK) && (arg2->elem.acHitElem != NULL) &&
        !(arg2->elem.acHitElem->atElemFlags & ATELEM_DREW_HITMARK)) {
        Math_Vec3s_ToVec3f(&sp28, &arg2->elem.acDmgInfo.hitPos);
        func_8005E604(globalCtx, arg2->elem.acHit, arg2->elem.acHitElem, &arg2->base, &arg2->elem, &sp28);
        arg2->elem.acHitElem->atElemFlags |= ATELEM_DREW_HITMARK;
    }
}

void func_80061028_Type2_processAC_(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderTris* tris = (ColliderTris*)collider;
    ColliderTrisElement* trisElem;
    Vec3f sp24;

    for (trisElem = tris->elements; trisElem < &tris->elements[tris->nElements]; trisElem++) {
        if ((trisElem->base.acElemFlags & ACELEM_DRAW_HITMARK) && (trisElem->base.acHitElem != NULL) &&
            !(trisElem->base.acHitElem->atElemFlags & ATELEM_DREW_HITMARK)) {
            Math_Vec3s_ToVec3f(&sp24, &trisElem->base.acDmgInfo.hitPos);
            func_8005E604(globalCtx, trisElem->base.acHit, trisElem->base.acHitElem, &tris->base, &trisElem->base,
                          &sp24);
            trisElem->base.acHitElem->atElemFlags |= ATELEM_DREW_HITMARK;
            return;
        }
    }
}

void func_8006110C_Type3_processAC_(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderQuad* quad = (ColliderQuad*)collider;
    Vec3f sp28;

    if ((quad->elem.acElemFlags & ACELEM_DRAW_HITMARK) && (quad->elem.acHitElem != NULL) &&
        !(quad->elem.acHitElem->atElemFlags & ATELEM_DREW_HITMARK)) {
        Math_Vec3s_ToVec3f(&sp28, &quad->elem.acDmgInfo.hitPos);
        func_8005E604(globalCtx, quad->elem.acHit, quad->elem.acHitElem, &quad->base, &quad->elem, &sp28);
        quad->elem.acHitElem->atElemFlags |= ATELEM_DREW_HITMARK;
    }
}

typedef void (*callback_800611A0)(GlobalContext*, ColliderContext*, Collider*);

callback_800611A0 D_8011DF5C_processAC_[] = {
    func_80060EBC_Type0_processAC_, // COLTYPE_SPHERES
    func_80060F94_Type1_processAC_, // COLTYPE_CYLINDER
    func_80061028_Type2_processAC_, // COLTYPE_TRIANGLES
    func_8006110C_Type3_processAC_, // COLTYPE_QUAD
};

void func_800611A0_processAC_(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    Collider** acColliderP;
    Collider* acCollider;

    for (acColliderP = colliderCtx->acColliders; acColliderP < &colliderCtx->acColliders[colliderCtx->nACColliders];
         acColliderP++) {
        acCollider = *acColliderP;
        if ((acCollider != NULL) && (acCollider->acFlags & AC_ON) &&
            ((acCollider->actor == NULL) || (acCollider->actor->update != NULL))) {
            D_8011DF5C_processAC_[acCollider->colType](globalCtx, colliderCtx, acCollider);
        }
    }
}

typedef void (*ATvsACFunc)(GlobalContext*, ColliderContext*, Collider*, Collider*);

ATvsACFunc sATvsACFuncs[COLTYPE_MAX][COLTYPE_MAX] = {
    // COLTYPE_SPHERES
    {
        Collider_ATSpheresVsACSpheres,  // COLTYPE_SPHERES
        Collider_ATSpheresVsACCylinder, // COLTYPE_CYLINDER
        Collider_ATSpheresVsACTris,     // COLTYPE_TRIANGLES
        Collider_ATSpheresVsACQuad,     // COLTYPE_QUAD
    },
    // COLTYPE_CYLINDER
    {
        Collider_ATCylinderVsACSpheres,  // COLTYPE_SPHERES
        Collider_ATCylinderVsACCylinder, // COLTYPE_CYLINDER
        Collider_ATCylinderVsACTris,     // COLTYPE_TRIANGLES
        Collider_ATCylinderVsACQuad,     // COLTYPE_QUAD
    },
    // COLTYPE_TRIANGLES
    {
        Collider_ATTrisVsACSpheres,  // COLTYPE_SPHERES
        Collider_ATTrisVsACCylinder, // COLTYPE_CYLINDER
        Collider_ATTrisVsACTris,     // COLTYPE_TRIANGLES
        Collider_ATTrisVsACQuad,     // COLTYPE_QUAD
    },
    // COLTYPE_QUAD
    {
        Collider_ATQuadVsACSpheres,  // COLTYPE_SPHERES
        Collider_ATQuadVsACCylinder, // COLTYPE_CYLINDER
        Collider_ATQuadVsACTris,     // COLTYPE_TRIANGLES
        Collider_ATQuadVsACQuad,     // COLTYPE_QUAD
    },
};

void Collider_DoATColliderVsAC(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* atCollider) {
    Collider** acColliderP;
    Collider* acCollider;

    for (acColliderP = colliderCtx->acColliders; acColliderP < &colliderCtx->acColliders[colliderCtx->nACColliders];
         acColliderP++) {
        acCollider = *acColliderP;
        if (acCollider != NULL) {
            if ((acCollider->acFlags & AC_ON) && ((acCollider->actor == NULL) || (acCollider->actor->update != NULL))) {
                if (((acCollider->acFlags & atCollider->atFlags & AT_TYPE_ALL) != 0) && (atCollider != acCollider) &&
                    ((atCollider->atFlags & AT_SELF) || (atCollider->actor == NULL) ||
                     (acCollider->actor != atCollider->actor))) {
                    sATvsACFuncs[atCollider->colType][acCollider->colType](globalCtx, colliderCtx, atCollider,
                                                                           acCollider);
                }
            }
        }
    }
}

void Collider_DoATVsAC(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    Collider** atColliderP;
    Collider* atCollider;

    if ((colliderCtx->nATColliders != 0) && (colliderCtx->nACColliders != 0)) {
        for (atColliderP = colliderCtx->atColliders; atColliderP < &colliderCtx->atColliders[colliderCtx->nATColliders];
             atColliderP++) {
            atCollider = *atColliderP;
            if ((atCollider != NULL) && (atCollider->atFlags & AT_ON) &&
                ((atCollider->actor == NULL) || (atCollider->actor->update != NULL))) {
                Collider_DoATColliderVsAC(globalCtx, colliderCtx, atCollider);
            }
        }
        func_800611A0_processAC_(globalCtx, colliderCtx);
    }
}

typedef enum ColliderMassType {
    /* 0 */ MASSTYPE_IMMOVABLE,
    /* 1 */ MASSTYPE_HEAVY,
    /* 2 */ MASSTYPE_NORMAL
} ColliderMassType;

s32 Collider_GetMassType(u8 mass) {
    if (mass == MASS_IMMOVABLE) {
        return MASSTYPE_IMMOVABLE;
    }
    if (mass == MASS_HEAVY) {
        return MASSTYPE_HEAVY;
    }
    return MASSTYPE_NORMAL;
}

void Collider_OCVsOCHit(Collider* leftCollider, ColliderElement* leftElement, Vec3f* leftPos, Collider* rightCollider,
                        ColliderElement* rightElement, Vec3f* rightPos, f32 arg6) {
    s32 pad[3];
    f32 dxz;
    f32 leftMass;
    f32 rightMass;
    f32 totalMass;
    f32 invTotalMass;
    f32 leftMassRatio;
    f32 rightMassRatio;
    Actor* leftActor;
    Actor* rightActor;
    s32 rightMassType;
    s32 leftMassType;
    f32 dx;
    f32 dz;

    leftActor = leftCollider->actor;
    rightActor = rightCollider->actor;
    leftCollider->ocFlags1 |= OC1_HIT;
    leftCollider->oc = rightActor;
    leftElement->ocElemFlags |= OCELEM_HIT;
    if (rightCollider->ocFlags2 & OC2_TYPE_PLAYER) {
        leftCollider->ocFlags2 |= OC2_HIT_PLAYER;
    }
    rightCollider->oc = leftActor;
    rightCollider->ocFlags1 |= OC1_HIT;
    rightElement->ocElemFlags |= OCELEM_HIT;
    if (leftCollider->ocFlags2 & OC2_TYPE_PLAYER) {
        rightCollider->ocFlags2 |= OC2_HIT_PLAYER;
    }
    if ((leftActor != NULL) && (rightActor != NULL) && !(leftCollider->ocFlags1 & OC1_NO_PUSH) &&
        !(rightCollider->ocFlags1 & OC1_NO_PUSH)) {
        leftMassType = Collider_GetMassType(leftActor->collideData.mass);
        rightMassType = Collider_GetMassType(rightActor->collideData.mass);
        leftMass = leftActor->collideData.mass;
        rightMass = rightActor->collideData.mass;
        totalMass = leftMass + rightMass;
        if (fabsf(totalMass) < 0.008f) {
            leftMass = 1.0f;
            rightMass = 1.0f;
            totalMass = 2.0f;
        }
        dx = rightPos->x - leftPos->x;
        dz = rightPos->z - leftPos->z;
        dxz = sqrtf((dx * dx) + (dz * dz));
        if (leftMassType == MASSTYPE_IMMOVABLE) {
            if (rightMassType != MASSTYPE_IMMOVABLE) {
                rightMassRatio = 0;
                leftMassRatio = 1;
            } else {
                // Both MASSTYPE_IMMOVABLE
                return;
            }
        } else if (leftMassType == MASSTYPE_HEAVY) {
            if (rightMassType == MASSTYPE_IMMOVABLE) {
                rightMassRatio = 1;
                leftMassRatio = 0;
            } else if (rightMassType == MASSTYPE_HEAVY) {
                rightMassRatio = 0.5f;
                leftMassRatio = 0.5f;
            } else { // rightMassType == MASSTYPE_NORMAL
                rightMassRatio = 0;
                leftMassRatio = 1;
            }
        } else { // leftMassType == MASSTYPE_NORMAL
            if (rightMassType == MASSTYPE_NORMAL) {
                invTotalMass = 1 / totalMass;
                rightMassRatio = rightMass * invTotalMass;
                leftMassRatio = leftMass * invTotalMass;
            } else { // rightMassType == MASSTYPE_HEAVY or MASSTYPE_IMMOVABLE
                rightMassRatio = 1;
                leftMassRatio = 0;
            }
        }
        if (!(fabsf(dxz) < 0.008f)) {
            dx *= arg6 / dxz;
            dz *= arg6 / dxz;
            leftActor->collideData.displacement.x += -dx * rightMassRatio;
            leftActor->collideData.displacement.z += -dz * rightMassRatio;
            rightActor->collideData.displacement.x += dx * leftMassRatio;
            rightActor->collideData.displacement.z += dz * leftMassRatio;
        } else {
            if (arg6 != 0.0f) {
                leftActor->collideData.displacement.x += -arg6 * rightMassRatio;
                rightActor->collideData.displacement.x += arg6 * leftMassRatio;
            } else {
                leftActor->collideData.displacement.x -= rightMassRatio;
                rightActor->collideData.displacement.x += leftMassRatio;
            }
        }
    }
}

void Collider_OCSpheresVsOCSpheres(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* leftCollider,
                                   Collider* rightCollider) {
    ColliderSpheres* leftSpheres = (ColliderSpheres*)leftCollider;
    ColliderSpheres* rightSpheres = (ColliderSpheres*)rightCollider;
    ColliderSpheresElement* rightSpheresElem;
    ColliderSpheresElement* leftSpheresElem;
    f32 sp74;
    Vec3f leftSpheresElemPos;
    Vec3f rightSpheresElemPos;

    if ((leftSpheres->nElements > 0) && (leftSpheres->elements != NULL) && (rightSpheres->nElements > 0) &&
        (rightSpheres->elements != NULL)) {
        for (leftSpheresElem = leftSpheres->elements; leftSpheresElem < &leftSpheres->elements[leftSpheres->nElements];
             leftSpheresElem++) {
            if (!(leftSpheresElem->base.ocElemFlags & OCELEM_ON)) {
                continue;
            }
            for (rightSpheresElem = rightSpheres->elements;
                 rightSpheresElem < &rightSpheres->elements[rightSpheres->nElements]; rightSpheresElem++) {
                if (!(rightSpheresElem->base.ocElemFlags & OCELEM_ON)) {
                    continue;
                }
                if (Math3D_SpheresTouchingSurface(&leftSpheresElem->shape.world, &rightSpheresElem->shape.world,
                                                  &sp74) == 1) {
                    Math_Vec3s_ToVec3f(&leftSpheresElemPos, &leftSpheresElem->shape.world.center);
                    Math_Vec3s_ToVec3f(&rightSpheresElemPos, &rightSpheresElem->shape.world.center);
                    Collider_OCVsOCHit(&leftSpheres->base, &leftSpheresElem->base, &leftSpheresElemPos,
                                       &rightSpheres->base, &rightSpheresElem->base, &rightSpheresElemPos, sp74);
                }
            }
        }
    }
}

void Collider_OCSpheresVsOCCylinder(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* leftCollider,
                                    Collider* rightCollider) {
    ColliderSpheres* leftSpheres = (ColliderSpheres*)leftCollider;
    ColliderCylinder* rightCylinder = (ColliderCylinder*)rightCollider;
    ColliderSpheresElement* leftSpheresElem;
    f32 sp78;
    Vec3f leftSpheresElemPos;
    Vec3f rightCylinderPos;

    if ((leftSpheres->nElements > 0) && (leftSpheres->elements != NULL) && (rightCylinder->base.ocFlags1 & OC1_ON) &&
        (rightCylinder->elem.ocElemFlags & OCELEM_ON)) {
        for (leftSpheresElem = leftSpheres->elements; leftSpheresElem < &leftSpheres->elements[leftSpheres->nElements];
             leftSpheresElem++) {
            if (!(leftSpheresElem->base.ocElemFlags & OCELEM_ON)) {
                continue;
            }
            if (func_800CFD84(&leftSpheresElem->shape.world, &rightCylinder->shape, &sp78) == 1) {
                Math_Vec3s_ToVec3f(&leftSpheresElemPos, &leftSpheresElem->shape.world.center);
                Math_Vec3s_ToVec3f(&rightCylinderPos, &rightCylinder->shape.pos);
                Collider_OCVsOCHit(&leftSpheres->base, &leftSpheresElem->base, &leftSpheresElemPos,
                                   &rightCylinder->base, &rightCylinder->elem, &rightCylinderPos, sp78);
            }
        }
    }
}

void Collider_OCCylinderVsOCSpheres(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* leftCollider,
                                    Collider* rightCollider) {
    Collider_OCSpheresVsOCCylinder(globalCtx, colliderCtx, rightCollider, leftCollider);
}

void Collider_OCCylinderVsOCCylinder(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* leftCollider,
                                     Collider* rightCollider) {
    ColliderCylinder* leftCylinder = (ColliderCylinder*)leftCollider;
    ColliderCylinder* rightCylinder = (ColliderCylinder*)rightCollider;
    f32 sp4C;
    Vec3f leftCylinderPos;
    Vec3f rightCylinderPos;

    if ((leftCylinder->base.ocFlags1 & OC1_ON) && (rightCylinder->base.ocFlags1 & OC1_ON) &&
        (leftCylinder->elem.ocElemFlags & OCELEM_ON) && (rightCylinder->elem.ocElemFlags & OCELEM_ON) &&
        (Math3D_CylinderOutCylinder(&leftCylinder->shape, &rightCylinder->shape, &sp4C) == 1)) {
        Math_Vec3s_ToVec3f(&leftCylinderPos, &leftCylinder->shape.pos);
        Math_Vec3s_ToVec3f(&rightCylinderPos, &rightCylinder->shape.pos);
        Collider_OCVsOCHit(&leftCylinder->base, &leftCylinder->elem, &leftCylinderPos, &rightCylinder->base,
                           &rightCylinder->elem, &rightCylinderPos, sp4C);
    }
}

s32 Collider_IsColliderOCOff(Collider* collider) {
    if (!(collider->ocFlags1 & OC1_ON)) {
        return 1;
    }
    return 0;
}

s32 Collider_CheckSkipOCVsOC(Collider* leftCollider, Collider* rightCollider) {
    if (((leftCollider->ocFlags1 & rightCollider->ocFlags2 & OC1_TYPE_ALL) == 0) ||
        ((leftCollider->ocFlags2 & rightCollider->ocFlags1 & OC1_TYPE_ALL) == 0) ||
        ((leftCollider->ocFlags2 & OC2_UNK1) && (rightCollider->ocFlags2 & OC2_UNK2)) ||
        ((rightCollider->ocFlags2 & OC2_UNK1) && (leftCollider->ocFlags2 & OC2_UNK2))) {
        return 1;
    }
    if (leftCollider->actor == rightCollider->actor) {
        return 1;
    }
    return 0;
}

typedef void (*OCVsOCFunc)(GlobalContext*, ColliderContext*, Collider*, Collider*);

OCVsOCFunc sOCVsOCFuncs[COLTYPE_MAX][COLTYPE_MAX] = {
    { Collider_OCSpheresVsOCSpheres, Collider_OCSpheresVsOCCylinder, NULL, NULL },
    { Collider_OCCylinderVsOCSpheres, Collider_OCCylinderVsOCCylinder, NULL, NULL },
    { NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL },
};

void Collider_DoOCVsOC(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    Collider** rightColliderP;
    Collider** leftColliderP;
    OCVsOCFunc func;

    for (leftColliderP = colliderCtx->ocColliders; leftColliderP < &colliderCtx->ocColliders[colliderCtx->nOCColliders];
         leftColliderP++) {
        if ((*leftColliderP == NULL) || (Collider_IsColliderOCOff(*leftColliderP) == 1)) {
            continue;
        }
        for (rightColliderP = leftColliderP + 1; rightColliderP < &colliderCtx->ocColliders[colliderCtx->nOCColliders];
             rightColliderP++) {
            if ((*rightColliderP == NULL) || (Collider_IsColliderOCOff(*rightColliderP) == 1) ||
                (Collider_CheckSkipOCVsOC(*leftColliderP, *rightColliderP) == 1)) {
                continue;
            }
            func = sOCVsOCFuncs[(*leftColliderP)->colType][(*rightColliderP)->colType];
            if (func == NULL) {
                osSyncPrintf("CollisionCheck_OC():未対応 %d, %d\n", (*leftColliderP)->colType,
                             (*rightColliderP)->colType);
                continue;
            }
            func(globalCtx, colliderCtx, *leftColliderP, *rightColliderP);
        }
    }
}

static CollideData sCollideDataInit = {
    NULL,
    {
        0.0f,
        0.0f,
        0.0f,
    },
    0xA,
    0xA,
    0,
    50,
    8,
    0,
    0,
    HIT_BACKLASH_NONE,
    HIT_SPECIAL_EFFECT_NONE,
};

void Collider_InitCollideData(CollideData* collideData) {
    *collideData = sCollideDataInit;
}

void func_80061E8C(CollideData* collideData) {
    collideData->damage = 0;
    collideData->damageEffect = 0;
    collideData->atHitBacklash = HIT_BACKLASH_NONE;
    collideData->acHitSpecialEffect = HIT_SPECIAL_EFFECT_NONE;
    collideData->displacement.x = collideData->displacement.y = collideData->displacement.z = 0.0f;
}

typedef struct struct_80061EB0_arg0 {
    /* 0x00 */ char unk_0[0x10];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ char unk_14[2];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
} struct_80061EB0_arg0;

typedef struct struct_80061EB0_arg1 {
    /* 0x00 */ u8 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
} struct_80061EB0_arg1;

// unused
void func_80061EB0(struct_80061EB0_arg0* arg0, struct_80061EB0_arg1* arg1) {
    arg0->unk17 = arg1->unk0;
    arg0->unk10 = arg1->unk2;
    arg0->unk12 = arg1->unk4;
    arg0->unk16 = arg1->unk6;
}

void func_80061ED4(CollideData* collideData, ActorDamageChart* damageChart, CollideDataInit* collideDataInit) {
    collideData->health = collideDataInit->health;
    collideData->damageChart = damageChart;
    collideData->unk_10 = collideDataInit->unk_02;
    collideData->unk_12 = collideDataInit->unk_04;
    collideData->mass = collideDataInit->mass;
}

void func_80061EFC(CollideData* collideData, ActorDamageChart* damageChart, CollideDataInitAlt* collideDataInitAlt) {
    collideData->health = collideDataInitAlt->health;
    collideData->damageChart = damageChart;
    collideData->unk_10 = collideDataInitAlt->unk_10;
    collideData->unk_12 = collideDataInitAlt->unk_12;
    collideData->unk_14 = collideDataInitAlt->unk_14;
    collideData->mass = collideDataInitAlt->mass;
}

// unused
void func_80061F2C(CollideData* collideData, s32 arg1, CollideDataInitAlt* collideDataInitAlt) {
    func_80061EFC(collideData, CollisionBtlTbl_Get(arg1), collideDataInitAlt);
}

void func_80061F64(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider, ColliderElement* elem) {
    f32 unkf;
    s32 i;
    u32 unk0;

    if (collider->actor != NULL && (collider->acFlags & AC_HIT) && (elem->acElemFlags & ACELEM_HIT) &&
        !(elem->acElemFlags & ACELEM_NO_DAMAGE)) {

        if (elem->acHitElem == NULL) {
            __assert("pclobj_elem->ac_hit_elem != NULL", "../z_collision_check.c", 6493);
        }

        if (collider->actor->collideData.damageChart == NULL) {
            unkf = (f32)elem->acHitElem->atDmgInfo.damage - (f32)elem->acDmgInfo.unk_05;
            if (unkf < 0.0f) {
                unkf = 0.0f;
            }
        } else {
            unk0 = elem->acHitElem->atDmgInfo.dmgFlags;

            for (i = 0; i < 32; i++) {
                if (unk0 == 1) {
                    break;
                }

                unk0 >>= 1;
            }

            unkf = collider->actor->collideData.damageChart->attack[i].raw & 0xF;
            collider->actor->collideData.damageEffect =
                (collider->actor->collideData.damageChart->attack[i].raw >> 4) & 0xF;
        }

        if (!(collider->acFlags & AC_HARD)) {
            collider->actor->collideData.damage += unkf;
        }
        if (1) {}
        if (1) {}
    }
}

void func_8006216C_Type0(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderSpheres* new_var2 = (ColliderSpheres*)collider;
    ColliderSpheres* new_var = (ColliderSpheres*)collider;
    s32 i;

    if (new_var->nElements <= 0 || new_var->elements == NULL) {
        return;
    }

    for (i = 0; i < new_var2->nElements; i++) {
        func_80061F64(globalCtx, colliderCtx, &new_var->base, &new_var->elements[i].base);
    }
}

void func_80062210_Type1(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderCylinder* new_var = (ColliderCylinder*)collider;

    func_80061F64(globalCtx, colliderCtx, collider, &new_var->elem);
}

void func_80062230_Type2(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    s32 i;
    ColliderTris* new_var = (ColliderTris*)collider;

    for (i = 0; i < new_var->nElements; i++) {
        func_80061F64(globalCtx, colliderCtx, &new_var->base, &new_var->elements[i].base);
    }
}

void func_800622C4_Type3(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider) {
    ColliderQuad* new_var = (ColliderQuad*)collider;

    func_80061F64(globalCtx, colliderCtx, collider, &new_var->elem);
}

typedef void (*func_ptr_800622E4)(GlobalContext*, ColliderContext*, Collider*);

func_ptr_800622E4 D_8011E008[] = {
    func_8006216C_Type0, // COLTYPE_SPHERES
    func_80062210_Type1, // COLTYPE_CYLINDER
    func_80062230_Type2, // COLTYPE_TRIANGLES
    func_800622C4_Type3, // COLTYPE_QUAD
};

void func_800622E4(GlobalContext* globalCtx, ColliderContext* colliderCtx) {
    Collider* acCollider;
    s32 i;

    for (i = 0; i < colliderCtx->nACColliders; i++) {
        acCollider = colliderCtx->acColliders[i];

        if (acCollider == NULL) {
            continue;
        }
        if (acCollider->acFlags & AC_NO_DAMAGE) {
            continue;
        }
        D_8011E008[acCollider->colType](globalCtx, colliderCtx, acCollider);
    }
}

s32 func_800623A4_Type0(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider, Vec3f* arg3,
                        Vec3f* arg4) {
    static Linef D_8015E610;

    ColliderSpheres* spheres = (ColliderSpheres*)collider;
    s32 i;
    ColliderSpheresElement* spheresElem;

    for (i = 0; i < spheres->nElements; i++) {
        spheresElem = &spheres->elements[i];

        if (!(spheresElem->base.ocElemFlags & OCELEM_ON)) {
            continue;
        }

        D_8015E610.a = *arg3;
        D_8015E610.b = *arg4;

        if (func_800CE600(&spheresElem->shape.world, &D_8015E610) == 1) {
            return true;
        }
    }

    return false;
}

s32 func_800624BC_Type1(GlobalContext* globalCtx, ColliderContext* colliderCtx, Collider* collider, Vec3f* arg3,
                        Vec3f* arg4) {
    static Vec3f D_8015E628;
    static Vec3f D_8015E638;

    ColliderCylinder* cylinder = (ColliderCylinder*)collider;

    if (!(cylinder->elem.ocElemFlags & OCELEM_ON)) {
        return false;
    }

    if (func_800CEE0C(&cylinder->shape, arg3, arg4, &D_8015E628, &D_8015E638) != 0) {
        return true;
    }

    return false;
}

typedef s32 (*func_ptr_80062530)(GlobalContext*, ColliderContext*, Collider*, Vec3f*, Vec3f*);

func_ptr_80062530 D_8011E018[] = {
    func_800623A4_Type0,
    func_800624BC_Type1,
    NULL,
    NULL,
};

s32 func_80062530(GlobalContext* globalCtx, ColliderContext* colliderCtx, Vec3f* arg2, Vec3f* arg3, Actor** skipActors,
                  s32 nSkipActors) {
    func_ptr_80062530 func;
    s32 skip;
    s32 result = false;
    s32 i;
    Collider** ocColliderP;
    Collider* ocCollider;

    for (ocColliderP = colliderCtx->ocColliders; ocColliderP < colliderCtx->ocColliders + colliderCtx->nOCColliders;
         ocColliderP++) {
        if (Collider_IsColliderOCOff(*ocColliderP) == 1) {
            continue;
        }

        skip = false;
        for (i = 0; i < nSkipActors; i++) {
            if ((*ocColliderP)->actor == skipActors[i]) {
                skip = true;
                break;
            }
        }

        if (skip == true) {
            continue;
        }

        ocCollider = *ocColliderP;
        func = D_8011E018[ocCollider->colType];

        if (func == NULL) {
            osSyncPrintf("CollisionCheck_generalLineOcCheck():未対応 %dタイプ\n", ocCollider->colType);
            continue;
        }

        result = func(globalCtx, colliderCtx, ocCollider, arg2, arg3);

        if (result) {
            break;
        }
    }

    return result;
}

// unused
void func_8006268C(GlobalContext* globalCtx, ColliderContext* colliderCtx, Vec3f* arg2, Vec3f* arg3) {
    func_80062530(globalCtx, colliderCtx, arg2, arg3, NULL, 0);
}

void func_800626B0(GlobalContext* globalCtx, ColliderContext* colliderCtx, Vec3f* arg2, Vec3f* arg3, Actor** skipActors,
                   s32 nSkipActors) {
    func_80062530(globalCtx, colliderCtx, arg2, arg3, skipActors, nSkipActors);
}

void Collider_UpdateCylinderShape(Actor* actor, ColliderCylinder* cylinder) {
    cylinder->shape.pos.x = actor->posRot.pos.x;
    cylinder->shape.pos.y = actor->posRot.pos.y;
    cylinder->shape.pos.z = actor->posRot.pos.z;
}

typedef struct struct_80062718_1 {
    /* 0x00 */ char unk0[0x46];
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
} struct_80062718_1;

typedef struct struct_80062718_2 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} struct_80062718_2;

// uses not decompiled
void func_80062718(struct_80062718_1* arg0, struct_80062718_2* arg1) {
    arg0->unk46 = arg1->unk0;
    arg0->unk48 = arg1->unk2;
    arg0->unk4A = arg1->unk4;
}

typedef struct struct_80062734 {
    char unk_0[0x40];
    ColliderQuadShape unk40;
} struct_80062734;

// uses not decompiled
void func_80062734(struct_80062734* arg0, Vec3f* cornerD, Vec3f* cornerC, Vec3f* cornerA, Vec3f* cornerB) {
    Math_Vec3f_Copy(&arg0->unk40.corners.cornerA, cornerA);
    Math_Vec3f_Copy(&arg0->unk40.corners.cornerB, cornerB);
    Math_Vec3f_Copy(&arg0->unk40.corners.cornerD, cornerD);
    Math_Vec3f_Copy(&arg0->unk40.corners.cornerC, cornerC);
    Collider_QuadShapeUpdateMidPoints(&arg0->unk40);
}

typedef struct struct_800627A0_ptr {
    /* 0x00 */ char unk_0[0x28];
    /* 0x28 */ TriNorm unk28;
} struct_800627A0_ptr;

typedef struct struct_800627A0 {
    char unk_0[0x1C];
    struct_800627A0_ptr* unk1C;
} struct_800627A0;

// uses not decompiled
void func_800627A0(struct_800627A0* arg0, s32 arg1, Vec3f* arg2, Vec3f* arg3, Vec3f* arg4) {
    struct_800627A0_ptr* temp_s0;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;

    temp_s0 = &arg0->unk1C[arg1];
    Math_Vec3f_Copy(&temp_s0->unk28.vtx[0], arg2);
    Math_Vec3f_Copy(&temp_s0->unk28.vtx[1], arg3);
    Math_Vec3f_Copy(&temp_s0->unk28.vtx[2], arg4);
    func_800CC8B4(arg2, arg3, arg4, &sp40, &sp3C, &sp38, &sp34);
    temp_s0->unk28.plane.normal.x = sp40;
    temp_s0->unk28.plane.normal.y = sp3C;
    temp_s0->unk28.plane.normal.z = sp38;
    temp_s0->unk28.plane.originDist = sp34;
}

// unused
void func_8006285C(GlobalContext* globalCtx, struct_800627A0* arg1, s32 arg2, Vec3f* arg3) {
    struct_800627A0_ptr* new_var;

    new_var = &arg1->unk1C[arg2];
    Collider_LoadTrisElementShape(globalCtx, &new_var->unk28, arg3);
}

void func_800628A4_Type0(s32 arg0, ColliderSpheres* spheres) {
    static Vec3f sModelCenter;
    // bss block number rollover!
    static Vec3f sWorldCenter;

    s32 i;

    for (i = 0; i < spheres->nElements; i++) {
        if (spheres->elements[i].shape.unk14 == arg0) {
            sModelCenter.x = spheres->elements[i].shape.model.center.x;
            sModelCenter.y = spheres->elements[i].shape.model.center.y;
            sModelCenter.z = spheres->elements[i].shape.model.center.z;
            Matrix_MultVec3f(&sModelCenter, &sWorldCenter);
            spheres->elements[i].shape.world.center.x = sWorldCenter.x;
            spheres->elements[i].shape.world.center.y = sWorldCenter.y;
            spheres->elements[i].shape.world.center.z = sWorldCenter.z;
            spheres->elements[i].shape.world.radius =
                (spheres->elements[i].shape.model.radius * spheres->elements[i].shape.unkRadiusScale);
        }
    }
}

void func_80062A28(GlobalContext* globalCtx, Vec3f* arg1) {
    static struct_8015D8A0 D_8015CF10;
    s32 sp24;

    D_8015CF10.unk0.x = (s16)(s32)arg1->x;
    D_8015CF10.unk0.y = (s16)(s32)arg1->y;
    D_8015CF10.unk0.z = (s16)(s32)arg1->z;
    D_8015CF10.unk494 = 5;
    D_8015CF10.unk498 = 5;
    D_8015CF10.unk49C[0].r = 128;
    D_8015CF10.unk49C[0].g = 0;
    D_8015CF10.unk49C[0].b = 0x40;
    D_8015CF10.unk49C[0].a = 0xFF;
    D_8015CF10.unk49C[1].r = 128;
    D_8015CF10.unk49C[1].g = 0;
    D_8015CF10.unk49C[1].b = 0x40;
    D_8015CF10.unk49C[1].a = 0xFF;
    D_8015CF10.unk49C[2].r = 0xFF;
    D_8015CF10.unk49C[2].g = 128;
    D_8015CF10.unk49C[2].b = 0;
    D_8015CF10.unk49C[2].a = 0xFF;
    D_8015CF10.unk49C[3].r = 0xFF;
    D_8015CF10.unk49C[3].g = 128;
    D_8015CF10.unk49C[3].b = 0;
    D_8015CF10.unk49C[3].a = 0xFF;
    D_8015CF10.unk4AC[0].r = 0x40;
    D_8015CF10.unk4AC[0].g = 0;
    D_8015CF10.unk4AC[0].b = 0x20;
    D_8015CF10.unk4AC[0].a = 0;
    D_8015CF10.unk4AC[1].r = 0x40;
    D_8015CF10.unk4AC[1].g = 0;
    D_8015CF10.unk4AC[1].b = 0x20;
    D_8015CF10.unk4AC[1].a = 0;
    D_8015CF10.unk4AC[2].r = 128;
    D_8015CF10.unk4AC[2].g = 0;
    D_8015CF10.unk4AC[2].b = 0x40;
    D_8015CF10.unk4AC[2].a = 0;
    D_8015CF10.unk4AC[3].r = 128;
    D_8015CF10.unk4AC[3].g = 0;
    D_8015CF10.unk4AC[3].b = 0x40;
    D_8015CF10.unk4AC[3].a = 0;
    D_8015CF10.unk4BC = 0;
    D_8015CF10.unk4C0 = 0x10;
    D_8015CF10.unk48C = 8.0f;
    D_8015CF10.unk490 = -1.0f;
    Effect_Add(globalCtx, &sp24, 0, 0U, 1U, &D_8015CF10);
}

void func_80062B80(GlobalContext* globalCtx, Vec3f* arg1) {
    static struct_8015D8A0 D_8015D3D8;
    s32 sp24;

    D_8015D3D8.unk0.x = (s16)(s32)arg1->x;
    D_8015D3D8.unk0.y = (s16)(s32)arg1->y;
    D_8015D3D8.unk0.z = (s16)(s32)arg1->z;
    D_8015D3D8.unk494 = 5;
    D_8015D3D8.unk498 = 5;
    D_8015D3D8.unk49C[0].r = 0xFF;
    D_8015D3D8.unk49C[0].g = 0xFF;
    D_8015D3D8.unk49C[0].b = 0xFF;
    D_8015D3D8.unk49C[0].a = 0xFF;
    D_8015D3D8.unk49C[1].r = 0x64;
    D_8015D3D8.unk49C[1].g = 0x64;
    D_8015D3D8.unk49C[1].b = 0x64;
    D_8015D3D8.unk49C[1].a = 0x64;
    D_8015D3D8.unk49C[2].r = 0x64;
    D_8015D3D8.unk49C[2].g = 0x64;
    D_8015D3D8.unk49C[2].b = 0x64;
    D_8015D3D8.unk49C[2].a = 0x64;
    D_8015D3D8.unk49C[3].r = 0x64;
    D_8015D3D8.unk49C[3].g = 0x64;
    D_8015D3D8.unk49C[3].b = 0x64;
    D_8015D3D8.unk49C[3].a = 0x64;
    D_8015D3D8.unk4AC[0].r = 0x32;
    D_8015D3D8.unk4AC[0].g = 0x32;
    D_8015D3D8.unk4AC[0].b = 0x32;
    D_8015D3D8.unk4AC[0].a = 0x32;
    D_8015D3D8.unk4AC[1].r = 0x32;
    D_8015D3D8.unk4AC[1].g = 0x32;
    D_8015D3D8.unk4AC[1].b = 0x32;
    D_8015D3D8.unk4AC[1].a = 0x32;
    D_8015D3D8.unk4AC[2].r = 0x32;
    D_8015D3D8.unk4AC[2].g = 0x32;
    D_8015D3D8.unk4AC[2].b = 0x32;
    D_8015D3D8.unk4AC[2].a = 0x32;
    D_8015D3D8.unk4AC[3].r = 0;
    D_8015D3D8.unk4AC[3].g = 0;
    D_8015D3D8.unk4AC[3].b = 0;
    D_8015D3D8.unk4AC[3].a = 0;
    D_8015D3D8.unk4BC = 0;
    D_8015D3D8.unk4C0 = 0x10;
    D_8015D3D8.unk48C = 8.0f;
    D_8015D3D8.unk490 = -1.0f;
    Effect_Add(globalCtx, &sp24, 0, 0U, 1U, &D_8015D3D8);
}

void func_80062CD4(GlobalContext* globalCtx, Vec3f* arg1) {
    static struct_8011E068 D_8011E028 = {
        { 0x10, 0 },
        { 0, 0, 0 },
        {
            0,    0xC8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xFF, 0xFF,
            0xFF, 0,    0xFF, 0xFF, 0x40, 0,    0xC8, 0xFF, 0,    0,    0xFF, 0x40, 6,
            0x66, 0x66, 0x42, 0xC,  0,    0,    0x41, 0xF0, 0,    0,    8,    0,
        },
        { 0, 0, 0 },
        { 0, 0x80, 0xFF, 0, 1, 0x2C, 0, 0, 0, 0, 0, 1 },
    };
    s32 sp24;

    D_8011E028.unk2.x = (s16)(s32)arg1->x;
    D_8011E028.unk2.y = (s16)(s32)arg1->y;
    D_8011E028.unk2.z = (s16)(s32)arg1->z;
    D_8011E028.unk2E.x = D_8011E028.unk2.x;
    D_8011E028.unk2E.y = D_8011E028.unk2.y;
    D_8011E028.unk2E.z = D_8011E028.unk2.z;
    Effect_Add(globalCtx, &sp24, 3, 0U, 1U, &D_8011E028);
}

void func_80062D60(GlobalContext* globalCtx, Vec3f* arg1) {
    func_80062CD4(globalCtx, arg1);
    Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_REFLECT_SW, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
}

void func_80062DAC(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2) {
    func_80062CD4(globalCtx, arg1);
    Audio_PlaySoundGeneral(NA_SE_IT_SHIELD_REFLECT_SW, arg2, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
}

void func_80062DF4(GlobalContext* globalCtx, Vec3f* arg1) {
    func_80062D60(globalCtx, arg1);
}

void func_80062E14(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2) {
    static struct_8011E068 D_8011E068 = {
        { 0x10, 0 },
        { 0, 0, 0 },
        {
            0,    0xC8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xFF, 0xFF,
            0xFF, 0,    0xFF, 0xFF, 0x40, 0,    0xC8, 0xFF, 0,    0,    0xFF, 0x40, 6,
            0x66, 0x66, 0x42, 0xC,  0,    0,    0x41, 0xF0, 0,    0,    8,    0,
        },
        { 0, 0, 0 },
        { 0, 0x80, 0xFF, 0, 1, 0x2C, 0, 0, 0, 0, 0, 0 },
    };
    s32 sp24;

    D_8011E068.unk2.x = arg1->x;
    D_8011E068.unk2.y = arg1->y;
    D_8011E068.unk2.z = arg1->z;
    D_8011E068.unk2E.x = D_8011E068.unk2.x;
    D_8011E068.unk2E.y = D_8011E068.unk2.y;
    D_8011E068.unk2E.z = D_8011E068.unk2.z;
    Effect_Add(globalCtx, &sp24, 3, 0U, 1U, &D_8011E068);
    Audio_PlaySoundGeneral(NA_SE_IT_REFLECTION_WOOD, arg2, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
}

s32 func_80062ECC(f32 arg0, f32 arg1, f32 arg2, Vec3f* arg3, Vec3f* arg4, Vec3f* arg5, Vec3f* arg6_out,
                  Vec3f* arg7_out) {
    Vec3f sp6C;
    Vec3f sp60;
    Vec3f sp54;
    f32 var_ft4_real;
    f32 sp4C;
    float new_var2;
    f32 temp_fa1_2;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 temp_fa0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0_2;
    s32 var_v1_2;
    s32 pad;

    sp6C.x = arg4->x - arg3->x;
    sp6C.y = (arg4->y - arg3->y) - arg2;
    sp6C.z = arg4->z - arg3->z;
    sp60.x = arg5->x - arg3->x;
    sp60.y = (arg5->y - arg3->y) - arg2;
    sp60.z = arg5->z - arg3->z;
    sp54.x = sp60.x - sp6C.x;
    sp54.y = sp60.y - sp6C.y;
    sp54.z = sp60.z - sp6C.z;
    if (((sp6C.y > 0.0f) && (sp6C.y < arg1)) && (sqrtf((sp6C.x * sp6C.x) + (sp6C.z * sp6C.z)) < arg0)) {
        return 3;
    }
    if (((sp60.y > 0.0f) && (sp60.y < arg1)) && (sqrtf((sp60.x * sp60.x) + (sp60.z * sp60.z)) < arg0)) {
        return 3;
    }
    new_var2 = 0.0f; // fake?
    temp_fa0 = ((sp6C.x * sp6C.x) + (sp6C.z * sp6C.z)) - (arg0 * arg0);
    temp_fv1 = (sp54.x * sp54.x) + (sp54.z * sp54.z);
    if (!(fabsf(temp_fv1) < 0.008f)) {
        temp_fa1_2 = ((2.0f * sp54.x) * sp6C.x) + ((2.0f * sp54.z) * sp6C.z);
        if ((temp_fa1_2 * temp_fa1_2) < ((4.0f * temp_fv1) * temp_fa0)) {
            return 0;
        }
        if (((temp_fa1_2 * temp_fa1_2) - ((4.0f * temp_fv1) * temp_fa0)) > new_var2) {
            var_v1_2 = var_v0_2 = 1;
        } else {
            var_v1_2 = 1;
            var_v0_2 = 0;
        }
        temp_fv0_2 = sqrtf((temp_fa1_2 * temp_fa1_2) - ((4.0f * temp_fv1) * temp_fa0));
        if (var_v1_2 == 1) {
            var_ft4_real = (temp_fv0_2 - temp_fa1_2) / (2.0f * temp_fv1);
        }
        if (var_v0_2 == 1) {
            sp4C = ((-temp_fa1_2) - temp_fv0_2) / (2.0f * temp_fv1);
        }
    } else if (!(fabsf(((2.0f * sp54.x) * sp6C.x) + ((2.0f * sp54.z) * sp6C.z)) < 0.008f)) {
        var_v1_2 = 1;
        var_v0_2 = 0;
        var_ft4_real = (-temp_fa0) / (((2.0f * sp54.x) * sp6C.x) + ((2.0f * sp54.z) * sp6C.z));
    } else {
        if (temp_fa0 <= 0.0f) {
            var_a0 = (sp6C.y > 0.0f) && (sp6C.y < arg1);
            var_a0_2 = (sp60.y > 0.0f) && (sp60.y < arg1);
            if ((var_a0 != 0) && (var_a0_2 != 0)) {
                *arg6_out = sp6C;
                *arg7_out = sp60;
                return 2;
            }
            if (var_a0 != 0) {
                *arg6_out = sp6C;
                return 1;
            }
            if (var_a0_2 != 0) {
                *arg6_out = sp60;
                return 1;
            }
        }
        return 0;
    }
    {
        if (var_v0_2 == 0) {
            if ((var_ft4_real < 0.0f) || (var_ft4_real > 1.0f)) {
                return 0;
            }
        } else {
            var_a0 = (var_ft4_real < 0.0f) || (var_ft4_real > 1.0f);
            var_a0_2 = (sp4C < 0.0f) || (sp4C > 1.0f);
            if ((var_a0 != 0) && (var_a0_2 != 0)) {
                return 0;
            }
            if (var_a0 != 0) {
                var_v1_2 = 0;
            }
            if (0 != var_a0_2) {
                var_v0_2 = 0;
            }
        }
        if (var_v1_2 == 1) {
            if ((((var_ft4_real * sp54.y) + sp6C.y) < 0.0f) || (arg1 < ((var_ft4_real * sp54.y) + sp6C.y))) {
                var_v1_2 = 0;
            }
        }
        if (var_v0_2 == 1) {
            if ((((sp4C * sp54.y) + sp6C.y) < 0.0f) || (arg1 < ((sp4C * sp54.y) + sp6C.y))) {
                var_v0_2 = 0;
            }
        }
        if ((var_v1_2 == 0) && (var_v0_2 == 0)) {
            return 0;
        }
        if ((var_v1_2 == 1) && (var_v0_2 == 1)) {
            arg6_out->x = ((var_ft4_real * sp54.x) + sp6C.x) + arg3->x;
            arg6_out->y = ((var_ft4_real * sp54.y) + sp6C.y) + arg3->y;
            arg6_out->z = ((var_ft4_real * sp54.z) + sp6C.z) + arg3->z;
            arg7_out->x = ((sp4C * sp54.x) + sp6C.x) + arg3->x;
            arg7_out->y = ((sp4C * sp54.y) + sp6C.y) + arg3->y;
            arg7_out->z = ((sp4C * sp54.z) + sp6C.z) + arg3->z;
            return 2;
        }
        if (var_v1_2 == 1) {
            arg6_out->x = ((var_ft4_real * sp54.x) + sp6C.x) + arg3->x;
            arg6_out->y = ((var_ft4_real * sp54.y) + sp6C.y) + arg3->y;
            arg6_out->z = ((var_ft4_real * sp54.z) + sp6C.z) + arg3->z;
            return 1;
        }
        if (var_v0_2 == 1) {
            arg6_out->x = ((sp4C * sp54.x) + sp6C.x) + arg3->x;
            arg6_out->y = ((sp4C * sp54.y) + sp6C.y) + arg3->y;
            arg6_out->z = ((sp4C * sp54.z) + sp6C.z) + arg3->z;
            return 1;
        }
        return 1;
    }
}

s16 func_800635D0(s32 arg0) {
    s16 var_v1;

    var_v1 = 0;
    if (arg0 & 0x400100) {
        var_v1 = 1;
    } else if (arg0 & 0x03000242) {
        var_v1 = 2;
    } else if (arg0 & 0x48800400) {
        var_v1 = 4;
    } else if (arg0 & 0x04000000) {
        var_v1 = 8;
    }
    gGameInfo->data[0x4E7] = var_v1;
    return var_v1;
}
