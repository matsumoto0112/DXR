#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_SPHERE_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_SPHERE_HLSL

#define HLSL
#include "../../Util/PBR.hlsli"
#include "../Helper.hlsli"
#include "../Local.hlsli"

inline float3 Normal(in MyAttr attr) {
    return normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld4x3()));
}

[shader("closesthit")] void ClosestHit_Sphere(inout RayPayload payload, in MyAttr attr) {
    float3 hitPosition = hitWorldPosition();
    float2 uv = GetUV(attr);
    float3 N = normalize(Normal(attr));
    float3 L = normalize(g_sceneCB.lightPosition.xyz);

    float2 metallicRoughness = SampleTexture(metallicRoughnessMap, samLinear, uv).rg;
    float3 albedoColor = SampleTexture(albedoTex, samLinear, uv).rgb;

    float dotNL = saturate(dot(N, L));

    float3 irradiance = dotNL * g_sceneCB.lightDiffuse.rgb;
    irradiance *= PI;

    float3 diffuseColor = lerp(albedoColor, float3(0, 0, 0), metallicRoughness.r);
    float3 color = float3(0, 0, 0);
    color += irradiance * diffuseColor / PI;

    float3 currentRayDirection = WorldRayDirection();
    //�񎟃��C�L���X�g
    Ray secondRay;
    secondRay.origin = hitPosition;
    secondRay.direction = reflect(currentRayDirection, N);

    //���ːF�̎擾
    float3 reflectColor = RayCast(secondRay, payload.recursionCount).rgb;
    color.rgb += reflectColor;

    payload.color.rgb = color;
    payload.color.a = 1.0;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_SPHERE_HLSL
