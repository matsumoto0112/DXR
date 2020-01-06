#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RenderTargetView
     * @brief �����_�[�^�[�Q�b�g�r���[
     */
    class RenderTargetView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        RenderTargetView() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~RenderTargetView() {}
        /**
         * @brief ������
         */
        void init(DeviceResource* device, const Buffer& buffer);
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