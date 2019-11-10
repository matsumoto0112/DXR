#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"

inline float3 Normal(in MyAttr attr) {
    float2 uv = GetUV(attr);

    float3 vertexNormal = normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld4x3()));
    float4 tangent = GetTangent(attr);
    float3 binormal = normalize(cross(vertexNormal, normalize(tangent.xyz)) * tangent.w);

    //法線マップから取得した法線
    float3 normalMap = normalize(normal.SampleLevel(samLinear, uv, 0.0).rgb);

    float3 N = normalMap.x * tangent.xyz * tangent.w + normalMap.y * binormal + normalMap.z * vertexNormal;

    N = normalize(N);

    N = vertexNormal;

    return N;
}

[shader("closesthit")]
void ClosestHit_Normal(inout RayPayload payload, in MyAttr attr) {
    float3 hitPosition = hitWorldPosition();
    float3 L = normalize(g_sceneCB.lightPosition.xyz - hitPosition);

    Ray shadowRay = { hitPosition,L };
    float factor = ShadowCast(shadowRay, payload.hitNum) ? 0.1 : 1.0;
    float2 uv = GetUV(attr);

    //float3 N = GetNormal(attr);
    float3 N = Normal(attr);
    float3 diffuse = albedo.SampleLevel(samLinear, uv, 0.0).rgb;

    float dotNL = saturate(dot(N, L));

    float4 color = g_sceneCB.lightAmbient;
    color.rgb += (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;
    color.rgb += float3(1, 1, 1) * pow(dotNL, 30);
    //color.rgb += emissive.SampleLevel(samLinear, uv, 0.0).rgb;

    //color.rgb *= factor;
    color = saturate(color);

    payload.color = color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL