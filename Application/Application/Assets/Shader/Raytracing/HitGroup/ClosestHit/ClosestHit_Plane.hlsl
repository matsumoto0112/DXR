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

    Ray shadowRay = { hitPosition,L };
    float factor = ShadowCast(shadowRay,payload.hitNum) ? 0.1 : 1.0;

    float2 uv = GetUV(attr);

    float3 diffuse = albedo.SampleLevel(samLinear, uv, 0.0).rgb;
    float dotNL = saturate(dot(N, L));

    float4 color = g_sceneCB.lightAmbient;
    color.rgb += (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;

    Ray secondRay;
    secondRay.origin = hitPosition;
    secondRay.direction = reflect(currentRayDirection, N);
    float3 reflectColor = traceRadianceRay(secondRay, payload.hitNum).rgb;
    color.rgb += reflectColor;

    color.rgb *= factor;
    color = saturate(color);

    payload.color = color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL