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

#include "eqlib/graphics/RenderInterface.h"
#include "eqlib/graphics/NodePool.h"
#include "eqlib/SizeChecks.h"

#include "eqlib/game/EQDX9.h"
#include "eqlib/game/MathTypes.h"

struct ID3DXMesh;
struct ID3D10Multithread;

namespace eqlib {

//============================================================================

class CBufferSet;
class CEffect;
class CLight;
class CMaterial;
class CMemoryPoolManager;
class CRenderEffect;
class CTextureProxy;

struct SBoundingRect;
struct SLitBatch;
struct SLitTriangle;
struct STexture;
struct STextureSet;

//----------------------------------------------------------------------------
constexpr int NUM_BLEND_MATRICES = 8;
constexpr int MAX_BUFFER_SET = 35;
constexpr int MAX_EFFECTS = 161;
constexpr int MAX_RENDER_EFFECTS = 225;
constexpr int MAX_VERTEX_DECLARATIONS = 21;
constexpr int MAX_EFFECT_PASS_COUNT = 6;

using RenderCallbackPtr = void(*)();

//----------------------------------------------------------------------------
enum EBufferSet
{
	BufferSet_Region,
	BufferSet_RegionBump,
	BufferSet_Region2UV,
	BufferSet_RegionBump2UV,
	BufferSet_RegionTint,
	BufferSet_RegionBumpTint,
	BufferSet_Region2UVTint,
	BufferSet_RegionBump2UVTint,

	BufferSet_SimpleModelGlobal,
	BufferSet_SimpleModelGlobalBump,
	BufferSet_SimpleModelGlobal2UV,
	BufferSet_SimpleModelGlobalBump2UV,
	BufferSet_SimpleModelGlobalTint,
	BufferSet_SimpleModelGlobalBumpTint,
	BufferSet_SimpleModelGlobal2UVTint,
	BufferSet_SimpleModelGlobalBump2UVTint,

	BufferSet_SimpleModelOnDemand,
	BufferSet_SimpleModelOnDemandBump,
	BufferSet_SimpleModelOnDemand2UV,
	BufferSet_SimpleModelOnDemandBump2UV,
	BufferSet_SimpleModelOnDemandTint,
	BufferSet_SimpleModelOnDemandBumpTint,
	BufferSet_SimpleModelOnDemand2UVTint,
	BufferSet_SimpleModelOnDemandBump2UVTint,

	BufferSet_SimpleModelZone,
	BufferSet_SimpleModelZoneBump,
	BufferSet_SimpleModelZone2UV,
	BufferSet_SimpleModelZoneBump2UV,
	BufferSet_SimpleModelZoneTint,
	BufferSet_SimpleModelZoneBumpTint,
	BufferSet_SimpleModelZone2UVTint,
	BufferSet_SimpleModelZoneBump2UVTint,

	BufferSet_Terrain,
	BufferSet_LitData,

	BufferSet_Count,
};

//----------------------------------------------------------------------------
enum ERenderEffect
{
	
};

//----------------------------------------------------------------------------
enum ERenderMaterial
{
	RenderMaterial_Opaque,
	RenderMaterial_Chroma,
	RenderMaterial_AlphaSingleDetail,
	RenderMaterial_AlphaPaletteDetail,
	RenderMaterial_AlphaBatchAdditive,
	RenderMaterial_AlphaBatch,
	RenderMaterial_OpaqueC1,
	RenderMaterial_OpaqueCG1,
	RenderMaterial_OpaqueCE1,
	RenderMaterial_OpaqueCB1,
	RenderMaterial_OpaqueCBS1,
	RenderMaterial_OpaqueCBS1_VSB,
	RenderMaterial_OpaqueCBS_2UV,
	RenderMaterial_OpaqueCBSG1,
	RenderMaterial_OpaqueCBSGE1,
	RenderMaterial_OpaqueC1_2UV,
	RenderMaterial_OpaqueCB1_2UV,
	RenderMaterial_OpaqueCBSG1_2UV,
	RenderMaterial_OpaqueCBST2_2UV,
	RenderMaterial_OpaqueTerrain,
	RenderMaterial_OpaqueLava,
	RenderMaterial_OpaqueLava2,
	RenderMaterial_OpaqueBasic,
	RenderMaterial_OpaqueBlend,
	RenderMaterial_OpaqueBlendNoBump,
	RenderMaterial_OpaqueFull,
	RenderMaterial_OpaqueFull_2UV,
	RenderMaterial_OpaqueBump,
	RenderMaterial_OpaqueBump_2UV,
	RenderMaterial_OpaqueSB,
	RenderMaterial_OpaqueSB_2UV,
	RenderMaterial_OpaqueGB,
	RenderMaterial_OpaqueGB_2UV,
	RenderMaterial_OpaqueRB,
	RenderMaterial_OpaqueRB_2UV,
	RenderMaterial_ChromaC1,
	RenderMaterial_ChromaCG1,
	RenderMaterial_ChromaCE1,
	RenderMaterial_ChromaCB1,
	RenderMaterial_ChromaCBS1,
	RenderMaterial_ChromaCBSG1,
	RenderMaterial_ChromaCBSGE1,
	RenderMaterial_ChromaBasic,
	RenderMaterial_ChromaBump,
	RenderMaterial_AlphaC1,
	RenderMaterial_AlphaCG1,
	RenderMaterial_AlphaCE1,
	RenderMaterial_AlphaCB1,
	RenderMaterial_AlphaCBS1,
	RenderMaterial_AlphaCBSG1,
	RenderMaterial_AlphaCBSGE1,
	RenderMaterial_AlphaBasic,
	RenderMaterial_AlphaBump,
	RenderMaterial_AlphaWater,
	RenderMaterial_AlphaWaterFall,
	RenderMaterial_AlphaLavaH,
	RenderMaterial_AddAlphaC1,
	RenderMaterial_AddAlphaCG1,
	RenderMaterial_AddAlphaCE1,
	RenderMaterial_AddAlphaCB1,
	RenderMaterial_AddAlphaCBS1,
	RenderMaterial_AddAlphaCBSG1,
	RenderMaterial_AddAlphaCBSGE1,
};

//----------------------------------------------------------------------------
struct [[offsetcomments]] SModeInfo
{
/*0x00*/ bool bDisplay;
/*0x01*/ bool bDepth16;
/*0x02*/ bool bDepth24;
/*0x03*/ bool bDepth32;
/*0x04*/ bool bWindowed;
/*0x08*/
};

//----------------------------------------------------------------------------
constexpr size_t SDevice_size = 0xe4; // @sizeof(SDevice) :: 2026-09-17 (live) @ 0x1800D9EC4

struct [[offsetcomments]] SDevice
{
/*0x00*/ uint32_t            unk1;
/*0x04*/ uint32_t            index;
/*0x08*/ char                name[128];
/*0x88*/ int                 product;
/*0x8c*/ int                 version;
/*0x90*/ int                 subversion;
/*0x94*/ int                 build;
/*0x98*/ int                 vendorID;
/*0x9c*/ int                 deviceID;
/*0xa0*/ GUID                deviceGUID;
/*0xb0*/ GUID                driverGUID;
/*0xc0*/ bool                supportsLargeTextures;
/*0xc1*/ bool                supportsHardwareTnL;
/*0xc2*/ bool                supportsVertexShaders;
/*0xc3*/ bool                supportsIndexedVertexBlending;
/*0xc4*/ bool                useFixedFunctionVertexBlending;
/*0xc8*/ int                 vsyncInterval;
/*0xcc*/ uint32_t            num16BitVideoModes;
/*0xd0*/ uint32_t            num32BitVideoModes;
/*0xd4*/ SModeInfo           mode32;
/*0xd9*/ SModeInfo           mode16A;
/*0xde*/ SModeInfo           mode16B;
/*0xe4*/
};

SIZE_CHECK(SDevice, SDevice_size);

//----------------------------------------------------------------------------
struct SBatchDefinition
{
	SBatchDefinition*        pBatchDefinitionNext;
	EBufferSet               BufferSetType;
	ERenderMaterial          RenderMaterialType;
	uint32_t                 uMaterialIndex;
	CMaterial*               pMaterial;
	STextureSet*             pTextureSet;
	IDirect3DIndexBuffer9*   pIndexBuffer0;
	IDirect3DVertexBuffer9*  pVertexBuffer0;
	uint32_t                 uVertexOffset0;
	uint32_t                 uMinIndex;
	uint32_t                 uNumFrames;
	uint32_t                 uStartIndex;
	uint32_t                 uPrimitiveCount;
	int32_t                  nAlpha;
	int32_t                  nMinVertexIndex;
	int32_t                  nMaxVertexIndex;
	SBoundingRect*           pBoundingRect;
};

//----------------------------------------------------------------------------
struct SBatch
{
	SBatch*                  pBatchNext;
	SBatchDefinition*        pBatchDefinition;
	ERenderEffect            eRenderEffectType;
	CMatrix44*               pm44ObjectToWorldTransform;
	void**                   ppDPVSObject;
	RGB*                     prgbTint;
	D3DCOLOR                 colorAlpha;
	float*                   pfShadeFactor;
	int32_t                  nCurrentFrame;
	IDirect3DVertexBuffer9*  pVertexBuffer1;
	uint32_t                 uVertexOffset1;
	bool                     bDeferred;
};

//----------------------------------------------------------------------------
struct SBoundingRect
{
	float                    fMinX;
	float                    fMaxX;
	float                    fMinY;
	float                    fMaxY;
};


//----------------------------------------------------------------------------
struct SDetailTextureData
{
	STexture*                pDetailTexture;
	uint32_t*                puScale;
};

//----------------------------------------------------------------------------
struct SLitBatch
{
	SLitTriangle*            pLitTriangleHead;
	IDirect3DVertexBuffer9*  pVertexBuffer0;
};

//----------------------------------------------------------------------------
struct SLitTriangle
{
	SLitTriangle*            pLitTriangleNext;
	uint16_t                 uVertexIndex[3];
	uint16_t                 uPad;
};

//----------------------------------------------------------------------------
struct SSubset
{
	ERenderEffect            eRenderEffectType;
	CMaterial**              ppMaterial;
	RGB*                     prgbTint;
	RGB*                     prgbTint2;
	int32_t                  nAlpha;
	float*                   pfShadeFactor;
	uint32_t                 uVertexBlendFlag;
	uint32_t                 uNumMatrices;
	CMatrix44*               ppm44BoneToWorldTransforms;
	void**                   ppDPVSObject;
	uint32_t                 uIndex;
	ID3DXMesh*               pMesh;
	bool                     bHasBlendWeights;
	uint32_t                 uFaceCount;
	bool                     bFading;
	bool                     bDeferred;
};

//----------------------------------------------------------------------------
struct STerrainBatch
{
	EBufferSet               BufferSetType;
	IDirect3DIndexBuffer9**  ppIndexBuffer;
	uint32_t*                puStartIndex;
	uint32_t*                puPrimitiveCount;
	IDirect3DVertexBuffer9*  pVertexBuffer;
	uint32_t                 uVertexOffset;
	uint32_t                 uMinIndex;
	int32_t                  nMinVertexIndex;
	int32_t                  nMaxVertexIndex;
	STexture*                pTexture;
	SDetailTextureData*      pDetailTextures;
	uint32_t*                puNumDetailTextures;
	ERenderEffect            eRenderEffectType;
	void**                   ppDPVSObject;
	bool                     bDetailsHaveBump;
	bool                     bFreeVB;
	bool                     bShared;
	bool                     bDeferred;
};

//----------------------------------------------------------------------------
struct STexture
{
	uint32_t                 uFlags;
	const char*              pszFileName;
	CTextureProxy*           aTextures[8];
};

//----------------------------------------------------------------------------
struct STextureSet
{
	bool                     bSkipFrames;
	uint32_t                 uTimeUpdateInterval;
	uint32_t                 uTimeNextUpdate;
	uint32_t                 uNumTextures;
	uint32_t                 uCurrentTexture;
	STexture**               ppTextures;
	CMatrix44*               pm44UVTransform;
};

//----------------------------------------------------------------------------
class CBatchNode : public TDataNode<CBatchNode, SBatch>
{
};

//----------------------------------------------------------------------------
class CTerrainBatchNode : public TDataNode<CTerrainBatchNode, STerrainBatch>
{
};

//----------------------------------------------------------------------------
class CSubsetNode : public TDataNode<CSubsetNode, SSubset>
{
};

//----------------------------------------------------------------------------
class CLitBatchNode : public TDataNode<CLitBatchNode, SLitBatch>
{
};

//----------------------------------------------------------------------------
class CRenderNode : public TListNode<CRenderNode>
{
public:
	TList<CBatchNode>        m_BatchList;
	TList<CTerrainBatchNode> m_TerrainBatchList;
	TList<CSubsetNode>       m_SubsetList;
	TList<CLitBatchNode>     m_LitBatchList;
	ERenderEffect            m_eRenderEffectType;
	CMaterial*               m_pMaterial;
	STexture*                m_pTexture;
};

//----------------------------------------------------------------------------
class CLightNode : public TListNode<CLightNode>
{
public:
	CLight*                  m_pLight;
};

//----------------------------------------------------------------------------
struct [[offsetcomments]] SStreamSourceState
{
/*0x00*/ Direct3DVertexBuffer9*    pVertexBuffer;
/*0x08*/ uint32_t                  offset;
/*0x0c*/ uint32_t                  stride;
/*0x10*/
};

struct [[offsetcomments]] SRenderStateBlock
{
/*0x0000*/ uint32_t                  fvf;
/*0x0008*/ uint64_t                  Unknown0x0008;
/*0x0010*/ Direct3DIndexBuffer9*     pIndices;
/*0x0018*/ uint32_t                  renderStates[210];
/*0x0360*/ uint32_t                  samplerStates[16][14];
/*0x06e0*/ SStreamSourceState        streams[16];
/*0x07e0*/ Direct3DTexture9*         textures[16];
/*0x0860*/ uint32_t                  textureStageStates[16][33];
/*0x10a0*/ IDirect3DVertexDeclaration9* pVertexDeclaration;
/*0x10a8*/
};

class [[offsetcomments]] CRenderStateCache
{
public:
/*0x0000*/ SRenderStateBlock         pending;
/*0x10a8*/ SRenderStateBlock         applied;
/*0x2150*/ SRenderStateBlock         snapshot;
/*0x31f8*/ bool                      bSnapshotValid;
/*0x31f9*/ uint8_t                   Unknown0x31f9[0xab];
/*0x32a4*/ int                       currentPreset;
/*0x32a8*/ int                       lastFlushedPreset;
/*0x32ac*/ bool                      bStatesDirty;
/*0x32ad*/ bool                      bTexturesDirty;
/*0x32ae*/ bool                      bStreamsDirty;
/*0x32b0*/ int                       maxDirtyRenderState;
/*0x32b4*/ int                       minDirtyRenderState;
/*0x32b8*/ bool                      bLightingDirty;
/*0x32b9*/ bool                      bTextureFactorDirty;
/*0x32bc*/ int                       maxDirtySampler;
/*0x32c0*/ int                       samplerMaxDirtyState[16];
/*0x3300*/ int                       samplerMinDirtyState[16];
/*0x3340*/ int                       maxDirtyStream;
/*0x3344*/ int                       maxDirtyTexture;
/*0x3348*/ int                       maxDirtyTextureStage;
/*0x334c*/ int                       stageMaxDirtyState[16];
/*0x338c*/ int                       stageMinDirtyState[16];
/*0x33cc*/ int                       flushCount;
/*0x33d0*/ int                       resourceOnlyFlushCount;
/*0x33d4*/ int                       presetCount;
/*0x33d8*/ int                       stateChanges;
/*0x33dc*/ int                       redundantSets;
/*0x33e0*/ int                       deviceCalls;
/*0x33e4*/
};

//----------------------------------------------------------------------------
constexpr size_t CRender_size = 0xf9f0; // @sizeof(CRender) :: 2026-09-17 (live) @ 0x180019E75

class [[offsetcomments]] CRender : public CRenderInterface
{
public:
	CRenderStateCache* GetStateCache() { return &stateCache; }

/*0x0000*/ //vftable
/*0x0008*/ ID3D10Multithread*        pMultithread;
/*0x0010*/ SDevice                   aDevices[16];
/*0x0e50*/ int                       nDeviceCount;
/*0x0e58*/ SDevice*                  pCurrentDevice;
/*0x0e60*/ bool                      bDeviceInitialized;
/*0x0e61*/ bool                      bResettingDevice;
/*0x0e64*/ D3DFORMAT                 adapterFormat;
/*0x0e68*/ D3DDISPLAYMODE            desktopMode;
/*0x0e78*/ int                       nDisplayWidth;
/*0x0e7c*/ int                       nDisplayHeight;
/*0x0e80*/ int                       nDisplayDepth;
/*0x0e84*/ int                       nDisplayRefreshRate;
/*0x0e88*/ bool                      bFullscreen;
/*0x0e89*/ bool                      bWindowedModeAvailable;
/*0x0e90*/ D3DPRESENT_PARAMETERS     d3dpp;
/*0x0ed0*/ uint32_t                  frameId;
/*0x0ed4*/ bool                      bSupportsMipMaps;
/*0x0ed5*/ bool                      bSupportsTrilinearMipMaps;
/*0x0ed6*/ bool                      bAutoMipMapping;
/*0x0ed8*/ uint32_t                  uMaxVertexBlendMatrices;
/*0x0edc*/ bool                      bMultithreadAvailable;
/*0x0edd*/ bool                      bSupportsDXT1Textures;
/*0x0ede*/ bool                      bSupportsDXT3Textures;
/*0x0edf*/ bool                      bSupports4444Textures;
/*0x0ee0*/ bool                      bSupportsDotProduct3;
/*0x0ee1*/ bool                      bRGB565Mode;
/*0x0ee2*/ bool                      bUseMode16A;
/*0x0ee3*/ bool                      bUseSoftwareVertexProcessing;
/*0x0ee4*/ bool                      bUseMixedVertexProcessing;
/*0x0ee5*/ bool                      bUseHardwareVertexProcessing;
/*0x0ee6*/ bool                      bUseHardwareVertexShaders;
/*0x0ee7*/ bool                      bUseHardwareIndexedVertexBlending;
/*0x0ee8*/ bool                      bUse1PassTechniques;
/*0x0eec*/ uint32_t                  uTotalTextureMemory;
/*0x0ef0*/ int                       nTextureQuality;
/*0x0ef8*/ HWND                      hWnd;
/*0x0f00*/ Direct3D9*                pDirect3D;
/*0x0f08*/ Direct3DDevice9*          pD3DDevice;
/*0x0f10*/ IDirect3DSurface9*        pD3DBackBuffer;
/*0x0f18*/ D3DVIEWPORT9              D3DViewPort;
/*0x0f30*/ D3DCAPS9                  D3DDeviceCaps;
/*0x1060*/ ID3DXBuffer*              screenCapFileData;
/*0x1070*/ __declspec(align(16)) CMatrix44 matrixIdentity;
/*0x10b0*/ __declspec(align(16)) CMatrix44 aMatrixWorldCurrent[NUM_BLEND_MATRICES];
/*0x12b0*/ __declspec(align(16)) CMatrix44 aMatrixWorldInverse[NUM_BLEND_MATRICES];
/*0x14b0*/ __declspec(align(16)) CMatrix44 aMatrixWorldInverseTransposed[NUM_BLEND_MATRICES];
/*0x16b0*/ CMatrix44*                apMatrixWorld[NUM_BLEND_MATRICES];
/*0x16f0*/ CMatrix44                 matrixViewProj;
/*0x1730*/ CMatrix44                 matrixView;
/*0x1770*/ CMatrix44                 matrixProj;
/*0x17b0*/ IDirect3DIndexBuffer9*    pIndexBufferLitData;
/*0x17b8*/ CBufferSet*               apBufferSets[MAX_BUFFER_SET];
/*0x18d0*/ CEffect*                  apEffects[MAX_EFFECTS];
/*0x1dd8*/ CRenderEffect*            apRenderEffects[MAX_RENDER_EFFECTS];
/*0x24e0*/ IDirect3DVertexDeclaration9* apVertexDeclarations[MAX_VERTEX_DECLARATIONS];
/*0x2588*/ Direct3DTexture9*         pAttenuationLookupTexture;
/*0x2590*/ Direct3DTexture9*         pSpecularPower68LookupTexture;
/*0x2598*/ IDirect3DCubeTexture9*    pNormalizationLookupCubeTexture;
/*0x25a0*/ TNodePool<CBatchNode>*    pBatchNodePool;
/*0x25a8*/ TNodePool<CSubsetNode>*   pSubsetNodePool;
/*0x25b0*/ TNodePool<CLitBatchNode>* pLitBatchNodePool;
/*0x25b8*/ TNodePool<CTerrainBatchNode>* pTerrainBatchNodePool;
/*0x25c0*/ TList<CRenderNode>        renderNodeListSinglePass[MAX_EFFECT_PASS_COUNT];
/*0x2620*/ TList<CRenderNode>        renderNodeListZPass[MAX_EFFECT_PASS_COUNT];
/*0x2680*/ TList<CRenderNode>        renderNodeListTexturePass[MAX_EFFECT_PASS_COUNT];
/*0x26e0*/ TNodePool<CRenderNode>*   pRenderNodePool;
/*0x26e8*/ TList<CLightNode>         activeLightList;
/*0x26f8*/ TNodePool<CLightNode>*    pLightNodePool;
/*0x2700*/ int                       renderedTextureCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x3618*/ int                       renderedTerrainBatchCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x4530*/ int                       renderedTerrainBatchTriCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x5448*/ int                       renderedBatchCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x6360*/ int                       renderedBatchTriCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x7278*/ int                       renderedSubsetCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x8190*/ int                       renderedSubsetTriCount[MAX_EFFECT_PASS_COUNT][MAX_EFFECTS];
/*0x90a8*/ int                       nStatsType;
/*0x90ac*/ bool                      bShowItemPlacementStats;
/*0x90ad*/ SItemPlacementStatsDisplay itemPlacementStats;
/*0x98c8*/ float                     fColorIntensityLookup[256];
/*0x9cc8*/ float                     fLookup[2048];
/*0xbcc8*/ CVector3                  eyeOffset;
/*0xbcd4*/ uint32_t                  uRegionCount;
/*0xbcd8*/ bool                      bFogEnabled;
/*0xbcdc*/ RGB                       rgbFogColor;
/*0xbce0*/ float                     fFogStart;
/*0xbce4*/ float                     fFogEnd;
/*0xbce8*/ float                     fFogDensity;
/*0xbcf0*/ RenderCallbackPtr         pfnRenderCallback;
/*0xbcf8*/ bool                      bCensorship;
/*0xbd00*/ CMemoryPoolManager*       pLitTriangleMemoryPoolManager;
/*0xbd08*/ CMemoryPoolManager*       pLitBatchMemoryPoolManager;
/*0xbd10*/ uint8_t                   Unknown0xbd10[0x14];
/*0xbd24*/ bool                      bShadowsStencilAvailable;
/*0xbd25*/ bool                      bShadowsAvailable;
/*0xbd26*/ uint8_t                   Unknown0xbd26[0x122];
/*0xbe48*/ bool                      bShadowsAllowed;
/*0xbe49*/ uint8_t                   Unknown0xbe49[0x157];
/*0xbfa0*/ bool                      bMPLAvailable;
/*0xbfa1*/ bool                      bMPLUse;
/*0xbfa2*/ uint8_t                   Unknown0xbfa2;
/*0xbfa3*/ bool                      bPostEffectsAvailable;
/*0xbfa4*/ uint8_t                   Unknown0xbfa4[3];
/*0xbfa7*/ bool                      bLitBatchesUse;
/*0xbfa8*/ bool                      bLitBatchesEnabled;
/*0xbfa9*/ uint8_t                   Unknown0xbfa9[0x12f];
/*0xc0d8*/ CRenderStateCache         stateCache;
/*0xf4c0*/ uint8_t                   Unknown0xf4c0[0x20];
/*0xf4e0*/ void*                     apObjectPreview[2];
/*0xf4f0*/ uint8_t                   Unknown0xf4f0[0x8];
/*0xf4f8*/ uint8_t                   environmentFabricShaderParams[0x34];
/*0xf52c*/ uint8_t                   environmentEmissiveShaderParams[0x454];
/*0xf980*/ uint8_t                   environmentGodRaysShaderParams[0x44];
/*0xf9c4*/ uint8_t                   environmentOITShaderParams[0x8];
/*0xf9cc*/ uint8_t                   Unknown0xf9cc[0x24];
/*0xf9f0*/
};

SIZE_CHECK(CRender, CRender_size);

//============================================================================

} // namespace eqlib
