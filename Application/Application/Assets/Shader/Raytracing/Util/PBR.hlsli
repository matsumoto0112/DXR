#ifndef SHADER_RAYTRACING_UTIL_HLSLI
#define SHADER_RAYTRACING_UTIL_HLSLI

#include "Helper.hlsli"

static inline float3 DiffuseBRDF(in float3 color, in float3 N, in float3 L) {
    const float dotNL = saturate(dot(N, L));
    return (color * dotNL) / PI;
}

#endif //! SHADER_RAYTRACING_UTIL_HLSLI
