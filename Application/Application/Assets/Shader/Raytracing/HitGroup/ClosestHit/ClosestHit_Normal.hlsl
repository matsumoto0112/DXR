#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"
#include "../Util/PBR.hlsli"

inline float3 Normal(in MyAttr attr) {
    float2 uv = GetUV(attr);
    float3 worldNormal = normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld4x3()));
    float4 tangent4 = GetTangent(attr);
    float3 tangent = normalize(mul(tangent4.xyz, (float3x3)ObjectToWorld4x3())) * tangent4.w;

    float3 binormal = normalize(cross(worldNormal, tangent));

    float3 normal = SampleTexture(normalMap, samLinear, uv).rgb;

    float3 N = normal.x * tangent.xyz + normal.y * binormal.xyz + normal.z * worldNormal.xyz;

    return N;
}

[shader("closesthit")] void ClosestHit_Normal(inout RayPayload payload, in MyAttr attr) {
    float3 hitPosition = hitWorldPosition();
    float2 uv = GetUV(attr);
    float3 N = normalize(Normal(attr));
    float3 L = normalize(g_sceneCB.lightPosition.xyz);
    float3 V = normalize(hitPosition - g_sceneCB.cameraPosition.xyz);

    float2 metallicRoughness = SampleTexture(metallicRoughnessMap, samLinear, uv).rg;
    float3 albedoColor = SampleTexture(albedoTex, samLinear, uv).rgb;

    LightingInfo info;
    info.N = N;
    info.L = L;
    info.V = V;
    info.lightColor = g_sceneCB.lightDiffuse.rgb;
    info.albedo = albedoColor;
    info.metallic = metallicRoughness.r;
    info.roughness = metallicRoughness.g;

    float3 color = Lighting(info);

    Ray shadowRay;
    shadowRay.origin = hitPosition;
    shadowRay.direction = L;
    float factor = ShadowRayCast(shadowRay, payload.recursionCount) ? 0.1 : 1.0;

    payload.color.rgb = color * factor;
    payload.color.a = 1.0;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL
