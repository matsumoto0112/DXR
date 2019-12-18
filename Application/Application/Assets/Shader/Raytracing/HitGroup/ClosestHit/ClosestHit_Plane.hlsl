#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"

[shader("closesthit")] void ClosestHit_Plane(inout RayPayload payload, in MyAttr attr) {
    float3 hitPosition = hitWorldPosition();
    float3 currentRayDirection = WorldRayDirection();
    float3 N = GetNormal(attr);
    float3 L = normalize(g_sceneCB.lightPosition.xyz);

    //�e�ɂ������Ă��邩����
    Ray shadowRay = { hitPosition, L };
    float factor = ShadowRayCast(shadowRay, payload.recursionCount) ? 0.5 : 1.0;

    float2 uv = GetUV(attr);

    //�A���r�G���g
    float4 color = g_sceneCB.lightAmbient;

    //�f�B�t���[�Y
    float3 diffuse = SampleTexture(albedoTex, samLinear, uv).rgb;
    float dotNL = saturate(dot(N, L));
    color.rgb += (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;

    //�񎟃��C�L���X�g
    Ray secondRay;
    secondRay.origin = hitPosition;
    secondRay.direction = reflect(currentRayDirection, N);

    //���ːF�̎擾
    float3 reflectColor = RayCast(secondRay, payload.recursionCount).rgb;
    color.rgb += reflectColor * 0.5;

    color.rgb *= factor;
    color = saturate(color);

    payload.color = color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_PLANE_HLSL
