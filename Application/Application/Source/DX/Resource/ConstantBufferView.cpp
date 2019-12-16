#include "ConstantBufferView.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {
    void ConstantBufferView::init(DeviceResource* device, const Buffer& buffer,
        const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
        const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) {
        mCPUHandle = cpuHandle;
        mGPUHandle = gpuHandle;

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = buffer.getResource()->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = static_cast<UINT>(buffer.getSize());
        device->getDevice()->CreateConstantBufferView(&cbvDesc, mCPUHandle);
    }
} // namespace Framework::DX
