#include "VertexBufferView.h"

namespace Framework::DX {
    //初期化
    void VertexBufferView::init(Buffer* buffer) {
        MY_THROW_IF_FALSE_LOG(buffer, "バッファが存在しません");
        MY_ASSERTION(
            buffer->getResourceType() == Buffer::Usage::VertexBuffer, "バッファの種類が違います");

        mView.BufferLocation = buffer->getResource()->GetGPUVirtualAddress();
        mView.SizeInBytes = buffer->getSize();
        mView.StrideInBytes = buffer->getStride();
    }
} // namespace Framework::DX
