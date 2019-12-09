#include "Buffer.h"
#include "Math/MathUtility.h"

namespace {
    /**
     * @brief バッファサイズを計算する
     */
    inline constexpr UINT calcBufferSize(Framework::DX::Buffer::Usage usage, UINT size) {
        if (usage == Framework::DX::Buffer::Usage::ConstantBuffer) {
            size = Framework::Math::MathUtil::alignPow2(
                size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
        }
        return size;
    }
} // namespace

namespace Framework::DX {
    /**
     * @brief デストラクタ
     */
    Buffer::~Buffer() {
        if (mMapped) { mResource->Unmap(0, nullptr); }
    }
    /**
     * @brief 初期化
     */
    void Buffer::init(
        ID3D12Device* device, Usage usage, UINT size, UINT stride, const std::wstring& name) {
        CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(calcBufferSize(usage, size));

        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
                D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&mResource)));
        MY_THROW_IF_FAILED(mResource->SetName(name.c_str()));

        mResourceType = usage;
        mSize = desc.Width;
        mStride = stride;
        mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
    }

    /**
     * @brief 初期化
     */
    void Buffer::init(ID3D12Device* device, Usage usage, DXGI_FORMAT format, UINT width,
        UINT height, const std::wstring& name) {
        CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
        CD3DX12_HEAP_PROPERTIES props(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
            D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);

        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &texDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&mResource)));
        mResource->SetName(name.c_str());
    }

    /**
     * @brief メモリのマップ
     */
    void* Buffer::map() {
        if (mMapped) return mMapped;
        MY_THROW_IF_FAILED(mResource->Map(0, nullptr, &mMapped));
        return mMapped;
    }
    /**
     * @brief メモリのアンマップ
     */
    void Buffer::unmap() {
        mResource->Unmap(0, nullptr);
        mMapped = nullptr;
    }
    /**
     * @brief リソースにデータを書き込む
     */
    void Buffer::writeResource(const void* data, UINT size) {
        void* mapped = map();
        memcpy(mapped, data, size);
        unmap();
    }
} // namespace Framework::DX
