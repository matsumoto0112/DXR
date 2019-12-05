#include "Texture2D.h"

using Framework::Desc::TextureFormat;

namespace {
    static constexpr UINT BYTES_PER_PIXEL = 4; // 1ピクセルのバイト数
    //フォーマット変換マップ
    static const std::unordered_map<TextureFormat, DXGI_FORMAT> FORMATS = {
        { TextureFormat::R8G8B8A8, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM },
        { TextureFormat::UNKNOWN, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN },
    };
} // namespace

namespace Framework::DX {
    void Texture2D::init(ID3D12Device* device, const Desc::TextureDesc& desc) {
        mWidth = desc.width;
        mHeight = desc.height;
        mFormat = desc.format;

        mBuffer.init(device, Buffer::Usage::ShaderResource, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            mWidth, mHeight, desc.name);

        //テクスチャデータを書き込む
        D3D12_BOX box = { 0, 0, 0, desc.width, desc.height, 1 };
        UINT row = desc.width * 4;
        UINT slice = row * desc.height;
        MY_THROW_IF_FAILED(
            mBuffer.getResource()->WriteToSubresource(0, &box, desc.pixels.data(), row, slice));
    }
    //シェーダーリソースビューを作成
    void Texture2D::createSRV(ID3D12Device* device, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
        const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) {
        mView.initAsTexture2D(
            device, mBuffer, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, cpuHandle, gpuHandle);
    }

} // namespace Framework::DX
