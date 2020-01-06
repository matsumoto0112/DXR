#include "DepthStencil.h"
#include "DX/DeviceResource.h"
#include "DX/Shader/DepthStencilFormat.h"

namespace Framework::DX {

    void DepthStencil::init(DeviceResource* device, UINT width, UINT height, DXGI_FORMAT format,
        const std::wstring& name) {
        //BUFFER_FORMAT
        Desc::TextureDesc desc = {};
        desc.width = width;
        desc.height = height;
        desc.format = DepthStencilFormat::bufferFormat(format);
        desc.name = name;
        desc.flags = Desc::TextureFlags::DepthStencil;

        //CLEAR_VALUE_FORMAT
        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = format;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0;

        mBuffer.init(device, desc, &clearValue);

        mDSV.init(device, mBuffer, format);
    }
    void DepthStencil::transition(
        ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState) {
        mBuffer.transition(commandList, nextState);
    }
    void DepthStencil::clear(ID3D12GraphicsCommandList* commandList) {
        commandList->ClearDepthStencilView(mDSV.getInfo().cpuHandle,
            D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_STENCIL,
            1.0f, 0, 0, nullptr);
    }
} // namespace Framework::DX
