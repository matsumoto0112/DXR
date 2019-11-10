#ifndef SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
#define SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI

#define HLSL
#include "../Util/HitGroupCompat.h"

ConstantBuffer<HitGroupConstant> l_sceneCB : register(b1);

Texture2D albedo : register(t3);
Texture2D tex1 : register(t4);

#endif //! SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI