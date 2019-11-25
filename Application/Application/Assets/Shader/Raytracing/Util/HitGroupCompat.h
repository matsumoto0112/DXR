/**
 * @file HitGroupCompat.h
 * @brief �q�b�g�O���[�v�V�F�[�_�[�̒�`
 */

#ifndef SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
#define SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Typedef.h"
#endif

/**
 * @struct HitGroupConstant
 * @brief �q�b�g�O���[�v�p�̃��[�J���R���X�^���g�o�b�t�@
 */
struct HitGroupConstant {
    UINT vertexOffset; //!< ���_�I�t�Z�b�g
    UINT indexOffset; //!< �C���f�b�N�X�I�t�Z�b�g
    Vec3 emissiveFactor; //!< �G�~�b�V�����̗v�f
};

#endif // !SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
