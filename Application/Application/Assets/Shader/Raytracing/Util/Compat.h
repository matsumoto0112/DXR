#ifndef SHADER_RAYTRACING_COMPAT_H
#define SHADER_RAYTRACING_COMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
using namespace DirectX;
typedef UINT16 Index;
#endif

struct SceneConstantBuffer {
    XMFLOAT4 cameraPosition;
    XMMATRIX projectionToWorld;
};

struct Vertex {
    XMFLOAT3 position;
};

struct RayPayload {
    XMFLOAT4 color;
};

#endif // !SHADER_RAYTRACING_COMPAT_H
