/**
 * @file ShaderResourceView.h
 * @brief �V�F�[�_�[���\�[�X�r���[�N���X
 */

#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class ShaderResourceView
     * @brief �V�F�[�_�[���\�[�X�r���[
     */
    class ShaderResourceView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        ShaderResourceView() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~ShaderResourceView() {}
        void initAsTexture2D(DeviceResource* device, const Buffer& buffer, DXGI_FORMAT format,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        void initAsBuffer(DeviceResource* device, const Buffer& buffer,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        void initAsRawBuffer(DeviceResource* device, const Buffer& buffer, UINT numElements,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        void initAsRaytracingAccelerationStructure(DeviceResource* device, const Buffer& buffer,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        const D3D12_CPU_DESCRIPTOR_HANDLE& getCPUHandle() const {
            return mCPUHandle;
        }
        const D3D12_GPU_DESCRIPTOR_HANDLE& getGPUHandle() const {
            return mGPUHandle;
        }

    private:
        D3D12_SHADER_RESOURCE_VIEW_DESC mView;
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
    };
} // namespace Framework::DX
