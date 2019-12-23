#ifndef SHADER_RAYTRACING_UTIL_HLSLI
#define SHADER_RAYTRACING_UTIL_HLSLI

#include "../../Util/Helper.hlsli"

struct LightingInfo {
    float3 lightColor;
    float3 albedo;
    float3 N;
    float3 L;
    float3 V;
    float metallic;
    float roughness;
};

static inline float3 DiffuseBRDF(in float3 color, in float dotNL) {
    return color * dotNL / PI;
}

static float3 SpecularBRDF(in float3 color) {
    return color;
}

static float3 Lighting(in LightingInfo info) {
    const float3 N = normalize(info.N);
    const float3 L = normalize(info.L);
    const float3 V = normalize(info.V);
    const float3 H = normalize(L + V);

    const float dotNL = saturate(dot(N, L));
    const float dotNH = saturate(dot(N, H));

    const float3 irradiance = info.lightColor * dotNL * PI;

    float3 color = float3(0, 0, 0);
    float3 diffuseColor = lerp(info.albedo, float3(0, 0, 0), info.metallic);
    float3 specularColor = lerp(float3(0.04, 0.04, 0.04), info.albedo, info.metallic);
    color += irradiance * DiffuseBRDF(diffuseColor, dotNL);
    color += irradiance * SpecularBRDF(specularColor);

    color = saturate(color);
    return color;
}

#endif //! SHADER_RAYTRACING_UTIL_HLSLI
