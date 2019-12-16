#include "Texture2D.h"

namespace {
    static constexpr UINT BYTES_PER_PIXEL = 4; // 1ピクセルのバイト数
} // namespace

namespace Framework::DX {
    //初期化
    void Texture2D::init(ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
        const Desc::TextureDesc& desc) {
        mTextureInfo = desc;

        mBuffer.init(device, desc);
        mImmediateBuffer.init(device, Buffer::Usage::ShaderResource,
            static_cast<UINT>(GetRequiredIntermediateSize(mBuffer.getResource(), 0, 1)), 0,
            L"Immediate" + desc.name);

        D3D12_SUBRESOURCE_DATA subresource = {};
        subresource.pData = desc.pixels.data();
        subresource.RowPitch = desc.width * BYTES_PER_PIXEL;
        subresource.SlicePitch = subresource.RowPitch * desc.height;
        UpdateSubresources(commandList, mBuffer.getResource(), mImmediateBuffer.getResource(), 0, 0,
            1, &subresource);

        mBuffer.transition(commandList, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
    }
    //シェーダーリソースビューを作成
    void Texture2D::createSRV(ID3D12Device* device, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
        const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) {
        mView.initAsTexture2D(device, mBuffer, mTextureInfo.format, cpuHandle, gpuHandle);
    }

} // namespace Framework::DX
