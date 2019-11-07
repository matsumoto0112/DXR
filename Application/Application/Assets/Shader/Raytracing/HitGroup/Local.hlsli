#ifndef SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI
#define SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI

#define HLSL
#include "../Util/HitGroupCompat.h"

ConstantBuffer<HitGroupConstant> l_sceneCB : register(b1);

#endif //! SHADER_RAYTRACING_HITGROUP_LOCAL_HLSLI