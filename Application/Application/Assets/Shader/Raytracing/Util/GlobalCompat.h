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

//レジスターの開始位置定義
//グローバル領域のコンスタントバッファ開始地点
#define GLOBAL_CONSTANT_BUFFER_VIEW_RESGISTER_START 0
//ローカル領域のコンスタントバッファ開始地点
#define LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_START 1
//グローバル領域のシェーダーリソース開始地点
#define GLOBAL_SHADER_RESOURCE_VIEW_REGISTER_START 0
//ローカル領域のシェーダーリソース開始地点
#define LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START 3
//グローバル領域のアンオーダードアクセス開始地点
#define GLOBAL_UNORDERED_ACCESS_VIEW_REGISTER_START 0
//ローカル領域のアンオーダードアクセス開始地点
#define LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_START 1
//グローバル領域のコンスタントバッファの数
#define GLOBAL_CONSTANT_BUFFER_VIEW_RESGISTER_NUM LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_START
//グローバル領域のシェーダーリソースの数
#define GLOBAL_SHADER_RESOURCE_VIEW_REGISTER_NUM LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START
//グローバル領域のアンオーダードアクセスの数
#define GLOBAL_UNORDERED_ACCESS_VIEW_REGISTER_NUM LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_START
#define LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_NUM (16 - LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_START)
#define LOCAL_SHADER_RESOURCE_VIEW_REGISTER_NUM 48 - LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START
#define LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_NUM 16 - LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_START

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
    float gammaRate;
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
