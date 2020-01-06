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
#include <atlbase.h>
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <initguid.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Libs/d3dx12.h"

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "Utility/Debug.h"

#include "Math/MathUtility.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/VectorUtil.h"
#include "Utility/Color4.h"
#include "Utility/STLExtend.h"

#include "Typedef.h"
