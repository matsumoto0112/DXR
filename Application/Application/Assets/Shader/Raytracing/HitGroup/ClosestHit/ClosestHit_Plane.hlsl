#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"

[shader("closesthit")]
void ClosestHit_Plane(inout RayPayload payload, in MyAttr attr) {
    float3 hitPosition = hitWorldPosition();
    float3 currentRayDirection = WorldRayDirection();
    float3 N = GetNormal(attr);
    float3 L = normalize(g_sceneCB.lightPosition.xyz - hitPosition);

    //影にかかっているか判定
    Ray shadowRay = { hitPosition,L };
    float factor = ShadowRayCast(shadowRay,payload.recursionCount) ? 0.1 : 1.0;

    float2 uv = GetUV(attr);

    //アンビエント
    float4 color = g_sceneCB.lightAmbient;

    //ディフューズ
    float3 diffuse = SampleTexture(albedo, samLinear, uv).rgb;
    float dotNL = saturate(dot(N, L));
    color.rgb += (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;

    //二次レイキャスト
    Ray secondRay;
    secondRay.origin = hitPosition;
    secondRay.direction = reflect(currentRayDirection, N);

    //反射色の取得
    float3 reflectColor = RayCast(secondRay, payload.recursionCount).rgb;
    color.rgb += reflectColor;

    color.rgb *= factor;
    color = saturate(color);

    color = SampleTexture(metalRough, samLinear, uv);

    payload.color = color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL