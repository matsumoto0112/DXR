#include "RenderTarget.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void RenderTarget::init(DeviceResource* device, UINT width, UINT height, DXGI_FORMAT format,
        const Utility::Color4& clearColor, const std::wstring& name) {
        Desc::TextureDesc texDesc = {};
        texDesc.width = width;
        texDesc.height = height;
        texDesc.name = name;
        texDesc.format = format;
        texDesc.flags = Desc::TextureFlags::RenderTarget;

        mBuffer.init(device, texDesc, &CD3DX12_CLEAR_VALUE(format, clearColor.get().data()));
        mView.init(device, mBuffer);
    }
    void RenderTarget::transition(
        ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState) {
        mBuffer.transition(commandList, nextState);
    }
    void RenderTarget::clear(ID3D12GraphicsCommandList* commandList, const Utility::Color4& color) {
        MY_ASSERTION(
            mBuffer.getCurrentState() == D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
            "レンダーターゲットとして扱っていないときにクリア処理が呼ばれました。");
        commandList->ClearRenderTargetView(
            mView.getInfo().cpuHandle, color.get().data(), 0, nullptr);
    }
    void RenderTarget::initAsResource(DeviceResource* device, Comptr<ID3D12Resource> resource) {
        mBuffer.init(resource, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);
        mView.init(device, mBuffer);
    }
} // namespace Framework::DX
