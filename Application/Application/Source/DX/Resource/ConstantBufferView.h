#pragma once
#include "DX/Descriptor/DescriptorHeapFlag.h"
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class ConstantBufferView
     * @brief discription
     */
    class ConstantBufferView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        ConstantBufferView() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~ConstantBufferView() {}
        void init(DeviceResource* device, const Buffer& buffer, DescriptorHeapType flag);
        const DescriptorInfo& getInfo() const {
            return mInfo;
        }

    private:
        DescriptorInfo mInfo;
    };
} // namespace Framework::DX
