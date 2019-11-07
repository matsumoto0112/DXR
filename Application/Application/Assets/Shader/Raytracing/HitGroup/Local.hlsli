#ifndef SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
#define SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI

#define HLSL
#include "../Util/HitGroupCompat.h"

//ConstantBuffer<HitGroupConstant> l_sceneCB : register(b1);

Texture2D tex0 : register(t3);

#endif //! SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI