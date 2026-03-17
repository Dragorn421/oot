#ifndef Z64COLLISION_CHECK_H
#define Z64COLLISION_CHECK_H

#include "ultra64.h"
#include "z64math.h"

struct Actor;

typedef enum ColliderType {
    /* 0 */ COLTYPE_SPHERES,
    /* 1 */ COLTYPE_CYLINDER,
    /* 2 */ COLTYPE_TRIANGLES,
    /* 3 */ COLTYPE_QUAD,
    /* 4 */ COLTYPE_MAX
} ColliderType;

typedef enum ColliderMaterial {
    /*  0 */ COL_MATERIAL_HIT0, // Blue blood, white hitmark
    /*  1 */ COL_MATERIAL_HIT1, // No blood, dust hitmark
    /*  2 */ COL_MATERIAL_HIT2, // Green blood, dust hitmark
    /*  3 */ COL_MATERIAL_HIT3, // No blood, white hitmark
    /*  4 */ COL_MATERIAL_HIT4, // Water burst, no hitmark
    /*  5 */ COL_MATERIAL_HIT5, // No blood, red hitmark
    /*  6 */ COL_MATERIAL_HIT6, // Green blood, white hitmark
    /*  7 */ COL_MATERIAL_HIT7, // Red blood, white hitmark
    /*  8 */ COL_MATERIAL_HIT8, // Blue blood, red hitmark
    /*  9 */ COL_MATERIAL_METAL,
    /* 10 */ COL_MATERIAL_NONE,
    /* 11 */ COL_MATERIAL_WOOD,
    /* 12 */ COL_MATERIAL_HARD,
    /* 13 */ COL_MATERIAL_TREE
} ColliderMaterial;

typedef struct Collider {
    /* 0x00 */ struct Actor* actor;
    /* 0x04 */ struct Actor* at;
    /* 0x08 */ struct Actor* ac;
    /* 0x0C */ struct Actor* oc;
    /* 0x10 */ u8 atFlags;
    /* 0x11 */ u8 acFlags;
    /* 0x12 */ u8 ocFlags1;
    /* 0x13 */ u8 ocFlags2;
    /* 0x14 */ u8 colMaterial;
    /* 0x15 */ u8 colType; // See `ColliderType` enum
} Collider; // size = 0x18

typedef struct ColliderSrc {
    /* 0x00 */ u8 colMaterial;
    /* 0x01 */ u8 atFlags;
    /* 0x02 */ u8 acFlags;
    /* 0x03 */ u8 ocFlags1;
    /* 0x04 */ u8 ocFlags2;
    /* 0x05 */ u8 colType; // See `ColliderType` enum
} ColliderSrc; // size = 0x06

typedef struct ColliderSrcAlt {
    /* 0x0 */ u8 colMaterial;
    /* 0x1 */ u8 atFlags;
    /* 0x2 */ u8 acFlags;
    /* 0x3 */ u8 ocFlags1;
    /* 0x4 */ u8 colType;
} ColliderSrcAlt; // size = 0x5

typedef struct ColliderSrcAltWithActor {
    /* 0x0 */ struct Actor* actor;
    /* 0x4 */ u8 atFlags;
    /* 0x5 */ u8 acFlags;
    /* 0x6 */ u8 ocFlags1;
    /* 0x7 */ u8 colType;
} ColliderSrcAltWithActor; // size = 0x8

typedef enum HitSpecialEffect {
    HIT_SPECIAL_EFFECT_NONE,
    HIT_SPECIAL_EFFECT_FIRE,
    HIT_SPECIAL_EFFECT_ICE,
    HIT_SPECIAL_EFFECT_ELECTRIC,
    HIT_SPECIAL_EFFECT_KNOCKBACK,
    HIT_SPECIAL_EFFECT_7 = 7, // Same effect as `HIT_SPECIAL_EFFECT_NONE`
    HIT_SPECIAL_EFFECT_8, // Same effect as `HIT_SPECIAL_EFFECT_NONE`
    HIT_SPECIAL_EFFECT_9 // Same effect as `HIT_SPECIAL_EFFECT_NONE`
} HitSpecialEffect;

typedef struct ColliderElementDamageInfoAT {
    /* 0x00 */ u32 dmgFlags; // Damage types dealt by this collider element as AT.
    /* 0x04 */ u8 hitSpecialEffect; // The hit special effect applied to any actor attacked by this AT collider.
    /* 0x05 */ u8 damage; // Damage
} ColliderElementDamageInfoAT; // size = 0x08

typedef enum HitBacklash {
    HIT_BACKLASH_NONE,
    HIT_BACKLASH_ELECTRIC
} HitBacklash;

typedef struct ColliderElementDamageInfoAC {
    /* 0x00 */ u32 dmgFlags; /* Collision Exclusion Mask */
    /* 0x04 */ u8 hitBacklash; /* Damage Effect (Knockback, Fire, etc.) */
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ Vec3s hitPos;
} ColliderElementDamageInfoAC; // size = 0x0C

typedef struct ColliderElementDamageInfoACSrc {
    /* 0x0 */ u32 dmgFlags; /* Bumper Exclusion Mask */
    /* 0x4 */ u8 hitBacklash;
    /* 0x5 */ u8 unk5;
} ColliderElementDamageInfoACSrc; // size = 0x8

typedef enum ElementMaterial {
    /* 0 */ ELEM_MATERIAL_UNK0,
    /* 1 */ ELEM_MATERIAL_UNK1,
    /* 2 */ ELEM_MATERIAL_UNK2,
    /* 3 */ ELEM_MATERIAL_UNK3,
    /* 4 */ ELEM_MATERIAL_UNK4,
    /* 5 */ ELEM_MATERIAL_UNK5,
    /* 6 */ ELEM_MATERIAL_UNK6,
    /* 7 */ ELEM_MATERIAL_UNK7
} ElementMaterial;

typedef struct ColliderElement {
    /* 0x00 */ ColliderElementDamageInfoAT atDmgInfo;
    /* 0x08 */ ColliderElementDamageInfoAC acDmgInfo;
    /* 0x14 */ u8 elemMaterial;
    /* 0x15 */ u8 atElemFlags;
    /* 0x16 */ u8 acElemFlags;
    /* 0x17 */ u8 ocElemFlags;
    /* 0x18 */ Collider* atHit;
    /* 0x1C */ Collider* acHit;
    /* 0x20 */ struct ColliderElement* atHitElem;
    /* 0x24 */ struct ColliderElement* acHitElem;
} ColliderElement; // size = 0x28

typedef struct ColliderElementSrc {
    /* 0x00 */ u8 elemMaterial;
    /* 0x04 */ ColliderElementDamageInfoAT atDmgInfo;
    /* 0x0C */ ColliderElementDamageInfoACSrc acDmgInfo;
    /* 0x14 */ u8 atElemFlags; /* Attack Toucher Flags */
    /* 0x15 */ u8 acElemFlags; /* Bumper Flags */
    /* 0x16 */ u8 ocElemFlags;
} ColliderElementSrc; // size = 0x18

/*
 * Spheres
 */

typedef struct ColliderSpheresElementShape {
    /* 0x00 */ Sphere16 model;
    /* 0x08 */ Sphere16 world;
    /* 0x10 */ f32 unkRadiusScale;
    /* 0x14 */ u8 unk14;
} ColliderSpheresElementShape; /* size = 0x18 */

typedef struct ColliderSpheresElement {
    /* 0x00 */ ColliderElement base;
    /* 0x28 */ ColliderSpheresElementShape shape;
} ColliderSpheresElement; /* size = 0x40 */

typedef struct ColliderSpheres {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 nElements;
    /* 0x1C */ ColliderSpheresElement* elements;
} ColliderSpheres;

typedef struct ColliderSpheresElementShapeSrc {
    u8 unk0;
    Sphere16 unk2;
    s16 unkA;
} ColliderSpheresElementShapeSrc;

typedef struct ColliderSpheresElementSrc {
    ColliderElementSrc base;
    ColliderSpheresElementShapeSrc shape;
} ColliderSpheresElementSrc; /* size = 0x24 */

typedef struct ColliderSpheresSrc {
    ColliderSrc base;
    s32 nElements;
    ColliderSpheresElementSrc* elements;
} ColliderSpheresSrc;

typedef struct ColliderSpheresSrcAltWithActor {
    ColliderSrcAltWithActor base;
    s32 nElements;
    ColliderSpheresElementSrc* elements;
} ColliderSpheresSrcAltWithActor;

typedef struct ColliderSpheresSrcAlt {
    ColliderSrcAlt base;
    s32 nElements;
    ColliderSpheresElementSrc* elements;
} ColliderSpheresSrcAlt;

/*
 * Cylinder
 */

typedef struct ColliderCylinder {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderElement elem;
    /* 0x40 */ Cylinder16 shape;
} ColliderCylinder; // size = 0x4C

typedef struct ColliderCylinderSrc {
    /* 0x00 */ ColliderSrc base;
    /* 0x08 */ ColliderElementSrc elem;
    /* 0x20 */ Cylinder16 shape;
} ColliderCylinderSrc;

typedef struct ColliderCylinderSrcAlt {
    ColliderSrcAlt base;
    ColliderElementSrc elem;
    Cylinder16 shape;
} ColliderCylinderSrcAlt;

typedef struct ColliderCylinderSrcAltWithActor {
    /* 0x00 */ ColliderSrcAltWithActor base;
    /* 0x08 */ ColliderElementSrc elem;
    /* 0x20 */ Cylinder16 shape;
} ColliderCylinderSrcAltWithActor;

/*
 * Tris
 */

typedef struct ColliderTrisElementSrc {
    /* 0x00 */ ColliderElementSrc base;
    /* 0x18 */ Vec3f vertices[3];
} ColliderTrisElementSrc; /* size = 0x3C */

typedef struct ColliderTrisSrc {
    ColliderSrc base;
    s32 nElements;
    ColliderTrisElementSrc* elements;
} ColliderTrisSrc;

typedef struct ColliderTrisSrcAlt {
    ColliderSrcAlt base;
    s32 nElements;
    ColliderTrisElementSrc* elements;
} ColliderTrisSrcAlt;

typedef struct ColliderTrisElement {
    ColliderElement base;
    TriNorm shape;
} ColliderTrisElement; /* size = 0x5C */

typedef struct ColliderTris {
    Collider base;
    s32 nElements;
    ColliderTrisElement* elements;
} ColliderTris;

/*
 * Quad
 */

typedef struct ColliderQuadShapeCorners {
    /* 0x00 */ Vec3f cornerD;
    /* 0x0C */ Vec3f cornerC;
    /* 0x18 */ Vec3f cornerA;
    /* 0x24 */ Vec3f cornerB;
} ColliderQuadShapeCorners; // size = 0x30

typedef struct ColliderQuadSrc {
    /* 0x00 */ ColliderSrc base;
    /* 0x08 */ ColliderElementSrc elem;
    /* 0x20 */ ColliderQuadShapeCorners corners;
} ColliderQuadSrc;

typedef struct ColliderQuadSrcAlt {
    /* 0x00 */ ColliderSrcAlt base;
    /* 0x08 */ ColliderElementSrc elem;
    /* 0x20 */ ColliderQuadShapeCorners corners;
} ColliderQuadSrcAlt;

typedef struct ColliderQuadShape {
    /* 0x00 */ ColliderQuadShapeCorners corners;
    /* 0x30 */ Vec3s middleAB;
    /* 0x36 */ Vec3s middleCD;
    /* 0x3C */ f32 unk3C;
} ColliderQuadShape; /* size = 0x40 */

typedef struct ColliderQuad {
    Collider base;
    ColliderElement elem;
    ColliderQuadShape shape;
} ColliderQuad;

/*
 * ?
 */

typedef struct struct_ColliderCtx_290 {
    char unk_0[0x18];
    u16 unk18;
} struct_ColliderCtx_290;

/*
 * Collider properties, for all types
 */

#define AT_NONE 0 // No flags set. Cannot have AT collisions when set as AT
#define AT_ON (1 << 0) // Can have AT collisions when set as AT
#define AT_HIT (1 << 1) // Had an AT collision
#define AT_BOUNCED (1 << 2) // Had an AT collision with an AC_HARD collider
#define AT_TYPE_PLAYER (1 << 3) // Has player-aligned damage
#define AT_TYPE_ENEMY (1 << 4) // Has enemy-aligned damage
#define AT_TYPE_OTHER (1 << 5) // Has non-aligned damage
#define AT_SELF (1 << 6) // Can have AT collisions with colliders attached to the same actor
#define AT_TYPE_ALL (AT_TYPE_PLAYER | AT_TYPE_ENEMY | AT_TYPE_OTHER) // Has all three damage alignments

#define AC_NONE 0 // No flags set. Cannot have AC collisions when set as AC
#define AC_ON (1 << 0) // Can have AC collisions when set as AC
#define AC_HIT (1 << 1) // Had an AC collision
#define AC_HARD (1 << 2) // Causes AT colliders to bounce off it
#define AC_TYPE_PLAYER AT_TYPE_PLAYER // Takes player-aligned damage
#define AC_TYPE_ENEMY AT_TYPE_ENEMY // Takes enemy-aligned damage
#define AC_TYPE_OTHER AT_TYPE_OTHER // Takes non-aligned damage
#define AC_NO_DAMAGE (1 << 6) // Collider does not take damage
#define AC_BOUNCED (1 << 7) // Caused an AT collider to bounce off it
#define AC_TYPE_ALL (AC_TYPE_PLAYER | AC_TYPE_ENEMY | AC_TYPE_OTHER) // Takes damage from all three alignments

#define OC1_NONE 0 // No flags set. Cannot have OC collisions when set as OC
#define OC1_ON (1 << 0) // Can have OC collisions when set as OC
#define OC1_HIT (1 << 1) // Had an OC collision
#define OC1_NO_PUSH (1 << 2) // Does not push other colliders away during OC collisions
#define OC1_TYPE_PLAYER (1 << 3) // Can have OC collisions with OC type player
#define OC1_TYPE_1 (1 << 4) // Can have OC collisions with OC type 1
#define OC1_TYPE_2 (1 << 5) // Can have OC collisions with OC type 2
#define OC1_TYPE_ALL (OC1_TYPE_PLAYER | OC1_TYPE_1 | OC1_TYPE_2) // Can have collisions with all three OC types

#define OC2_NONE 0 // No flags set. Has no OC type
#define OC2_HIT_PLAYER (1 << 0) // Had an OC collision with OC type player
#define OC2_UNK1 (1 << 1) // Prevents OC collisions with OC2_UNK2. Some horses and toki_sword have it.
#define OC2_UNK2 (1 << 2) // Prevents OC collisions with OC2_UNK1. Nothing has it.
#define OC2_TYPE_PLAYER OC1_TYPE_PLAYER // Has OC type player
#define OC2_TYPE_1 OC1_TYPE_1 // Has OC type 1
#define OC2_TYPE_2 OC1_TYPE_2 // Has OC type 2
#define OC2_FIRST_ONLY (1 << 6) // Skips AC checks on elements after the first collision. Only used by Ganon

#define ATELEM_NONE 0 // No flags set. Cannot have AT collisions
#define ATELEM_ON (1 << 0) // Can have AT collisions
#define ATELEM_HIT (1 << 1) // Had an AT collision
#define ATELEM_NEAREST (1 << 2) // For COLSHAPE_QUAD colliders, only collide with the closest AC element
#define ATELEM_SFX_MASK (3 << 3)
#define ATELEM_SFX_NORMAL (0 << 3) // Hit sound effect based on AC collider's type
#define ATELEM_SFX_HARD (1 << 3) // Always uses hard deflection sound
#define ATELEM_SFX_WOOD (2 << 3) // Always uses wood deflection sound
#define ATELEM_SFX_NONE (3 << 3) // No hit sound effect
#define ATELEM_AT_HITMARK (1 << 5) // Draw hitmarks for every AT collision
#define ATELEM_DREW_HITMARK (1 << 6) // Already drew hitmark for this frame
#define ATELEM_UNK7 (1 << 7) // Unknown purpose. Used by some enemy quads

#define ACELEM_NONE 0 // No flags set. Cannot have AC collisions
#define ACELEM_ON (1 << 0) // Can have AC collisions
#define ACELEM_HIT (1 << 1) // Had an AC collision
#define ACELEM_HOOKABLE (1 << 2) // Can be hooked if actor has hookability flags set.
#define ACELEM_NO_AT_INFO (1 << 3) // Does not give its info to the AT collider that hit it.
#define ACELEM_NO_DAMAGE (1 << 4) // Does not take damage.
#define ACELEM_NO_SWORD_SFX (1 << 5) // Does not have a sound effect when hit by player-attached AT colliders.
#define ACELEM_NO_HITMARK (1 << 6) // Skips hit effects.
#define ACELEM_DRAW_HITMARK (1 << 7) // Draw hitmark for AC collision this frame.

#define OCELEM_NONE 0 // No flags set. Cannot have OC collisions
#define OCELEM_ON (1 << 0) // Can have OC collisions
#define OCELEM_HIT (1 << 1) // Had an OC collision
#define OCELEM_UNK3 (1 << 3) // Unknown purpose. Used by Dead Hand element 0 and Dodongo element 5

#define OCLINE_NONE 0 // Did not have an OcLine collision
#define OCLINE_HIT (1 << 0) // Had an OcLine collision

#define DMG_ENTRY(damage, reaction) ((damage) | ((reaction) << 4))

#define DMG_DEKU_NUT     (1 << 0)
#define DMG_DEKU_STICK   (1 << 1)
#define DMG_SLINGSHOT    (1 << 2)
#define DMG_EXPLOSIVE    (1 << 3)
#define DMG_BOOMERANG    (1 << 4)
#define DMG_ARROW_NORMAL (1 << 5)
#define DMG_HAMMER_SWING (1 << 6)
#define DMG_HOOKSHOT     (1 << 7)
#define DMG_SLASH_KOKIRI (1 << 8)
#define DMG_SLASH_MASTER (1 << 9)
#define DMG_SLASH_GIANT  (1 << 10)
#define DMG_ARROW_FIRE   (1 << 11)
#define DMG_ARROW_ICE    (1 << 12)
#define DMG_ARROW_LIGHT  (1 << 13)
#define DMG_ARROW_UNK1   (1 << 14)
#define DMG_ARROW_UNK2   (1 << 15)
#define DMG_ARROW_UNK3   (1 << 16)
#define DMG_MAGIC_FIRE   (1 << 17)
#define DMG_MAGIC_ICE    (1 << 18)
#define DMG_MAGIC_LIGHT  (1 << 19)
#define DMG_SHIELD       (1 << 20)
#define DMG_MIR_RAY      (1 << 21)
#define DMG_SPIN_KOKIRI  (1 << 22)
#define DMG_SPIN_GIANT   (1 << 23)
#define DMG_SPIN_MASTER  (1 << 24)
#define DMG_JUMP_KOKIRI  (1 << 25)
#define DMG_JUMP_GIANT   (1 << 26)
#define DMG_JUMP_MASTER  (1 << 27)
#define DMG_UNKNOWN_1    (1 << 28)
#define DMG_UNBLOCKABLE  (1 << 29)
#define DMG_HAMMER_JUMP  (1 << 30)
#define DMG_UNKNOWN_2    (1 << 31)

#endif
