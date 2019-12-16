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
    ShaderResourceView IndexBuffer::createSRV(DeviceResource* device,
        const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
        const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) {
        ShaderResourceView view;
        view.initAsRawBuffer(
            device, mBuffer, mIndexNum * mBuffer.getStride() / sizeof(float), cpuHandle, gpuHandle);
        return view;
    }
} // namespace Framework::DX
