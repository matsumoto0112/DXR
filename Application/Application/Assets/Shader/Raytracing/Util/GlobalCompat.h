/**
 * @file GlobalCompat.h
 * @brief �S�V�F�[�_�[�ACpp�t�@�C���Ŏg�p�����`
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

static const UINT MAX_RAY_RECURSION_DEPTH = 3; //! �ő�ċA��

/**
 * @brief �V�[���S�̂̏��
 */
struct SceneConstantBuffer {
    Mat4 projectionToWorld; //!< �v���W�F�N�V������Ԃ��烏�[���h��Ԃɕϊ�
    Vec4 cameraPosition; //!< �J�����̃��[���h���W
    Vec4 lightPosition; //!< �f�B���N�V���i�����C�g�̃��[���h���W
    Color lightDiffuse; //!< �f�B���N�V���i�����C�g�̐F
    Color lightAmbient; //!< ���F
    float globalTime; //!< �A�v���P�[�V�����J�n������̌o�ߎ���
};

/**
 * @brief ���C���̃��C�̃y�C���[�h
 */
struct RayPayload {
    Color color; //!< �F
    UINT recursionCount; //!< �ċA��
};

/**
 * @brief �e�̃��C�̃y�C���[�h
 */
struct ShadowPayload {
    bool hit; //!< �����ɓ���������
};

#endif // !SHADER_RAYTRACING_GLOBALCOMPAT_H
