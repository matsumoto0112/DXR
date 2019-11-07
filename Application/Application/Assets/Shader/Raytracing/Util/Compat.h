#ifndef SHADER_RAYTRACING_COMPAT_H
#define SHADER_RAYTRACING_COMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Application/Source/Typedef.h"
using namespace DirectX;
typedef UINT16 Index;
#endif

static const UINT MAX_RECURSION_NUM = 2;

struct SceneConstantBuffer {
    Mat4 projectionToWorld;
    XMFLOAT4 cameraPosition;
};

struct Vertex {
    Vec3 position;
};

struct RayPayload {
    Color color;
};

struct MissConstant {
    Color back;
};

#endif // !SHADER_RAYTRACING_COMPAT_H
