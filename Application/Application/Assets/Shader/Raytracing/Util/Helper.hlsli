/**
* @file Helper.hlsli
* @brief �w���p�[�֐��A�\���̒�`
*/

#ifndef SHADER_RAYTRACING_UTIL_HELPER_HLSLI
#define SHADER_RAYTRACING_UTIL_HELPER_HLSLI



#define HLSL
struct Ray {
    float3 origin;
    float3 direction;
};

/**
* @brief �J��������̃��C���΂�
*/
inline Ray generateCameraRay(in uint2 index, in float3 cameraPosition, in float4x4 projectionToWorld) {
    float2 xy = index + float2(0.5, 0.5);
    float2 screenPos = xy / DispatchRaysDimensions().xy * 2.0 - 1.0;
    screenPos.y = -screenPos.y;

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
static inline uint3 loadIndices(uint offsetBytes, ByteAddressBuffer Indices) {
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
* @brief �Փ˓_�̖@�����擾����
*/
inline static float3 getNormal(float3 normals[3], in MyAttr attr) {
    return normals[0] +
        attr.barycentrics.x * (normals[1] - normals[0]) +
        attr.barycentrics.y * (normals[2] - normals[0]);
}

/**
* @brief �Փ˓_��UV���擾����
*/
inline static float2 getUV(float2 uvs[3], in MyAttr attr) {
    return uvs[0] +
        attr.barycentrics.x * (uvs[1] - uvs[0]) +
        attr.barycentrics.y * (uvs[2] - uvs[0]);
}


#endif //! SHADER_RAYTRACING_UTIL_HELPER_HLSLI