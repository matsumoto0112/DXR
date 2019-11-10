#ifndef SHADER_RAYTRACING_HITGROUP_HELPER_HLSLI
#define SHADER_RAYTRACING_HITGROUP_HELPER_HLSLI

#define HLSL
#include "../Util/GlobalCompat.h"
#include "../Util/Global.hlsli"
#include "../Util/Helper.hlsli"
#include "Local.hlsli"

inline bool ShadowRayCast(in Ray ray,in uint currentRecursionNum) {
    //�ő�ċA�񐔂𒴂��Ă����牽�����Ȃ�
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
/**
* @brief �O�p�`�̃C���f�b�N�X���擾����
*/
inline uint3 GetIndices() {
    uint indexSizeInBytes = 2; //2Byte�̃C���f�b�N�X�𗘗p���Ă��邽��
    uint indicesPerTriangle = 3;
    uint triangleIndexStride = indicesPerTriangle * indexSizeInBytes;
    uint baseIndex = PrimitiveIndex() * triangleIndexStride + l_sceneCB.indexOffset * indexSizeInBytes;

    return LoadIndices(baseIndex, Indices) + l_sceneCB.vertexOffset;
}

/**
* @brief �Փ˓_�̖@�����擾����
*/
inline float3 GetNormal(in MyAttr attr) {
    uint3 indices = GetIndices();

    float3 normals[3] =
    {
        Vertices[indices[0]].normal,
        Vertices[indices[1]].normal,
        Vertices[indices[2]].normal,
    };

    return normals[0] +
        attr.barycentrics.x * (normals[1] - normals[0]) +
        attr.barycentrics.y * (normals[2] - normals[0]);
}

/**
* @brief �Փ˓_��UV���W���擾����
*/
inline float2 GetUV(in MyAttr attr) {
    uint3 indices = GetIndices();

    float2 uvs[3] =
    {
        Vertices[indices[0]].uv,
        Vertices[indices[1]].uv,
        Vertices[indices[2]].uv,
    };
    return uvs[0] +
        attr.barycentrics.x * (uvs[1] - uvs[0]) +
        attr.barycentrics.y * (uvs[2] - uvs[0]);
}

/**
* @brief �Փ˓_�̐ڐ����擾����
*/
inline float4 GetTangent(in MyAttr attr) {
    uint3 indices = GetIndices();

    float4 tangents[3] =
    {
        Vertices[indices[0]].tangent,
        Vertices[indices[1]].tangent,
        Vertices[indices[2]].tangent,
    };

    return tangents[0] +
        attr.barycentrics.x * (tangents[1] - tangents[0]) +
        attr.barycentrics.y * (tangents[2] - tangents[0]);
}

#endif //! SHADER_RAYTRACING_HITGROUP_HELPER_HLSLI