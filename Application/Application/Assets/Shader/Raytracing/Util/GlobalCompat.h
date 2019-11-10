/**
* @file GlobalCompat.h
* @brief �V�F�[�_�[�t�@�C����cpp���ŋ��ʂ��Ďg�p����݊������������\���́A��`
*/

#ifndef SHADER_RAYTRACING_GLOBALCOMPAT_H
#define SHADER_RAYTRACING_GLOBALCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Application/Source/Typedef.h"
using namespace DirectX;
typedef UINT16 Index;
#endif

static const UINT MAX_RAY_RECURSION_DEPTH = 2; //!< �ő僌�C�ċA��

/**
* @brief ���_���
*/
struct Vertex {
    Vec3 position; //!< ���W
    Vec3 normal; //!< �@��
    Vec2 uv; //!< UV���W
    Vec4 tangent; //!< �ڐ�
};

/**
* @brief �V�[���S�̂̏��
*/
struct SceneConstantBuffer {
    Mat4 projectionToWorld; //!< �v���W�F�N�V������Ԃ��烏�[���h��Ԃւ̕ϊ��s��
    Vec4 cameraPosition; //!< �J�����̃��[���h���W
    Vec4 lightPosition; //!< �f�B���N�V���i�����C�g�̃��[���h���W
    Color lightDiffuse; //!< �f�B���N�V���i�����C�g�̐F
    Color lightAmbient; //!< ���F
};

/**
* @brief ���C�y�C���[�h
*/
struct RayPayload {
    Color color; //!< �F
    UINT recursionCount; //!<
};

/**
* @brief �e�p�y�C���[�h
*/
struct ShadowPayload {
    bool hit; //!< �����ɓ���������
};

#endif // !SHADER_RAYTRACING_GLOBALCOMPAT_H
