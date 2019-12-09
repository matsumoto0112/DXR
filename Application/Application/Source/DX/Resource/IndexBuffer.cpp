#include "IndexBuffer.h"

namespace Framework::DX {
    //�R�}���h���X�g�ɃZ�b�g����
    void IndexBuffer::setCommandList(ID3D12GraphicsCommandList* commandList) {
        commandList->IASetPrimitiveTopology(mTopology);
        commandList->IASetIndexBuffer(&mView.getView());
    }
    //�`�悷��
    void IndexBuffer::draw(ID3D12GraphicsCommandList* commandList) {
        commandList->DrawIndexedInstanced(mIndexNum, 1, 0, 0, 0);
    }
} // namespace Framework::DX
