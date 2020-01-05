#include "Texture2D.h"

namespace {
    static constexpr UINT BYTES_PER_PIXEL = 4; // 1�s�N�Z���̃o�C�g��
} // namespace

namespace Framework::DX {
    //������
    void Texture2D::init(DeviceResource* device, ID3D12GraphicsCommandList* commandList,
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
    //�V�F�[�_�[���\�[�X�r���[���쐬
    void Texture2D::createSRV(DeviceResource* device, DescriptorHeapType heapFlag) {
        mView.initAsTexture2D(device, mBuffer, mTextureInfo.format, heapFlag);
    }

} // namespace Framework::DX
