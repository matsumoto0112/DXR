/**
* @file MissCompat.h
* @brief �~�X�V�F�[�_�[���Ŏg�p����\���́A��`
*/

#ifndef SHADER_RAYTRACING_UTIL_MISSCOMPAT_H
#define SHADER_RAYTRACING_UTIL_MISSCOMPAT_H

#ifdef HLSL
#include "Typedef.hlsli"
#else
#include <DirectXMath.h>
#include <Windows.h>
#include "Application/Source/Typedef.h"
using namespace DirectX;
#endif

struct MissConstant {
    Color back;
};

#endif // !SHADER_RAYTRACING_UTIL_MISSCOMPAT_H
