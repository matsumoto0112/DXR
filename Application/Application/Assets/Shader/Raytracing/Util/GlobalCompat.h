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

//���W�X�^�[�̊J�n�ʒu��`
//�O���[�o���̈�̃R���X�^���g�o�b�t�@�J�n�n�_
#define GLOBAL_CONSTANT_BUFFER_VIEW_RESGISTER_START 0
//���[�J���̈�̃R���X�^���g�o�b�t�@�J�n�n�_
#define LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_START 1
//�O���[�o���̈�̃V�F�[�_�[���\�[�X�J�n�n�_
#define GLOBAL_SHADER_RESOURCE_VIEW_REGISTER_START 0
//���[�J���̈�̃V�F�[�_�[���\�[�X�J�n�n�_
#define LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START 3
//�O���[�o���̈�̃A���I�[�_�[�h�A�N�Z�X�J�n�n�_
#define GLOBAL_UNORDERED_ACCESS_VIEW_REGISTER_START 0
//���[�J���̈�̃A���I�[�_�[�h�A�N�Z�X�J�n�n�_
#define LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_START 1
//�O���[�o���̈�̃R���X�^���g�o�b�t�@�̐�
#define GLOBAL_CONSTANT_BUFFER_VIEW_RESGISTER_NUM LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_START
//�O���[�o���̈�̃V�F�[�_�[���\�[�X�̐�
#define GLOBAL_SHADER_RESOURCE_VIEW_REGISTER_NUM LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START
//�O���[�o���̈�̃A���I�[�_�[�h�A�N�Z�X�̐�
#define GLOBAL_UNORDERED_ACCESS_VIEW_REGISTER_NUM LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_START
#define LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_NUM (16 - LOCAL_CONSTANT_BUFFER_VIEW_RESGISTER_START)
#define LOCAL_SHADER_RESOURCE_VIEW_REGISTER_NUM 48 - LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START
#define LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_NUM 16 - LOCAL_UNORDERED_ACCESS_VIEW_REGISTER_START

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
    float gammaRate;
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
