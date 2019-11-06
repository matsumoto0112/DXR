#ifndef SHADER_RAYTRACING_GLOBAL_HLSLI
#define SHADER_RAYTRACING_GLOBAL_HLSLI

#define HLSL
#include "Compat.h"

//Top-Level-AS�\����
RaytracingAccelerationStructure g_scene : register(t0, space0);
//�����_�[�^�[�Q�b�g
RWTexture2D<float4> g_renderTarget : register(u0);
//�V�[�����
ConstantBuffer<SceneConstantBuffer> g_sceneCB : register(b0);

typedef BuiltInTriangleIntersectionAttributes MyAttr;

#endif //! SHADER_RAYTRACING_GLOBAL_HLSLI