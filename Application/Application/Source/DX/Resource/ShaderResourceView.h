/**
 * @file ShaderResourceView.h
 * @brief �V�F�[�_�[���\�[�X�r���[�N���X
 */

#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
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
        void initAsTexture2D(
            DeviceResource* device, const Buffer& buffer, DXGI_FORMAT format, bool isGlobal = true);
        void initAsBuffer(DeviceResource* device, const Buffer& buffer, bool isGlobal = true);
        void initAsRawBuffer(
            DeviceResource* device, const Buffer& buffer, UINT numElements, bool isGlobal = true);
        void initAsRaytracingAccelerationStructure(
            DeviceResource* device, const Buffer& buffer, bool isGlobal = true);
        const DescriptorInfo& getInfo() const {
            return mInfo;
        }

    private:
        void createShaderResourceView(DeviceResource* device, ID3D12Resource* resource,
            const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, bool isGlobal);

    private:
        DescriptorInfo mInfo;
    };
} // namespace Framework::DX
