#ifndef SHADER_RAYTRACING_MISS_LOCAL_HLSLI
#define SHADER_RAYTRACING_MISS_LOCAL_HLSLI

#define HLSL
#include "../Util/Compat.h"

ConstantBuffer<MissConstant> l_missCB : register(b1);

#endif //! SHADER_RAYTRACING_MISS_LOCAL_HLSLI