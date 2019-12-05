#include "VertexBufferView.h"

namespace Framework::DX {
    //������
    void VertexBufferView::init(Buffer* buffer) {
        MY_THROW_IF_FALSE_LOG(buffer, "�o�b�t�@�����݂��܂���");
        MY_ASSERTION(
            buffer->getResourceType() == Buffer::Usage::VertexBuffer, "�o�b�t�@�̎�ނ��Ⴂ�܂�");

        mView.BufferLocation = buffer->getResource()->GetGPUVirtualAddress();
        mView.SizeInBytes = buffer->getSize();
        mView.StrideInBytes = buffer->getStride();
    }
} // namespace Framework::DX
