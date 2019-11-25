#ifndef SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
#define SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI

#define HLSL
#include "../Util/HitGroupCompat.h"

ConstantBuffer<HitGroupConstant> l_sceneCB : register(b1);

Texture2D albedoTex : register(t3);
Texture2D normalMap : register(t4);
Texture2D metallicRoughnessMap : register(t5);
Texture2D emissiveMap : register(t6);
Texture2D occulusionMap : register(t7);

#endif //! SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
