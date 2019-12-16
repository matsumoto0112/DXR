#include "Buffer.h"
#include "DX/DeviceResource.h"
#include "Math/MathUtility.h"

namespace {
    /**
     * @brief �o�b�t�@�T�C�Y���v�Z����
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
     * @brief ������
     */
    void Buffer::init(
        DeviceResource* device, Usage usage, UINT64 size, UINT stride, const std::wstring& name) {
        mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
        CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(calcBufferSize(usage, size));

        MY_THROW_IF_FAILED(device->getDevice()->CreateCommittedResource(&props,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, mCurrentState, nullptr,
            IID_PPV_ARGS(&mResource)));
        MY_THROW_IF_FAILED(mResource->SetName(name.c_str()));

        mResourceType = usage;
        mSize = desc.Width;
        mStride = stride;
    }
    //������
    void Buffer::init(DeviceResource* device, const Desc::TextureDesc& texDesc,
        const D3D12_CLEAR_VALUE* clearValue) {
        CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC desc
            = CD3DX12_RESOURCE_DESC::Tex2D(texDesc.format, texDesc.width, texDesc.height);

        switch (texDesc.flags) {
        case Desc::TextureFlags::None:
            mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
            break;
        case Desc::TextureFlags::RenderTarget:
            mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
            desc.Flags |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            break;
        case Desc::TextureFlags::DepthStencil:
            mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE;
            desc.Flags |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            break;
        case Desc::TextureFlags::UnorderedAccess:
            mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
            desc.Flags |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            desc.MipLevels = 1;
            break;
        default: break;
        }

        MY_THROW_IF_FAILED(device->getDevice()->CreateCommittedResource(&props,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, mCurrentState, clearValue,
            IID_PPV_ARGS(&mResource)));
        MY_THROW_IF_FAILED(mResource->SetName(texDesc.name.c_str()));

        mResourceType = Usage::ShaderResource;
        mSize = texDesc.width * texDesc.height;
        mStride = 0;
    }
    //���\�[�X���珉����
    void Buffer::init(Comptr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state) {
        mResource = resource;
        mCurrentState = state;
    }

    //�������̃}�b�v����
    void* Buffer::map() {
        void* mapped;
        MY_THROW_IF_FAILED(mResource->Map(0, nullptr, &mapped));
        return mapped;
    }
    //�������̃A���}�b�v����
    void Buffer::unmap() {
        mResource->Unmap(0, nullptr);
    }
    //���\�[�X�Ƀf�[�^����������
    void Buffer::writeResource(const void* data, UINT64 size) {
        void* mapped = map();
        memcpy(mapped, data, size);
        unmap();
    }
    //���\�[�X�̏�Ԃ̑J��
    void Buffer::transition(
        ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState) {
        if (mCurrentState == nextState) return;
        commandList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(), mCurrentState, nextState));
        mCurrentState = nextState;
    }
} // namespace Framework::DX
