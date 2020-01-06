#include "DepthStencilTexture.h"
#include "DX/DeviceResource.h"
#include "DX/Shader/DepthStencilFormat.h"

namespace Framework::DX {

    void DepthStencilTexture::init(DeviceResource* device, UINT width, UINT height,
        DXGI_FORMAT format, const std::wstring& name) {
        mDepthStencil.init(device, width, height, format, name);

        mSRV.initAsTexture2D(device, mDepthStencil.getBuffer(),
            DepthStencilFormat::shaderResourceViewFormat(format), DescriptorHeapType::CbvSrvUav);
    }
    void DepthStencilTexture::transition(
        ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState) {}
    void DepthStencilTexture::transition(
        ID3D12GraphicsCommandList* commandList, DepthStencilState nextMode) {
        D3D12_RESOURCE_STATES nextState = {};
        switch (nextMode) {
        case DepthStencilState::ShaderResource:
            nextState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
            break;
        case DepthStencilState::DepthStencil:
            nextState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE;
            break;
        default: return;
        }
        transition(commandList, nextState);
    }
    void DepthStencilTexture::clear(ID3D12GraphicsCommandList* commandList) {
        mDepthStencil.clear(commandList);
    }
} // namespace Framework::DX
