/**
* @file GlobalCompat.h
* @brief シェーダーファイルとcpp内で共通して使用する互換性を持った構造体、定義
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

static const UINT MAX_RECURSION_NUM = 2;

struct SceneConstantBuffer {
    Mat4 projectionToWorld;
    Vec4 cameraPosition;
    Vec4 lightPosition;
    Color lightDiffuse;
    Color lightAmbient;
};

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
};

struct RayPayload {
    Color color;
};

struct ShadowPayload {
    bool hit;
};

#endif // !SHADER_RAYTRACING_GLOBALCOMPAT_H
