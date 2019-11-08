#ifndef SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
#define SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Application/Source/Typedef.h"
using namespace DirectX;
#endif

struct HitGroupConstant {
    Color color;
    UINT vertexOffset;
    UINT indexOffset;

//#ifndef HLSL
//    D3D12_GPU_DESCRIPTOR_HANDLE tex0;
//#endif
};

#endif // !SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
