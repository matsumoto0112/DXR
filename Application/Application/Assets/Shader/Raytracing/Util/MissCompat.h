/**
 * @file MissCompat.h
 * @brief �~�X�V�F�[�_�[�p�̒�`
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
 * @brief �~�X�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@
 */
struct MissConstant {
    Color back; //!< �w�i�F
};

#endif // !SHADER_RAYTRACING_UTIL_MISSCOMPAT_H
