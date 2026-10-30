/*
 * MacroQuest: The extension platform for EverQuest
 * Copyright (C) 2002-present MacroQuest Authors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include "eqlib/Common.h"
#include "eqlib/SizeChecks.h"
#include "eqlib/graphics/GraphicsEngine.h"
#include "eqlib/game/MathTypes.h"

#include <cstdint>

namespace eqlib {

//============================================================================

//----------------------------------------------------------------------------
class CActor;
class CActorInterface;
class CEQBone;
class CEmitterInterface;
class CParticlePointDefinition;
class CSimpleModel;
class CXStr;
struct SEmitterDefinition;

//============================================================================
// ParticlePointInterface
//============================================================================

// The data behind CActorInterface::AddPointDefinition / GetParticlePointData.
class [[offsetcomments]] CParticlePointDefinition
{
public:
/*0x00*/ CXStr      Tag;
/*0x08*/ int        BoneIndex;
/*0x0c*/ CVector3   Translation;
/*0x18*/ CVector3   Rotation;       // EQ 512-unit angles
/*0x24*/ CVector3   Scale;
/*0x30*/ CMatrix44  Transform;      // built from Translation, Rotation and Scale
/*0x70*/
};

// A named attach point on an actor's model (e.g. R_POINT_DAG), see ActorClient::pParticlePoints.
class [[offsetcomments]] CParticlePointInterface
{
public:
/*0x00*/ virtual CXStr& GetName() = 0;
/*0x08*/ virtual CMatrix44& GetWorldSpaceMatrix() = 0; // refreshes from the bone or model first; row[3] is the world position
/*0x10*/ virtual CVector3& GetPositionOffset() = 0;     // the definition's Translation
};

constexpr size_t CParticlePoint_size = 0x68; // @sizeof(CParticlePoint) :: 2026-09-17 (live) @ 0x18009FBA7

class [[offsetcomments]] CParticlePoint : public CParticlePointInterface
{
public:
/*0x00*/ // vftable
/*0x08*/ CParticlePointDefinition* pDefinition;
/*0x10*/ CMatrix44                 worldMatrix;
/*0x50*/ void*                     pHierarchicalModel;
/*0x58*/ CSimpleModel*             pSimpleModel;
/*0x60*/ CEQBone*                  pBone;
/*0x68*/
};

SIZE_CHECK(CParticlePoint, CParticlePoint_size);

//============================================================================
// Emitter
//============================================================================

struct SEmitterDefData;

class [[offsetcomments]] CEmitterInterface
{
public:
/*0x00*/ virtual void SetBoundingScale(float scale, const SEmitterDefData*) = 0;
/*0x08*/ virtual float GetBoundingScale() const = 0;
/*0x10*/ virtual float GetBoundingRadius() const = 0;
/*0x18*/ virtual void SetAlwaysVisible(bool) = 0;
/*0x20*/ virtual bool IsAlwaysVisible() const = 0;
/*0x28*/ virtual void SetLocation(const CVector3&) = 0;
/*0x30*/ virtual void SetOrientation(const CMatrix33&) = 0;
/*0x38*/ virtual void SetTransform(const CMatrix44&) = 0;
/*0x40*/ virtual void SetLifeRemaining(float) = 0;
/*0x48*/ virtual float GetLifeRemaining() const = 0;
/*0x50*/ virtual ~CEmitterInterface() {}
};

constexpr size_t CEmitter_size = 0xC8; // @sizeof(CEmitter) :: 2026-09-17 (live) @ 0x18009DD6D

class [[offsetcomments]] CEmitter : public CEmitterInterface
{
public:
/*0x08*/ bool                     bInitialized;       // set once the particle buffer is allocated
/*0x09*/ uint8_t                  Unknown0x0009[0x10 - 0x09];
/*0x10*/ int                      MaxParticles;
/*0x14*/ uint8_t                  Unknown0x0014[0x3c - 0x14];
/*0x3c*/ CVector3                 Position;
/*0x48*/ CMatrix33                Orientation;
/*0x6c*/ uint8_t                  Unknown0x006c[0x70 - 0x6c];
/*0x70*/ CActorInterface*         pActor;
/*0x78*/ void*                    Unknown0x0078;      // position source, used ahead of pActor
/*0x80*/ CParticlePointInterface* pAttachPoint;       // when set, the emitter follows it every frame
/*0x88*/ void*                    pParticles;         // 0x80-byte particle records
/*0x90*/ void*                    pParticlesEnd;      // pParticles + MaxParticles records
/*0x98*/ uint8_t                  Unknown0x0098[0xc8 - 0x98];
/*0xc8*/
};

SIZE_CHECK(CEmitter, CEmitter_size);

// Returned by CSceneGraph::CreateParticleCloudInstance.
class [[offsetcomments]] CParticleCloudInterface
{
public:
/*0x00*/ virtual ~CParticleCloudInterface() {}
/*0x08*/ virtual CEmitterInterface* GetEmitterInterface() const = 0;
};

class CParticleSystemInterface
{
public:
/*0x000*/ virtual void UpdateAllEmittersScalingRadius(CActorInterface* actor) = 0;
/*0x008*/ virtual int LoadEmitterDefinitions() = 0;
/*0x010*/ virtual bool IsSpellsDefInfiniteDuration(int defIndex) = 0;
/*0x018*/ virtual bool IsEnvironmentDefInfiniteDuration(int defIndex) = 0;
/*0x020*/ virtual void FreeTextureCache() = 0;
/*0x028*/ virtual void FreeAllGroups() = 0;
/*0x030*/ virtual int CreateEmitterInSpells(int defIndex, int emitterId, int durationMs, float density, float alpha,
		const CVector3* position, CActor* actor, CEQBone* bone, CParticlePointInterface* attachPoint,
		CEmitterInterface** outEmitter, float scale, bool autoDeleteWhenEmpty, bool ignoreNearClip, int viewId) = 0;
/*0x038*/ virtual int CreateEmitterInEnvironment(int defIndex, int emitterId, int durationMs, float density, float alpha,
		const CVector3* position, CActor* actor, CEQBone* bone, CEmitterInterface** outEmitter,
		bool autoDeleteWhenEmpty, bool ignoreNearClip) = 0;
/*0x040*/ virtual int CreateEmitterInActors(int defIndex, int emitterId, int durationMs, float density, float alpha,
		const CVector3* position, const CMatrix44* axes, CParticlePointInterface* attachPoint, bool autoDeleteWhenEmpty,
		float scale, CEmitterInterface** outEmitter, int viewId) = 0;
/*0x048*/ virtual int ReattachEmittersToActor(CActor* oldActor, CActor* newActor) = 0;
/*0x050*/ virtual int DeleteEmitterInSpells(CEmitterInterface* emitter) = 0;
/*0x058*/ virtual int DeleteEmitterInEnvironment(CEmitterInterface* emitter) = 0;
/*0x060*/ virtual int DeleteEmitterInActors(CEmitterInterface* emitter) = 0;
/*0x068*/ virtual int DeleteEmittersForActor(CActor* actor) = 0;
/*0x070*/ virtual int DeleteEmittersById(int emitterId) = 0;
/*0x078*/ virtual void SetNearClipDistances(float spells, float environment, float actors) = 0;
/*0x080*/ virtual int GetSpellsSlotCount() = 0;
/*0x088*/ virtual int GetEnvironmentSlotCount() = 0;
/*0x090*/ virtual int GetActorsSlotCount() = 0;
/*0x098*/ virtual SEmitterDefinition* GetSpellsDefArray() = 0;
/*0x0a0*/ virtual SEmitterDefinition* GetEnvironmentDefArray() = 0;
/*0x0a8*/ virtual SEmitterDefinition* GetActorsDefArray() = 0;
/*0x0b0*/ virtual int AddDefInSpells(int copyFromIndex) = 0;
/*0x0b8*/ virtual int AddDefInEnvironment(int copyFromIndex) = 0;
/*0x0c0*/ virtual int AddDefInActors(int copyFromIndex) = 0;
/*0x0c8*/ virtual int MergeDuplicateDefInSpells(int defIndex, int* outDuplicateIndex) = 0;
/*0x0d0*/ virtual int MergeDuplicateDefInEnvironment(int defIndex, int* outDuplicateIndex) = 0;
/*0x0d8*/ virtual int MergeDuplicateDefInActors(int defIndex, int* outDuplicateIndex) = 0;
/*0x0e0*/ virtual int LoadDefsFromFileInSpells(const char* filename) = 0;
/*0x0e8*/ virtual int LoadDefsFromFileInEnvironment(const char* filename) = 0;
/*0x0f0*/ virtual int LoadDefsFromFileInActors(const char* filename) = 0;
/*0x0f8*/ virtual void RemoveDefInSpells(int defIndex) = 0;
/*0x100*/ virtual int SaveDefsToFileInSpells(const char* filename) = 0;
/*0x108*/ virtual int SaveDefsToFileInEnvironment(const char* filename) = 0;
/*0x110*/ virtual int SaveDefsToFileInActors(const char* filename) = 0;
/*0x118*/ virtual int SaveSingleDefToFileInSpells(const SEmitterDefinition* def, const char* filename) = 0;
/*0x120*/ virtual int SaveSingleDefToFileInEnvironment(const SEmitterDefinition* def, const char* filename) = 0;
/*0x128*/ virtual int SaveSingleDefToFileInActors(const SEmitterDefinition* def, const char* filename) = 0;
/*0x130*/ virtual int InvalidateSlotTextureInSpells(int slotIndex) = 0;
/*0x138*/ virtual int InvalidateSlotTextureInEnvironment(int slotIndex) = 0;
/*0x140*/ virtual int InvalidateSlotTextureInActors(int slotIndex) = 0;
/*0x148*/ virtual void SetEnvironmentParticleDensity(float density) = 0;
/*0x150*/ virtual void SetEnvironmentParticleOpacity(float opacity) = 0;
/*0x158*/ virtual void SetActorParticleDensity(float density) = 0;
/*0x160*/ virtual void SetActorParticleOpacity(float opacity) = 0;
/*0x168*/ virtual bool GetClimateType() = 0;
/*0x170*/ virtual void SetClimateType(bool climateType) = 0;
/*0x178*/ virtual ~CParticleSystemInterface() {}
/*0x180*/ virtual int DeleteEmitterInSpellEmitters(CEmitterInterface* emitter) = 0;
/*0x188*/ virtual int DeleteEmittersForAttachObject(CParticlePointInterface* attachObj) = 0;
/*0x190*/ virtual void DeleteAllEmitters() = 0;
};

//----------------------------------------------------------------------------
constexpr size_t CParticle_size = 0x80; // @sizeof(CParticle) :: 2026-09-17 (live) @ 0x18009D60F

struct [[offsetcomments]] CParticle
{
/*0x00*/ float                    origin[3];
/*0x0c*/ float                    axis0[3];
/*0x18*/ float                    axis1[3];
/*0x24*/ float                    axis2[3];
/*0x30*/ float                    angle512;
/*0x34*/ float                    radius;
/*0x38*/ float                    height;
/*0x3c*/ float                    radialDirCos;
/*0x40*/ float                    radialDirSin;
/*0x44*/ float                    velAxis0;
/*0x48*/ float                    velAxis1;
/*0x4c*/ float                    velAxis2;
/*0x50*/ float                    velRadial;
/*0x54*/ float                    velAngle512;
/*0x58*/ float                    velGravity;
/*0x5c*/ float                    lifetime;
/*0x60*/ float                    remaining;
/*0x64*/ float                    rotInit512;
/*0x68*/ float                    rotSpeed512;
/*0x6c*/ float                    width;
/*0x70*/ float                    heightSize;
/*0x74*/ float                    drift[3];
/*0x80*/
};

SIZE_CHECK(CParticle, CParticle_size);

constexpr size_t SEmitterDefinition_size = 0x1A0; // @sizeof(SEmitterDefinition) :: 2026-09-17 (live) @ 0x1800B4126

struct [[offsetcomments]] SEmitterDefinition
{
/*0x000*/ char                    name[64];
/*0x040*/ char                    texture[32];
/*0x060*/ int                     attachOrientMode;
/*0x064*/ int                     noZWrite;
/*0x068*/ int                     additive;
/*0x06c*/ int                     scaleByActor;
/*0x070*/ int                     renderMode;           // 0 billboard, 1 streak, 2 flat
/*0x074*/ int                     followEmitter;
/*0x078*/ float                   emitterDuration;
/*0x07c*/ float                   particleLifetime;
/*0x080*/ int                     burstCount;
/*0x084*/ int                     emitPerTick;
/*0x088*/ float                   ticksPerSecond;
/*0x08c*/ float                   emitDelay;
/*0x090*/ float                   alphaFadeIn;
/*0x094*/ float                   alphaFadeOut;
/*0x098*/ float                   sizeGrowIn;
/*0x09c*/ float                   sizeShrinkOut;
/*0x0a0*/ float                   lodDistance;
/*0x0a4*/ float                   maxAlpha;
/*0x0a8*/ int                     spawnShape;
/*0x0ac*/ float                   spawnRadius;
/*0x0b0*/ float                   spawnRadius2;
/*0x0b4*/ float                   spawnHeight;
/*0x0b8*/ float                   spawnOffset[3];
/*0x0c4*/ float                   tiltAxis01_512;
/*0x0c8*/ float                   tiltAxis02_512;
/*0x0cc*/ float                   widthMin;
/*0x0d0*/ float                   depthBias;
/*0x0d4*/ int                     colorStart[3];
/*0x0e0*/ int                     colorEnd[3];
/*0x0ec*/ float                   velAxis0Min, velAxis0Max, accAxis0;
/*0x0f8*/ float                   velAxis1Min, velAxis1Max, accAxis1;
/*0x104*/ float                   velAxis2Min, velAxis2Max, accAxis2;
/*0x110*/ float                   velRadialMin, velRadialMax, accRadial;
/*0x11c*/ float                   velAngleMin, velAngleMax, accAngle;
/*0x128*/ float                   gravity;
/*0x12c*/ float                   windX;
/*0x130*/ int                     spriteCells;
/*0x134*/ float                   spriteFps;
/*0x138*/ float                   rotSpeedMin;
/*0x13c*/ int                     legacyCloudParam0;
/*0x140*/ uint32_t                legacyCloudFlags;
/*0x144*/ int                     fixedParticleCount;
/*0x148*/ float                   legacyCloudVectors[3][3];
/*0x16c*/ float                   sizeScale;
/*0x170*/ float                   legacyMaxAlpha;
/*0x174*/ int                     randomInitRotation;
/*0x178*/ int                     screenAlignAxis;
/*0x17c*/ float                   heightMin;
/*0x180*/ float                   heightMax;
/*0x184*/ float                   widthMax;
/*0x188*/ float                   rotSpeedMax;
/*0x18c*/ int                     uniformSize;
/*0x190*/ float                   heightShrinkOut;
/*0x194*/ float                   widthShrinkOut;
/*0x198*/ int                     renderNegativeRadius;
/*0x19c*/ int                     spawnScaleByActor;
/*0x1a0*/

};

SIZE_CHECK(SEmitterDefinition, SEmitterDefinition_size);

constexpr size_t CEmitterDefSlot_size = 0x38; // @sizeof(CEmitterDefSlot) :: 2026-09-17 (live) @ 0x18009F6A0

class [[offsetcomments]] CEmitterDefSlot
{
public:
/*0x00*/ virtual ~CEmitterDefSlot() {}

/*0x08*/ bool                     enabled;
/*0x0c*/ int                      firstVertex;
/*0x10*/ int                      primCount;
/*0x18*/ CEmitter*                emitters;
/*0x20*/ SEmitterDefinition*      def;
/*0x28*/ void*                    texture;
/*0x30*/ float                    boundRadius;
/*0x38*/
};

SIZE_CHECK(CEmitterDefSlot, CEmitterDefSlot_size);

struct [[offsetcomments]] SParticleGroup
{
/*0x00*/ int                      defCount;
/*0x08*/ SEmitterDefinition*      defs;
/*0x10*/ int                      defAlloc;
/*0x14*/ bool                     defValid;
/*0x18*/ int                      count;
/*0x20*/ CEmitterDefSlot**        slots;
/*0x28*/ int                      slotAlloc;
/*0x2c*/ bool                     slotValid;
/*0x30*/ void*                    pVB;
/*0x38*/
};


struct [[offsetcomments]] SParticleCamera
{
/*0x00*/ float                    left;
/*0x04*/ float                    centerX;
/*0x08*/ float                    right;
/*0x0c*/ float                    top;
/*0x10*/ float                    centerY;
/*0x14*/ float                    bottom;
/*0x18*/ float                    focalPx;
/*0x1c*/ float                    zScaleA;
/*0x20*/ float                    nearClipB;
/*0x24*/ float                    farClip;
/*0x28*/ float                    camPos[3];
/*0x34*/ float                    worldToEye[9];
/*0x58*/
};

constexpr size_t SParticleTextureCacheEntry_size = 0x30; // @sizeof(SParticleTextureCacheEntry) :: 2026-09-17 (live) @ 0x1800B3762

struct [[offsetcomments]] SParticleTextureCacheEntry
{
/*0x00*/ char                        name[0x20];
/*0x20*/ void*                       texture;
/*0x28*/ SParticleTextureCacheEntry* pNext;
/*0x30*/
};

SIZE_CHECK(SParticleTextureCacheEntry, SParticleTextureCacheEntry_size);

constexpr size_t CParticleSystem_size = 0x118; // @sizeof(CParticleSystem) :: 2026-09-17 (live) @ 0x180019EB4

class [[offsetcomments]] CParticleSystem : public CParticleSystemInterface
{
public:
/*0x000*/ // vftable
/*0x008*/ int                     lastTickMs;
/*0x00c*/ float                   nearClipSpells;
/*0x010*/ float                   nearClipFogged;
/*0x014*/ float                   nearClipActors;
/*0x018*/ SParticleGroup          spells;
/*0x050*/ SParticleGroup          spellEmitters;
/*0x088*/ SParticleGroup          environment;
/*0x0c0*/ SParticleGroup          actors;
/*0x0f8*/ SParticleTextureCacheEntry* pTextureList;
/*0x100*/ float                   environmentParticleDensity;
/*0x104*/ float                   environmentParticleOpacity;
/*0x108*/ float                   actorParticleDensity;
/*0x10c*/ float                   actorParticleOpacity;
/*0x110*/ bool                    climateType;
/*0x114*/ int                     viewId;
/*0x118*/
};

SIZE_CHECK(CParticleSystem, CParticleSystem_size);

//============================================================================

} // namespace eqlib
