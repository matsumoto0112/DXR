#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class DepthStencilView
     * @brief �f�v�X�E�X�e���V���r���[
     */
    class DepthStencilView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        DepthStencilView() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~DepthStencilView() {}
        /**
         * @brief ������
         */
        void init(DeviceResource* device, const Buffer& buffer, DXGI_FORMAT format);
        /**
         * @brief �f�B�X�N���v�^���̎擾
         */
        const DescriptorInfo& getInfo() const {
            return mInfo;
        }

    private:
        DescriptorInfo mInfo;
    };
} // namespace Framework::DX