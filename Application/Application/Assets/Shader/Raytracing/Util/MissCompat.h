/**
 * @file MissCompat.h
 * @brief ミスシェーダー用の定義
 */

#ifndef SHADER_RAYTRACING_UTIL_MISSCOMPAT_H
#define SHADER_RAYTRACING_UTIL_MISSCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Typedef.h"
#endif

/**
 * @struct MissConstant
 * @brief ミスシェーダー用のコンスタントバッファ
 */
struct MissConstant {
    Color back; //!< 背景色
};

#endif // !SHADER_RAYTRACING_UTIL_MISSCOMPAT_H
