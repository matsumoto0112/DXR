#pragma once
#include "targetvar.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Libs/d3dx12.h"

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <GLTFSDK/GLTF.h>

#include "Math/MathUtility.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/VectorUtil.h"
#include "Utility/Color4.h"

#include "ImGui/Item/ImGUI.h"

#include "Typedef.h"
