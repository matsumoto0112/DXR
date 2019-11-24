#ifndef SHADER_RAYTRACING_GLOBALCOMPAT_H
#define SHADER_RAYTRACING_GLOBALCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "DX/ModelCompat.h"
#include "Typedef.h"
using namespace DirectX;
typedef UINT16 Index;
#endif

static const UINT MAX_RAY_RECURSION_DEPTH = 2;

/**
 * @brief 頂点構造体
 */
struct Vertex {
    Vec3 position; //!< 座標
    Vec3 normal; //!< 法線
    Vec2 uv; //!< UV座標
    Vec4 tangent; //!< 接線
};

/**
 * @brief シーン全体の情報
 */
struct SceneConstantBuffer {
    Mat4 projectionToWorld;
    Vec4 cameraPosition; //!< カメラのワールド座標
    Vec4 lightPosition; //!< ディレクショナルライトのワールド座標
    Color lightDiffuse; //!< ディレクショナルライトの色
    Color lightAmbient; //!< 環境色
};

/**
 * @brief メインのレイのペイロード
 */
struct RayPayload {
    Color color; //!< 色
    UINT recursionCount; //!<
};

/**
 * @brief 影のレイのペイロード
 */
struct ShadowPayload {
    bool hit; //!< 何かに当たったか
};

#endif // !SHADER_RAYTRACING_GLOBALCOMPAT_H
