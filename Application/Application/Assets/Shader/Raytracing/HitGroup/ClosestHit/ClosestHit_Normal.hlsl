#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"

inline float3 Normal(in MyAttr attr) {
    float2 uv = GetUV(attr);
    float3 worldNormal = normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld4x3()));
    float4 tangent4 = GetTangent(attr);
    float3 tangent = normalize(mul(tangent4.xyz, (float3x3)ObjectToWorld4x3())) * tangent4.w;

    float3 binormal = cross(worldNormal, tangent);

    float3 normalMap = SampleTexture(normal, samLinear, uv).rgb;

    float3 N = normalMap.x * tangent.xyz + normalMap.y * binormal.xyz + normalMap.z * worldNormal.xyz;
    return N;
}

[shader("closesthit")]
void ClosestHit_Normal(inout RayPayload payload, in MyAttr attr) {
    float3 hitPosition = hitWorldPosition();

    float2 uv = GetUV(attr);
    float3 N = normalize(Normal(attr));

    float4 color = float4(0, 0, 0, 0);
    float3 L = normalize(g_sceneCB.lightPosition.xyz - hitPosition);
    float dotNL = saturate(dot(N, L));
    float4 diffuse = SampleTexture(albedo, samLinear, uv) * dotNL;

    color += diffuse;

     color.a = 1.0;
    payload.color = color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL