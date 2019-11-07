#ifndef SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
#define SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI

#define HLSL
#include "../Util/HitGroupCompat.h"

ConstantBuffer<HitGroupConstant> l_sceneCB : register(b1);

#ifdef HLSL
Texture2D<float4> tex0 : register(t3, space0);
#endif

#endif //! SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI