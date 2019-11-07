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
    uint baseIndex = PrimitiveIndex() * triangleIndexStride /*+ l_material.indexOffset*/;

    return loadIndices(baseIndex, Indices)/* + l_material.vertexOffset*/;
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
    float2 uvs[3]=
    {
        Vertices[indices[0]].uv,
        Vertices[indices[1]].uv,
        Vertices[indices[2]].uv,
    };
    return getUV(uvs, attr);
}

[shader("closesthit")]
void Normal(inout RayPayload payload, in MyAttr attr) {
    //float3 N = getNormal(getIndices(), attr);
    //N = mul((float3x3)ObjectToWorld3x4(), N);
    //N = N * 0.5 + 0.5;
    //payload.color = float4(N, 1.0);

    //float2 uv = getUV(getIndices(), attr);
    //payload.color = float4(uv, 0, 1);

    payload.color = l_sceneCB.color;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL