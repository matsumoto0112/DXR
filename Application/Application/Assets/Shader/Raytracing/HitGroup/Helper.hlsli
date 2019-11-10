#ifndef SHADER_RAYTRACING_HITGROUP_HELPER_HLSLI
#define SHADER_RAYTRACING_HITGROUP_HELPER_HLSLI

#define HLSL
#include "../Util/GlobalCompat.h"
#include "../Util/Global.hlsli"
#include "../Util/Helper.hlsli"
#include "Local.hlsli"

inline bool ShadowCast(in Ray ray,in uint currentRecursionNum) {
    //ç≈ëÂçƒãAâÒêîÇí¥Ç¶ÇƒÇ¢ÇΩÇÁâΩÇ‡ÇµÇ»Ç¢
    if (currentRecursionNum >= MAX_RAY_RECURSION_DEPTH) {
        return false;
    }

    ShadowPayload shadow = { true };
    float3 hitPosition = hitWorldPosition();
    float3 L = normalize(g_sceneCB.lightPosition.xyz - hitPosition);
    RayDesc rayDesc;
    rayDesc.Origin = ray.origin;
    rayDesc.Direction = ray.direction;
    rayDesc.TMin = 0.01;
    rayDesc.TMax = 10000.0;
    TraceRay(
        g_scene,
        RAY_FLAG_SKIP_CLOSEST_HIT_SHADER,
        ~0,
        0,
        0,
        1,
        rayDesc,
        shadow);
    return shadow.hit;
}

inline uint3 GetIndices() {
    uint indexSizeInBytes = 2;
    uint indicesPerTriangle = 3;
    uint triangleIndexStride = indicesPerTriangle * indexSizeInBytes;
    uint baseIndex = PrimitiveIndex() * triangleIndexStride + l_sceneCB.indexOffset * indexSizeInBytes;

    return loadIndices(baseIndex, Indices) + l_sceneCB.vertexOffset;
}

inline float2 GetUV(in MyAttr attr) {
    uint3 indices = GetIndices();

    float2 uvs[3] =
    {
        Vertices[indices[0]].uv,
        Vertices[indices[1]].uv,
        Vertices[indices[2]].uv,
    };
    return getUV(uvs, attr);
}

inline float3 GetNormal(in MyAttr attr) {
    uint3 indices = GetIndices();

    //ñ@ê¸ÇÃéÊìæ
    float3 normals[3] =
    {
        Vertices[indices[0]].normal,
        Vertices[indices[1]].normal,
        Vertices[indices[2]].normal,
    };

    return getNormal(normals, attr);
}

inline float4 GetTangent(in MyAttr attr) {
    uint3 indices = GetIndices();

    //ñ@ê¸ÇÃéÊìæ
    float4 tangents[3] =
    {
        Vertices[indices[0]].tangent,
        Vertices[indices[1]].tangent,
        Vertices[indices[2]].tangent,
    };

    return getTangent(tangents, attr);
}


#endif //! SHADER_RAYTRACING_HITGROUP_HELPER_HLSLI