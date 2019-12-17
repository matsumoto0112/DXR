#include "ConstantBufferView.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {
    void ConstantBufferView::init(DeviceResource* device, const Buffer& buffer, bool isGlobal) {
        if (isGlobal) {
            mInfo = device->getRaytracingDescriptorManager()->allocateGlobal();
        } else {
            mInfo = device->getRaytracingDescriptorManager()->allocateLocal();
        }

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = buffer.getResource()->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = static_cast<UINT>(buffer.getSize());
        device->getDevice()->CreateConstantBufferView(&cbvDesc, mInfo.cpuHandle);
    }
} // namespace Framework::DX
