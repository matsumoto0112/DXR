#include "IndexBufferView.h"

namespace {
    inline constexpr DXGI_FORMAT calcFormatFromStride(UINT stride) {
        return stride == 2 ? DXGI_FORMAT::DXGI_FORMAT_R16_UINT : DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
    }
} // namespace

namespace Framework::DX {
    //������
    void IndexBufferView::init(Buffer* buffer) {
        MY_THROW_IF_FALSE_LOG(buffer, "�o�b�t�@�����݂��܂���");
        MY_ASSERTION(
            buffer->getResourceType() == Buffer::Usage::IndexBuffer, "�o�b�t�@�̎�ނ��Ⴂ�܂��B");

        mView.BufferLocation = buffer->getResource()->GetGPUVirtualAddress();
        mView.Format = calcFormatFromStride(buffer->getStride());
        mView.SizeInBytes = buffer->getSize();
    }
} // namespace Framework::DX
