#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../../Util/Global.hlsli"
#include "../../Util/Helper.hlsli"
#include "../Local.hlsli"

static inline uint3 getIndices() {
    uint indexSizeInBytes = 2;
    uint indicesPerTriangle = 3;
    uint triangleIndexStride = indicesPerTriangle * indexSizeInBytes;
    uint baseIndex = PrimitiveIndex() * triangleIndexStride + l_sceneCB.indexOffset * indexSizeInBytes;

    return loadIndices(baseIndex, Indices) + l_sceneCB.vertexOffset;
}

static inline float3 getNormal(in uint3 indices, in MyAttr attr) {
    //ñ@ê¸ÇÃéÊìæ
    float3 normals[3] =
    {
        Vertices[indices[0]].normal,
        Vertices[indices[1]].normal,
        Vertices[indices[2]].normal,
    };

    return getNormal(normals, attr);
}

static inline float2 getUV(in uint3 indices, in MyAttr attr) {
    float2 uvs[3] =
    {
        Vertices[indices[0]].uv,
        Vertices[indices[1]].uv,
        Vertices[indices[2]].uv,
    };
    return getUV(uvs, attr);
}

[shader("closesthit")]
void Normal(inout RayPayload payload, in MyAttr attr) {
    ShadowPayload shadow = { true };
    float3 hitPosition = hitWorldPosition();
    float3 L = normalize(g_sceneCB.lightPosition.xyz - hitPosition);
    RayDesc ray;
    ray.Origin = hitPosition;
    ray.Direction = L;
    ray.TMin = 0.01;
    ray.TMax = 10000.0;
    TraceRay(
        g_scene,
        RAY_FLAG_SKIP_CLOSEST_HIT_SHADER,
        ~0,
        0,
        0,
        1,
        ray,
        shadow);

    float2 uv = getUV(getIndices(), attr);
    float3 diffuse = tex0.SampleLevel(samLinear, uv, 0.0).rgb;

    float3 N = getNormal(getIndices(), attr);
    float dotNL = saturate(dot(N, L));
    float3 color = (diffuse * g_sceneCB.lightDiffuse.rgb) * dotNL / PI;
    float factor = shadow.hit ? 0.1 : 1.0;
    color *= factor;
    //color += g_sceneCB.lightAmbient.rgb;

    color = saturate(color);
    payload.color.rgb = color;
    payload.color.a = 1.0;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL