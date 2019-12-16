#pragma once
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
         * @brief コンストラクタ
         */
        ConstantBufferView() {}
        /**
         * @brief デストラクタ
         */
        ~ConstantBufferView() {}
        void init(DeviceResource* device, const Buffer& buffer, bool isGlobal);
        const DescriptorInfo& getInfo() const {
            return mInfo;
        }

    private:
        DescriptorInfo mInfo;
    };
} // namespace Framework::DX
