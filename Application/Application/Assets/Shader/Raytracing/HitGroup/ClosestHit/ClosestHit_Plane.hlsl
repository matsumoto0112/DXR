#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"
#include "../Util/PBR.hlsli"

[shader("closesthit")] void ClosestHit_Plane(inout RayPayload payload, in MyAttr attr) {
    //パラメータを取得する
    float3 hitPosition = hitWorldPosition();
    float3 currentRayDirection = WorldRayDirection();
    float3 N = GetNormal(attr);
    float3 L = normalize(g_sceneCB.lightPosition.xyz);
    float2 uv = GetUV(attr);
    float3 V = normalize(hitPosition - g_sceneCB.cameraPosition.xyz);
    float3 albedoColor = SampleTexture(albedoTex, samLinear, uv).rgb;
    float2 metallicRoughness = SampleTexture(metallicRoughnessMap, samLinear, uv).rg;

    //影にかかっているか判定
    Ray shadowRay = { hitPosition, L };
    float factor = ShadowRayCast(shadowRay, payload.recursionCount) ? 0.5 : 1.0;

    //二次レイキャスト
    Ray secondRay;
    secondRay.origin = hitPosition;
    secondRay.direction = reflect(currentRayDirection, N);

    LightingInfo info;
    info.N = N;
    info.L = L;
    info.V = V;
    info.lightColor = g_sceneCB.lightDiffuse.rgb;
    info.albedo = albedoColor;
    info.metallic = metallicRoughness.r;
    info.roughness = metallicRoughness.g;

    float3 color = Lighting(info);

    //反射色の取得
    float3 reflectColor = RayCast(secondRay, payload.recursionCount).rgb;
    color.rgb += reflectColor * 0.5;

    color.rgb *= factor;
    color = saturate(color);

    payload.color = float4(color, 1.0);
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL
