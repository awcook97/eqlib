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

#include "eqlib/SizeChecks.h"

#include "eqlib/game/MathTypes.h"
#include "eqlib/graphics/GraphicsEngine.h"

namespace eqlib {

//============================================================================

//----------------------------------------------------------------------------
class CMathInterface
{
public:
/*0x00*/ virtual ~CMathInterface() {}
/*0x08*/ virtual float Sin(float angle512) = 0;
/*0x10*/ virtual float Cos(float angle512) = 0;
/*0x18*/ virtual float FastSin(float angle512) = 0;
/*0x20*/ virtual float FastCos(float angle512) = 0;
/*0x28*/ virtual float Sqrt(float) = 0;
/*0x30*/ virtual float InvSqrt(float) = 0;
/*0x38*/ virtual float AngleArcTan(float y, float x, int* exact) = 0;
/*0x40*/ virtual void CalculateRotationCoefficients(float, float, float, float* matrix3x3) = 0;
};

struct [[offsetcomments]] SArcTanNode
{
/*0x00*/ float                   tangent;
/*0x04*/ float                   cotangent;
/*0x08*/ SArcTanNode*            pLess;
/*0x10*/ SArcTanNode*            pGreater;
/*0x18*/
};

constexpr size_t CMath_size = 0x1C08; // @sizeof(CMath) :: 2026-09-17 (live) @ 0x180019F05

class [[offsetcomments]] CMath : public CMathInterface
{
public:
/*0x0008*/ float                   cosTable[512];
/*0x0808*/ float                   sinTable[512];
/*0x1008*/ SArcTanNode             arcTanNodes[128];
/*0x1c08*/

	SArcTanNode* GetArcTanRoot() { return &arcTanNodes[64]; }
};

SIZE_CHECK(CMath, CMath_size);

//============================================================================

} // namespace eqlib

