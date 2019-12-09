#include "IndexBufferView.h"

namespace {
    inline constexpr DXGI_FORMAT calcFormatFromStride(UINT stride) {
        return stride == 2 ? DXGI_FORMAT::DXGI_FORMAT_R16_UINT : DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
    }
} // namespace

namespace Framework::DX {
    //初期化
    void IndexBufferView::init(const Buffer& buffer) {
        MY_ASSERTION(
            buffer.getResourceType() == Buffer::Usage::IndexBuffer, "バッファの種類が違います。");

        mView.BufferLocation = buffer.getResource()->GetGPUVirtualAddress();
        mView.Format = calcFormatFromStride(buffer.getStride());
        mView.SizeInBytes = buffer.getSize();
    }
} // namespace Framework::DX
