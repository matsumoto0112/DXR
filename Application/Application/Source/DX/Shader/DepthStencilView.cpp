#include "DepthStencilView.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void DepthStencilView::init(DeviceResource* device, const Buffer& buffer, DXGI_FORMAT format) {
        mInfo = device->getHeapManager()->allocate(DescriptorHeapType::Dsv);

        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        //DSV_DESC_FORMAT
        dsvDesc.Format = format;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;
        dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
        device->getDevice()->CreateDepthStencilView(
            buffer.getResource(), &dsvDesc, mInfo.cpuHandle);
    }
} // namespace Framework::DX
