/**
* @file Global.hlsli
* @brief �V�F�[�_�[�t�@�C���S�̂Ŏg�p����O���[�o���w�b�_�[
*/


#ifndef SHADER_RAYTRACING_GLOBAL_HLSLI
#define SHADER_RAYTRACING_GLOBAL_HLSLI

#define HLSL
#include "GlobalCompat.h"

//Top-Level-AS�\����
RaytracingAccelerationStructure g_scene : register(t0, space0);
//�����_�[�^�[�Q�b�g
RWTexture2D<float4> g_renderTarget : register(u0);
//�V�[�����
ConstantBuffer<SceneConstantBuffer> g_sceneCB : register(b0);
//�C���f�b�N�X�z��
ByteAddressBuffer Indices : register(t1, space0);
//���_�z��
StructuredBuffer<Vertex> Vertices : register(t2, space0);
//�O�p�`�̏Փˏ��
typedef BuiltInTriangleIntersectionAttributes MyAttr;

//���`�T���v���[
SamplerState samLinear : register(s0);

#endif //! SHADER_RAYTRACING_GLOBAL_HLSLI