/**
* @file Helper.hlsli
* @brief �w���p�[�֐��A�\���̒�`
*/

#ifndef SHADER_RAYTRACING_UTIL_HELPER_HLSLI
#define SHADER_RAYTRACING_UTIL_HELPER_HLSLI

#define HLSL
#include "GlobalCompat.h"
#include "Global.hlsli"

static const float PI = 3.141592654f;

/**
* @brief ���C
*/
struct Ray {
    float3 origin; //!< ���_
    float3 direction; //!< ����
};

/**
* @brief �J��������̃��C���΂�
*/
inline Ray GenerateCameraRay(in uint2 index, in float3 cameraPosition, in float4x4 projectionToWorld) {
    //�X�N���[�����W���v�Z����
    float2 xy = index + float2(0.5, 0.5);
    float2 screenPos = xy / DispatchRaysDimensions().xy * 2.0 - 1.0;
    screenPos.y = -screenPos.y;

    //���[���h���W�ɖ߂�
    float4 world = mul(float4(screenPos, 0.0, 1.0), projectionToWorld);
    world.xyz /= world.w;

    Ray ray;
    ray.origin = cameraPosition;
    ray.direction = normalize(world.xyz - cameraPosition);
    return ray;
}

/**
* @brief �C���f�b�N�X��ǂݍ���
* @details �C���f�b�N�X��2�o�C�g�œo�^����Ă��邪�V�F�[�_�[���ł�4�o�C�g�P�ʂł����ǂݍ��߂Ȃ�����
*/
static inline uint3 LoadIndices(uint offsetBytes, ByteAddressBuffer Indices) {
    uint3 indices;

    const uint alignedOffset = offsetBytes & ~3;
    const uint2 four16BitIndices = Indices.Load2(alignedOffset);

    if (alignedOffset == offsetBytes) {
        indices.x = four16BitIndices.x & 0xffff;
        indices.y = (four16BitIndices.x >> 16) & 0xffff;
        indices.z = four16BitIndices.y & 0xffff;
    }
    else {
        indices.x = (four16BitIndices.x >> 16) & 0xffff;
        indices.y = four16BitIndices.y & 0xffff;
        indices.z = (four16BitIndices.y >> 16) & 0xffff;
    }
    return indices;
}



/**
* @brief �Փ˓_�̃��[���h���W���擾����
*/
inline float3 hitWorldPosition() {
    return WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
}

/**
* @brief ���C���΂��A���������I�u�W�F�N�g�̐F���擾����
*/
inline float4 RayCast(in Ray ray, in uint currentRecursionNum) {
    //�ċA�񐔐���
    if (currentRecursionNum >= MAX_RAY_RECURSION_DEPTH) {
        return float4(0, 0, 0, 0);
    }

    RayDesc rayDesc;
    rayDesc.Origin = ray.origin;
    rayDesc.Direction = ray.direction;
    rayDesc.TMin = 0.01;
    rayDesc.TMax = 10000.0;

    RayPayload payload = { float4(0,0,0,0),currentRecursionNum + 1 };

    TraceRay(
        g_scene,
        RAY_FLAG_CULL_BACK_FACING_TRIANGLES,
        ~0,
        0,
        1,
        0,
        rayDesc,
        payload);

    return payload.color;
}

//�e�N�X�`���̃T���v�����O
inline static float4 SampleTexture(in Texture2D tex, in SamplerState sampler, in float2 uv) {
    return tex.SampleLevel(sampler, uv, 0.0);
}


#endif //! SHADER_RAYTRACING_UTIL_HELPER_HLSLI