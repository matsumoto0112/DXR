#ifndef SHADER_RAYTRACING_MISSSHADER_MISS_HLSL
#define SHADER_RAYTRACING_MISSSHADER_MISS_HLSL

#define HLSL
#include "../Util/Global.hlsli"
#include "Local.hlsli"

[shader("miss")]
void Shadow(inout ShadowPayload payload) {
    payload.hit = false;
}

#endif //! SHADER_RAYTRACING_MISSSHADER_MISS_HLSL