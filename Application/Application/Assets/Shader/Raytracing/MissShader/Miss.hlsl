#ifndef SHADER_RAYTRACING_MISSSHADER_MISS_HLSL
#define SHADER_RAYTRACING_MISSSHADER_MISS_HLSL

#define HLSL
#include "../Util/Global.hlsli"
#include "Local.hlsli"

[shader("miss")] void Miss(inout RayPayload payload) {
    //payload.color = l_missCB.back;
    payload.color = float4(0, 0, 0, 1);
}

#endif //! SHADER_RAYTRACING_MISSSHADER_MISS_HLSL
