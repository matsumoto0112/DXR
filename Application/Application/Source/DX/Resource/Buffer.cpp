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
     * @brief 初期化
     */
    void Buffer::init(
        ID3D12Device* device, Usage usage, UINT size, UINT stride, const std::wstring& name) {
        mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
        mStride = stride;

        CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(calcBufferSize(usage, size));
        init(device, usage, props, desc, name);
    }

    void Buffer::init(ID3D12Device* device, const Desc::TextureDesc& texDesc) {
        CD3DX12_RESOURCE_DESC desc = {};
        if (texDesc.flags == Desc::TextureFlags::UnorderedAccess) {
            mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
            desc = CD3DX12_RESOURCE_DESC::Tex2D(texDesc.format, texDesc.width, texDesc.height, 1, 1,
                1, 0, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
        } else {
            desc = CD3DX12_RESOURCE_DESC::Tex2D(texDesc.format, texDesc.width, texDesc.height);
            mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
        }
        CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
        init(device, Usage::ShaderResource, props, desc, texDesc.name);
    }
    //メモリのマップ処理
    void* Buffer::map() {
        void* mapped;
        MY_THROW_IF_FAILED(mResource->Map(0, nullptr, &mapped));
        return mapped;
    }
    //メモリのアンマップ処理
    void Buffer::unmap() {
        mResource->Unmap(0, nullptr);
    }
    //リソースにデータを書き込む
    void Buffer::writeResource(const void* data, UINT64 size) {
        void* mapped = map();
        memcpy(mapped, data, size);
        unmap();
    }
    //リソースの状態の遷移
    void Buffer::transition(
        ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState) {
        if (mCurrentState == nextState) return;
        commandList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(), mCurrentState, nextState));
        mCurrentState = nextState;
    }
    void Buffer::init(ID3D12Device* device, Usage usage, const CD3DX12_HEAP_PROPERTIES& props,
        const CD3DX12_RESOURCE_DESC& desc, const std::wstring& name) {
        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
                mCurrentState, nullptr, IID_PPV_ARGS(&mResource)));
        MY_THROW_IF_FAILED(mResource->SetName(name.c_str()));

        mResourceType = usage;
        mSize = static_cast<UINT>(desc.Width);
    }
} // namespace Framework::DX
