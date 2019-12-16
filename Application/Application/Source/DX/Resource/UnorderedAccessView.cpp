#include "UnorderedAccessView.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {
    void UnorderedAccessView::initAsTexture2D(
        DeviceResource* device, const Buffer& buffer, bool isGlobal) {
        if (isGlobal) {
            mInfo = device->getRaytracingDescriptorManager()->getGlobalView()->allocate(
                device->getDevice());
        } else {
            mInfo = device->getRaytracingDescriptorManager()->getLocalView()->allocate(
                device->getDevice());
        }

        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

        device->getDevice()->CreateUnorderedAccessView(
            buffer.getResource(), nullptr, &uavDesc, mInfo.cpuHandle);
    }
} // namespace Framework::DX
