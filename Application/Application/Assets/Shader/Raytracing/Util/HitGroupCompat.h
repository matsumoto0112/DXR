#ifndef SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
#define SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Application/Source/Typedef.h"
using namespace DirectX;
#endif

/**
* @brief �q�b�g�O���[�v�̃R���X�^���g�o�b�t�@
*/
struct HitGroupConstant {
    UINT vertexOffset; //!< ���_�I�t�Z�b�g
    UINT indexOffset; //!< �C���f�b�N�X�I�t�Z�b�g
};

#endif // !SHADER_RAYTRACING_HITGROUP_HITGROUPCOMPAT_H
