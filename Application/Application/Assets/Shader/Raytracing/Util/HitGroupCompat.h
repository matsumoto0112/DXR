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
    //最初にテクスチャがないとテクスチャが使えないっぽい
//#ifndef HLSL
//    D3D12_GPU_DESCRIPTOR_HANDLE tex0;
//    D3D12_GPU_DESCRIPTOR_HANDLE tex1;
//#endif

    Color color;
    UINT vertexOffset;
    UINT indexOffset;

};

#endif // !SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
