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
    //コンストラクタ
    Texture2D::Texture2D() : mWidth(0), mHeight(0), mFormat(TextureFormat::UNKNOWN) {}
    //コンストラクタ
    Texture2D::Texture2D(ID3D12Device* device, const Desc::TextureDesc& desc) {
        createBuffer(device, desc);
    }
    //デストラクタ
    Texture2D::~Texture2D() {}
    //バッファを作成する
    void Texture2D::createBuffer(ID3D12Device* device, const Desc::TextureDesc& desc) {
        mWidth = desc.width;
        mHeight = desc.height;
        mFormat = desc.format;

        //テクスチャリソースを作成する
        CD3DX12_RESOURCE_DESC texDesc
            = CD3DX12_RESOURCE_DESC::Tex2D(FORMATS.at(desc.format), desc.width, desc.height, 1);
        CD3DX12_HEAP_PROPERTIES props(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
            D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);

        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &texDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&mResource)));
        mResource->SetName(desc.name.c_str());

        //テクスチャデータを書き込む
        D3D12_BOX box = { 0, 0, 0, desc.width, desc.height, 1 };
        UINT row = desc.width * 4;
        UINT slice = row * desc.height;
        MY_THROW_IF_FAILED(mResource->WriteToSubresource(0, &box, desc.pixels.data(), row, slice));
    }
    //シェーダーリソースビューを作成
    void Texture2D::createSRV(ID3D12Device* device) {
        //シェーダーリソースビューを作成する
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = FORMATS.at(mFormat);
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.PlaneSlice = 0;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

        device->CreateShaderResourceView(mResource.Get(), &srvDesc, mCPUHandle);
    }

} // namespace Framework::DX
