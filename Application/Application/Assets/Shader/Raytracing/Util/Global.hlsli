#ifndef SHADER_RAYTRACING_GLOBAL_HLSLI
#define SHADER_RAYTRACING_GLOBAL_HLSLI

#define HLSL
#include "Compat.h"

//Top-Level-AS構造体
RaytracingAccelerationStructure g_scene : register(t0, space0);
//レンダーターゲット
RWTexture2D<float4> g_renderTarget : register(u0);
//シーン情報
ConstantBuffer<SceneConstantBuffer> g_sceneCB : register(b0);

typedef BuiltInTriangleIntersectionAttributes MyAttr;

#endif //! SHADER_RAYTRACING_GLOBAL_HLSLI