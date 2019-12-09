#include "UnorderedAccessView.h"

namespace Framework::DX {

    void UnorderedAccessView::initAsTexture2D(ID3D12Device* device, const Buffer& buffer,
        const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
        const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) {
        mCPUHandle = cpuHandle;
        mGPUHandle = gpuHandle;

        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

        device->CreateUnorderedAccessView(buffer.getResource(), nullptr, &uavDesc, mCPUHandle);
    }
} // namespace Framework::DX
