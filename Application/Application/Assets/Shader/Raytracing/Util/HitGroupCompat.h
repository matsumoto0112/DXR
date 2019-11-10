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

/**
* @brief ヒットグループのコンスタントバッファ
*/
struct HitGroupConstant {
    UINT vertexOffset; //!< 頂点オフセット
    UINT indexOffset; //!< インデックスオフセット
};

#endif // !SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
