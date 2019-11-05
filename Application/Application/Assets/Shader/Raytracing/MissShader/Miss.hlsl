#ifndef SHADER_RAYTRACING_MISSSHADER_MISS_HLSL
#define SHADER_RAYTRACING_MISSSHADER_MISS_HLSL

#define HLSL
#include "../Util/Global.hlsli"

[shader("miss")]
void Miss(inout RayPayload payload) {
    payload.color = float4(1, 1, 1, 1);
}

#endif //! SHADER_RAYTRACING_MISSSHADER_MISS_HLSL