#include "RenderTargetView.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void RenderTargetView::init(DeviceResource* device, const Buffer& buffer) {
        mInfo = device->getHeapManager()->allocate(DescriptorHeapType::Rtv);

        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = buffer.getResource()->GetDesc().Format;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        device->getDevice()->CreateRenderTargetView(
            buffer.getResource(), &rtvDesc, mInfo.cpuHandle);
    }
} // namespace Framework::DX
