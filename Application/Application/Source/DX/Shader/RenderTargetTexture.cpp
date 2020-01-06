#include "RenderTargetTexture.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void RenderTargetTexture::init(DeviceResource* device, UINT width, UINT height,
        DXGI_FORMAT format, const Utility::Color4& clearColor, const std::wstring& name) {
        mRenderTarget.init(device, width, height, format, clearColor, name);
        mSRV.initAsTexture2D(
            device, mRenderTarget.getBuffer(), format, DescriptorHeapType::CbvSrvUav);
    }
    void RenderTargetTexture::transition(
        ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState) {
        mRenderTarget.transition(commandList, nextState);
    }
    void RenderTargetTexture::transition(
        ID3D12GraphicsCommandList* commandList, RenderTargetState nextMode) {
        D3D12_RESOURCE_STATES nextState = {};
        switch (nextMode) {
        case RenderTargetState::ShaderResource:
            nextState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
            break;
        case RenderTargetState::RenderTarget:
            nextState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
            break;
        default: return;
        }
        transition(commandList, nextState);
    }
    void RenderTargetTexture::clear(
        ID3D12GraphicsCommandList* commandList, const Utility::Color4& clearColor) {
        mRenderTarget.clear(commandList, clearColor);
    }
} // namespace Framework::DX
