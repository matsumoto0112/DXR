#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"

inline float3 Normal(in MyAttr attr) {
    float2 uv = GetUV(attr);

    float3 vertexNormal = normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld3x4()));
    float4 tangent = GetTangent(attr);
    float3 binormal = normalize(cross(vertexNormal, normalize(tangent.xyz)) * tangent.w);

    float3 normalMap = normalize(SampleTexture(normal, samLinear, uv).rgb);

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
    float factor = ShadowRayCast(shadowRay, payload.recursionCount) ? 0.1 : 1.0;
    float2 uv = GetUV(attr);

    float3 N = Normal(attr);
    float3 diffuse = SampleTexture(albedo, samLinear, uv).rgb;

    float dotNL = saturate(dot(N, L));

    float4 color = g_sceneCB.lightAmbient;
    color.rgb += (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;
    color.rgb += float3(1, 1, 1) * pow(dotNL, 30);
    color.rgb += SampleTexture(emissive, samLinear, uv).rgb;

    color.rgb *= factor;
    color = saturate(color);

    payload.color = color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL