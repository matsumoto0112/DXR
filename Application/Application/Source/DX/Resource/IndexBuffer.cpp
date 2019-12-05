#include "IndexBuffer.h"

namespace Framework::DX {
    //初期化
    void IndexBuffer::init(ID3D12Device* device, const std::vector<UINT>& indices,
        D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name) {
        mIndexNum = static_cast<UINT>(indices.size());
        mTopology = topology;
        mBuffer.init(
            device, Buffer::Usage::IndexBuffer, mIndexNum * sizeof(UINT), sizeof(UINT), name);
        mBuffer.writeResource(indices.data(), mIndexNum * sizeof(UINT));
        mView.init(&mBuffer);
    }
    //初期化
    void IndexBuffer::init(ID3D12Device* device, const std::vector<UINT16>& indices,
        D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name) {
        mIndexNum = static_cast<UINT>(indices.size());
        mTopology = topology;
        mBuffer.init(
            device, Buffer::Usage::IndexBuffer, mIndexNum * sizeof(UINT16), sizeof(UINT16), name);
        mBuffer.writeResource(indices.data(), mIndexNum * sizeof(UINT16));
        mView.init(&mBuffer);
    }
    //コマンドリストにセットする
    void IndexBuffer::setCommandList(ID3D12GraphicsCommandList* commandList) {
        commandList->IASetPrimitiveTopology(mTopology);
        commandList->IASetIndexBuffer(&mView.getView());
    }
    //描画する
    void IndexBuffer::draw(ID3D12GraphicsCommandList* commandList) {
        commandList->DrawIndexedInstanced(mIndexNum, 1, 0, 0, 0);
    }
} // namespace Framework::DX
