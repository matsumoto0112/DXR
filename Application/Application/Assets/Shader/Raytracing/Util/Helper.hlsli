/**
* @file Helper.hlsli
* @brief ヘルパー関数、構造体定義
*/

#ifndef SHADER_RAYTRACING_UTIL_HELPER_HLSLI
#define SHADER_RAYTRACING_UTIL_HELPER_HLSLI



#define HLSL
struct Ray {
    float3 origin;
    float3 direction;
};

/**
* @brief カメラからのレイを飛ばす
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
* @brief インデックスを読み込む
* @details インデックスは2バイトで登録されているがシェーダー内では4バイト単位でしか読み込めないため
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
* @brief 衝突点のワールド座標を取得する
*/
inline float3 hitWorldPosition() {
    return WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
}

/**
* @brief 衝突点の法線を取得する
*/
inline static float3 getNormal(float3 normals[3], in MyAttr attr) {
    return normals[0] +
        attr.barycentrics.x * (normals[1] - normals[0]) +
        attr.barycentrics.y * (normals[2] - normals[0]);
}

/**
* @brief 衝突点のUVを取得する
*/
inline static float2 getUV(float2 uvs[3], in MyAttr attr) {
    return uvs[0] +
        attr.barycentrics.x * (uvs[1] - uvs[0]) +
        attr.barycentrics.y * (uvs[2] - uvs[0]);
}


#endif //! SHADER_RAYTRACING_UTIL_HELPER_HLSLI