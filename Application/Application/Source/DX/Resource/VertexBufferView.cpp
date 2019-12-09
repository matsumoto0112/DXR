#include "VertexBufferView.h"

namespace Framework::DX {
    //初期化
    void VertexBufferView::init(const Buffer& buffer) {
        MY_ASSERTION(
            buffer.getResourceType() == Buffer::Usage::VertexBuffer, "バッファの種類が違います");

        mView.BufferLocation = buffer.getResource()->GetGPUVirtualAddress();
        mView.SizeInBytes = buffer.getSize();
        mView.StrideInBytes = buffer.getStride();
    }
} // namespace Framework::DX
