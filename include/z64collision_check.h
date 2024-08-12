#ifndef Z64COLLISION_CHECK_H
#define Z64COLLISION_CHECK_H

#define COLLISION_CHECK_AT_MAX 50
#define COLLISION_CHECK_AC_MAX 60
#define COLLISION_CHECK_OC_MAX 50
#define COLLISION_CHECK_OC_LINE_MAX 3

struct Actor;

/*
 * Bases for all shapes of colliders
 */

typedef enum ColliderShape {
    /* 0 */ COLSHAPE_JNTSPH,
    /* 1 */ COLSHAPE_CYLINDER,
    /* 2 */ COLSHAPE_TRIS,
    /* 3 */ COLSHAPE_QUAD,
    /* 4 */ COLSHAPE_MAX
} ColliderShape;

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
    /* 0x00 */ struct Actor* actor; // Attached actor
    /* 0x04 */ struct Actor* at; // Actor attached to what it collided with as an AT collider.
    /* 0x08 */ struct Actor* ac; // Actor attached to what it collided with as an AC collider.
    /* 0x0C */ struct Actor* oc; // Actor attached to what it collided with as an OC collider.
    /* 0x10 */ u8 atFlags;
    /* 0x11 */ u8 acFlags;
    /* 0x12 */ u8 ocFlags1;
    /* 0x13 */ u8 ocFlags2; // Flags related to which colliders it can OC collide with.
    /* 0x14 */ u8 colMaterial; // Determines hitmarks and sound effects during AC collisions. See `ColliderMaterial` enum
    /* 0x15 */ u8 shape; // See `ColliderShape` enum
} Collider; // size = 0x18

typedef struct ColliderInit {
    /* 0x00 */ u8 colMaterial;
    /* 0x01 */ u8 atFlags;
    /* 0x02 */ u8 acFlags;
    /* 0x03 */ u8 ocFlags1;
    /* 0x04 */ u8 ocFlags2;
    /* 0x05 */ u8 shape;
} ColliderInit; // size = 0x06

typedef struct ColliderInitType1 {
    /* 0x00 */ u8 colMaterial;
    /* 0x01 */ u8 atFlags;
    /* 0x02 */ u8 acFlags;
    /* 0x03 */ u8 ocFlags1;
    /* 0x04 */ u8 shape;
} ColliderInitType1; // size = 0x05

typedef struct ColliderInitToActor {
    /* 0x00 */ struct Actor* actor;
    /* 0x04 */ u8 atFlags;
    /* 0x05 */ u8 acFlags;
    /* 0x06 */ u8 ocFlags1;
    /* 0x07 */ u8 shape;
} ColliderInitToActor; // size = 0x08

typedef struct ColliderElementDamageInfoAT {
    /* 0x00 */ u32 dmgFlags; // Damage types dealt by this collider element as AT.
    /* 0x04 */ u8 playerACHitReaction; // Player reaction to a Player AC collider hitting this AT collider
    /* 0x05 */ u8 damage; // Damage
} ColliderElementDamageInfoAT; // size = 0x08

typedef struct ColliderElementDamageInfoAC {
    /* 0x00 */ u32 dmgFlags; // Damage types that may affect this collider element as AC.
    /* 0x04 */ u8 playerATHitReaction; // Player reaction to a Player AT collider hitting this AC collider
    /* 0x05 */ u8 defense; // Damage Resistance
    /* 0x06 */ Vec3s hitPos; // Point of contact
} ColliderElementDamageInfoAC; // size = 0x0C

typedef struct ColliderElementDamageInfoACInit {
    /* 0x00 */ u32 dmgFlags; // Damage types that may affect this collider element as AC.
    /* 0x04 */ u8 playerATHitReaction; // Player reaction to a Player AT collider hitting this AC collider
    /* 0x05 */ u8 defense; // Damage Resistance
} ColliderElementDamageInfoACInit; // size = 0x08

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
    /* 0x00 */ ColliderElementDamageInfoAT atDmgInfo; // Damage properties when acting as an AT collider
    /* 0x08 */ ColliderElementDamageInfoAC acDmgInfo; // Damage properties when acting as an AC collider
    /* 0x14 */ u8 elemMaterial; // Affects sfx when attacked by Player, and interaction with hookshot and arrows.
    /* 0x15 */ u8 atElemFlags; // Information flags for AT collisions
    /* 0x16 */ u8 acElemFlags; // Information flags for AC collisions
    /* 0x17 */ u8 ocElemFlags; // Information flags for OC collisions
    /* 0x18 */ Collider* atHit; // object touching this element's AT collider
    /* 0x1C */ Collider* acHit; // object touching this element's AC collider
    /* 0x20 */ struct ColliderElement* atHitElem; // element that hit the AT collider
    /* 0x24 */ struct ColliderElement* acHitElem; // element that hit the AC collider
} ColliderElement; // size = 0x28

typedef struct ColliderElementInit {
    /* 0x00 */ u8 elemMaterial; // Affects sfx when attacked by Player, and interaction with hookshot and arrows.
    /* 0x04 */ ColliderElementDamageInfoAT atDmgInfo; // Damage properties when acting as an AT collider
    /* 0x0C */ ColliderElementDamageInfoACInit acDmgInfo; // Damage properties when acting as an AC collider
    /* 0x14 */ u8 atElemFlags; // Information flags for AT collisions
    /* 0x15 */ u8 acElemFlags;  // Information flags for AC collisions
    /* 0x16 */ u8 ocElemFlags; // Information flags for OC collisions
} ColliderElementInit; // size = 0x18

/*
 * JntSph - A collider made of sphere shaped elements. Each sphere can attach to a skeleton joint (limb).
 */

// collider structs

typedef struct ColliderJntSphElementDim {
    /* 0x00 */ Sphere16 modelSphere; // model space sphere
    /* 0x08 */ Sphere16 worldSphere; // world space sphere
    /* 0x10 */ f32 scale; // world space sphere = model * scale * 0.01
    /* 0x14 */ u8 limb; // attached limb
} ColliderJntSphElementDim; // size = 0x18

typedef struct ColliderJntSphElement {
    /* 0x00 */ ColliderElement base;
    /* 0x28 */ ColliderJntSphElementDim dim;
} ColliderJntSphElement; // size = 0x40

typedef struct ColliderJntSph {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderJntSphElement* elements;
} ColliderJntSph; // size = 0x20

// init data structs

typedef struct ColliderJntSphElementDimInit {
    /* 0x00 */ u8 limb; // attached limb
    /* 0x02 */ Sphere16 modelSphere; // model space sphere
    /* 0x0A */ s16 scale; // world space sphere = model * scale * 0.01
} ColliderJntSphElementDimInit; // size = 0x0C

typedef struct ColliderJntSphElementInit {
    /* 0x00 */ ColliderElementInit base;
    /* 0x18 */ ColliderJntSphElementDimInit dim;
} ColliderJntSphElementInit; // size = 0x24

typedef struct ColliderJntSphInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderJntSphElementInit* elements;
} ColliderJntSphInit; // size = 0x10

typedef struct ColliderJntSphInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderJntSphElementInit* elements;
} ColliderJntSphInitType1; // size = 0x10

typedef struct ColliderJntSphInitToActor {
    /* 0x00 */ ColliderInitToActor base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderJntSphElementInit* elements;
} ColliderJntSphInitToActor; // size = 0x10

/*
 * Cylinder - A single cylinder shaped collider
 */

// collider structs

typedef struct ColliderCylinder {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderElement elem;
    /* 0x40 */ Cylinder16 dim;
} ColliderCylinder; // size = 0x4C

// init data structs

typedef struct ColliderCylinderInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInit; // size = 0x2C

typedef struct ColliderCylinderInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInitType1; // size = 0x2C

typedef struct ColliderCylinderInitToActor {
    /* 0x00 */ ColliderInitToActor base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInitToActor; // size = 0x2C

/*
 * Tris - A collider made of triangle shaped elements
 */

// collider structs

typedef struct ColliderTrisElement {
    /* 0x00 */ ColliderElement base;
    /* 0x28 */ TriNorm dim;
} ColliderTrisElement; // size = 0x5C

typedef struct ColliderTris {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

// init data structs

typedef struct ColliderTrisElementDimInit {
    /* 0x00 */ Vec3f vtx[3];
} ColliderTrisElementDimInit; // size = 0x24

typedef struct ColliderTrisElementInit {
    /* 0x00 */ ColliderElementInit base;
    /* 0x18 */ ColliderTrisElementDimInit dim;
} ColliderTrisElementInit; // size = 0x3C

typedef struct ColliderTrisInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderTrisElementInit* elements;
} ColliderTrisInit; // size = 0x10

typedef struct ColliderTrisInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderTrisElementInit* elements;
} ColliderTrisInitType1; // size = 0x10

/*
 * Quad - A single quad shaped collider
 */

// collider structs

typedef struct ColliderQuadDim {
    /* 0x00 */ Vec3f quad[4];
    /* 0x30 */ Vec3s dcMid; // midpoint of vectors d, c
    /* 0x36 */ Vec3s baMid; // midpoint of vectors b, a
    /* 0x3C */ f32 acDistSq; // distance to nearest AC collision this frame, squared.
} ColliderQuadDim; // size = 0x40

typedef struct ColliderQuad {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderElement elem;
    /* 0x40 */ ColliderQuadDim dim;
} ColliderQuad; // size = 0x80

// init data structs

typedef struct ColliderQuadDimInit {
    /* 0x00 */ Vec3f quad[4];
} ColliderQuadDimInit; // size = 0x30

typedef struct ColliderQuadInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ ColliderQuadDimInit dim;
} ColliderQuadInit; // size = 0x50

typedef struct ColliderQuadInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ ColliderQuadDimInit dim;
} ColliderQuadInitType1; // size = 0x50

/*
 * Line collider
 */

typedef struct OcLine {
    /* 0x00 */ Linef line;
    /* 0x18 */ u16 ocFlags;
} OcLine; // size = 0x1C

typedef struct CollisionCheckContext {
    /* 0x000 */ s16 colATCount;
    /* 0x002 */ u16 sacFlags;
    /* 0x004 */ Collider* colAT[COLLISION_CHECK_AT_MAX];
    /* 0x0CC */ s32 colACCount;
    /* 0x0D0 */ Collider* colAC[COLLISION_CHECK_AC_MAX];
    /* 0x1C0 */ s32 colOCCount;
    /* 0x1C4 */ Collider* colOC[COLLISION_CHECK_OC_MAX];
    /* 0x28C */ s32 colLineCount;
    /* 0x290 */ OcLine* colLine[COLLISION_CHECK_OC_LINE_MAX];
} CollisionCheckContext; // size = 0x29C

/*
 * Collider properties, for all shapes
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

#define DMG_ENTRY(damage, effect) ((damage) | ((effect) << 4))

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

#define DMG_SLASH (DMG_SLASH_KOKIRI | DMG_SLASH_MASTER | DMG_SLASH_GIANT)
#define DMG_SPIN_ATTACK (DMG_SPIN_KOKIRI | DMG_SPIN_MASTER | DMG_SPIN_GIANT)
#define DMG_JUMP_SLASH (DMG_JUMP_KOKIRI | DMG_JUMP_MASTER | DMG_JUMP_GIANT)
#define DMG_SWORD (DMG_SLASH | DMG_SPIN_ATTACK | DMG_JUMP_SLASH)
#define DMG_HAMMER (DMG_HAMMER_SWING | DMG_HAMMER_JUMP)
#define DMG_FIRE (DMG_ARROW_FIRE | DMG_MAGIC_FIRE)
#define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)
#define DMG_RANGED (DMG_ARROW | DMG_HOOKSHOT | DMG_SLINGSHOT)
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)

#endif
