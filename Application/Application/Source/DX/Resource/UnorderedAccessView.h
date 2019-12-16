/**
 * @file UnorderedAccessView.h
 * @brief アンオーダードアクセスビュー
 */

#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class UnorderedAccessView
     * @brief discription
     */
    class UnorderedAccessView {
    public:
        /**
         * @brief
         */
        UnorderedAccessView() {}
        /**
         * @brief
         */
        ~UnorderedAccessView() {}
        void initAsTexture2D(DeviceResource* device, const Buffer& buffer,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        const D3D12_CPU_DESCRIPTOR_HANDLE& getCPUHandle() const {
            return mCPUHandle;
        }
        const D3D12_GPU_DESCRIPTOR_HANDLE& getGPUHandle() const {
            return mGPUHandle;
        }

    private:
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
    };
} // namespace Framework::DX
