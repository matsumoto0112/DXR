#include "VertexBuffer.h"

namespace Framework::DX {

    void VertexBuffer::setCommandList(ID3D12GraphicsCommandList* commandList) {
        commandList->IASetVertexBuffers(0, 1, &mView.getView());
    }
} // namespace Framework::DX
