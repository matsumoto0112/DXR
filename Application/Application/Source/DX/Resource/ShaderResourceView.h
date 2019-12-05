#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class ShaderResourceView
     * @brief シェーダーリソースビュー
     */
    class ShaderResourceView {
    public:
        /**
         * @brief コンストラクタ
         */
        ShaderResourceView() {}
        /**
         * @brief デストラクタ
         */
        ~ShaderResourceView() {}
        void initAsTexture2D(ID3D12Device* device, const Buffer& buffer, DXGI_FORMAT format,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        void initAsBuffer(ID3D12Device* device, const Buffer& buffer,
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
