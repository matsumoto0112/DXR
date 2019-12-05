/**
 * @file GlobalCompat.h
 * @brief 全シェーダー、Cppファイルで使用する定義
 */

#ifndef SHADER_RAYTRACING_GLOBALCOMPAT_H
#define SHADER_RAYTRACING_GLOBALCOMPAT_H

#ifdef HLSL
// clang-format off
#include "Typedef.hlsli"
#include "../../../../Source/DX/ModelCompat.h"
// clang-format on
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "DX/ModelCompat.h"
#include "Typedef.h"
#endif

static const UINT MAX_RAY_RECURSION_DEPTH = 3; //! 最大再帰回数

/**
 * @brief シーン全体の情報
 */
struct SceneConstantBuffer {
    Mat4 projectionToWorld; //!< プロジェクション空間からワールド空間に変換
    Vec4 cameraPosition; //!< カメラのワールド座標
    Vec4 lightPosition; //!< ディレクショナルライトのワールド座標
    Color lightDiffuse; //!< ディレクショナルライトの色
    Color lightAmbient; //!< 環境色
    float globalTime; //!< アプリケーション開始時からの経過時間
};

/**
 * @brief メインのレイのペイロード
 */
struct RayPayload {
    Color color; //!< 色
    UINT recursionCount; //!< 再帰回数
};

/**
 * @brief 影のレイのペイロード
 */
struct ShadowPayload {
    bool hit; //!< 何かに当たったか
};

#endif // !SHADER_RAYTRACING_GLOBALCOMPAT_H
