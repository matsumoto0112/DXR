#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../../Util/Global.hlsli"
#include "../../Util/Helper.hlsli"

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

[shader("closesthit")]
void Normal(inout RayPayload payload, in MyAttr attr) {
    float3 N = getNormal(getIndices(), attr);
    N = mul((float3x3)ObjectToWorld3x4(), N);
    N = N * 0.5 + 0.5;
    N = saturate(N);
    payload.color = float4(N, 1.0);
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL