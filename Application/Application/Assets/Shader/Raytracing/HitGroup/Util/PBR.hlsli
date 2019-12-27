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

static float D_GGX(in float roughness, in float dotNH) {
    const float a2 = roughness * roughness;
    const float dotNH2 = dotNH * dotNH;
    const float d = (dotNH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * d * d);
}

static float G_smith(in float roughness, in float dotNV, in float dotNL) {
    const float k = roughness * 0.5 + EPSILON;
    const float l = dotNL / (dotNL * (1.0 - k) + k);
    const float v = dotNV / (dotNV * (1.0 - k) + k);
    return l * v;
}

static float3 F_schlick(in float3 F, in float dotVH) {
    return F + (1.0 - F) * pow(1 - dotVH, 5.0);
}

struct SpecularBRDFInfo {
    float3 color;
    float a;
    float dotNH;
    float dotNV;
    float dotNL;
    float dotVH;
};

static float3 SpecularBRDF(in SpecularBRDFInfo info) {
    const float d = D_GGX(info.a, info.dotNH);
    const float g = G_smith(info.a, info.dotNV, info.dotNL);
    const float3 f = F_schlick(info.color, info.dotVH);
    return (d * g * f) / (4.0 * info.dotNL * info.dotNV + EPSILON);
}

static float3 Lighting(in LightingInfo info) {
    const float3 N = normalize(info.N);
    const float3 L = normalize(info.L);
    const float3 V = normalize(info.V);
    const float3 H = normalize(L + V);

    const float dotNL = saturate(dot(N, L));
    const float dotNH = saturate(dot(N, H));
    const float dotNV = saturate(dot(N, V));
    const float dotVH = saturate(dot(V, H));

    const float3 irradiance = info.lightColor * dotNL * PI;
    float3 color = float3(0, 0, 0);
    float3 diffuseColor = lerp(info.albedo, float3(0, 0, 0), info.metallic);
    float3 specularColor = lerp(float3(0.04, 0.04, 0.04), info.albedo, info.metallic);
    color += irradiance * DiffuseBRDF(diffuseColor, dotNL);

    SpecularBRDFInfo specInfo
        = { specularColor, info.roughness * info.roughness, dotNH, dotNV, dotNL, dotVH };

    color += irradiance * SpecularBRDF(specInfo);

    color = saturate(color);
    return color;
}

#endif //! SHADER_RAYTRACING_UTIL_HLSLI
