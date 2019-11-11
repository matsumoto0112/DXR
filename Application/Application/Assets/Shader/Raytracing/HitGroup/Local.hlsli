#ifndef SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
#define SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI

#define HLSL
#include "../Util/HitGroupCompat.h"

ConstantBuffer<HitGroupConstant> l_sceneCB : register(b1);

Texture2D albedo : register(t3);
Texture2D normal : register(t4);
Texture2D metalRough : register(t5);
Texture2D emissive : register(t6);

#endif //! SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI