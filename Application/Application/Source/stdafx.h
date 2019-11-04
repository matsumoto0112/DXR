/**
* @file stdafx.h
* @brief プリコンパイル済みヘッダー
*/

#pragma once
#include "targetvar.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NOMINMAX
#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Application/Libs/d3dx12.h"


#include <GLTFSDK/GLTF.h>

#include "Framework/Math/MathUtility.h"
#include "Framework/Math/Matrix4x4.h"
#include "Framework/Math/Vector2.h"
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Vector4.h"
#include "Framework/Math/VectorUtil.h"
