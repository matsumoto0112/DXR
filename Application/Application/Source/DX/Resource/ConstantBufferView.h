#pragma once
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
        void init(DeviceResource* device, const Buffer& buffer,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
    };
} // namespace Framework::DX
