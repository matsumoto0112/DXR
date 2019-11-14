#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../Helper.hlsli"
#include "../Local.hlsli"

inline float3 Normal(in MyAttr attr) {
    //float2 uv = GetUV(attr);

    //float3 vertexNormal = normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld3x4()));
    //float4 tangent = GetTangent(attr);
    //float3 binormal = normalize(cross(vertexNormal, normalize(tangent.xyz)) * tangent.w);

    //float3 normalMap = normalize(SampleTexture(normal, samLinear, uv).rgb);

    //float3 N = normalMap.x * tangent.xyz * tangent.w + normalMap.y * binormal + normalMap.z * vertexNormal;

    //N = normalize(N);

    //N = vertexNormal;
    float2 uv = GetUV(attr);

    //float3 normal = mul(normalize(GetNormal(attr)), (float3x3)ObjectToWorld4x3());

    //float3 normal = mul(GetNormal(attr), (float3x3)ObjectToWorld3x4());
    float3 normal = normalize(mul(GetNormal(attr), (float3x3)ObjectToWorld4x3()));
    float4 tangent4 = GetTangent(attr);
    float3 tangent = normalize(mul(tangent4.xyz, (float3x3)ObjectToWorld4x3())) * tangent4.w;

    //float3 normalMap = SampleTexture(normal, samLinear, uv).rgb;

    float3 N = tangent;

    return N;
}

[shader("closesthit")]
void ClosestHit_Normal(inout RayPayload payload, in MyAttr attr) {
    //float3 hitPosition = hitWorldPosition();
    //float3 L = normalize(g_sceneCB.lightPosition.xyz - hitPosition);

    //Ray shadowRay = { hitPosition,L };
    //float factor = ShadowRayCast(shadowRay, payload.recursionCount) ? 0.1 : 1.0;
    //float2 uv = GetUV(attr);

    //float3 N = Normal(attr);
    //float3 diffuse = SampleTexture(albedo, samLinear, uv).rgb;

    //float dotNL = saturate(dot(N, L));

    //float4 color = g_sceneCB.lightAmbient;
    //color.rgb += (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;
    //color.rgb += float3(1, 1, 1) * pow(dotNL, 30);
    //color.rgb += SampleTexture(emissive, samLinear, uv).rgb;

    //color.rgb *= factor;
    //color = saturate(color);

    //payload.color = color;

    //float2 uv = GetUV(attr);
    float3 N = Normal(attr);

    //float4 color = float4(0, 0, 0, 0);
    ////float4 diffuse = SampleTexture(albedo, samLinear, uv);

    ////color += diffuse;
    //N = N * 0.5 + 0.5;
    //color.rgb = N;

    //color.a = 1.0;
    //payload.color = color
    float4 col = float4(N, 1.0);
    col = col * 0.5 + 0.5;
    payload.color = col;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_CLOSESTHIT_CLOSESTHIT_NORMAL_HLSL