#include "Texture2D.h"

using Framework::Desc::TextureFormat;

namespace {
    static constexpr UINT BYTES_PER_PIXEL = 4; // 1ピクセルのバイト数
    static constexpr DXGI_FORMAT toFormat(TextureFormat format) {
        switch (format) {
        case Framework::Desc::TextureFormat::R8G8B8A8:
            return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        default: return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        }
    }
} // namespace

namespace Framework::DX {
    //初期化
    void Texture2D::init(ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
        const Desc::TextureDesc& desc) {
        mFormat = toFormat(desc.format);
        mBuffer.init(
            device, Buffer::Usage::ShaderResource, mFormat, desc.width, desc.height, desc.name);
        mImmediateBuffer.init(device, Buffer::Usage::ShaderResource,
            GetRequiredIntermediateSize(mBuffer.getResource(), 0, 1), 0, L"Immediate" + desc.name);

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
        mView.initAsTexture2D(device, mBuffer, mFormat, cpuHandle, gpuHandle);
    }

} // namespace Framework::DX
