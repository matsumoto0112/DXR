#include "VertexBuffer.h"

namespace Framework::DX {
    //デストラクタ
    VertexBuffer::~VertexBuffer() {}
    //シェーダーリソースビューを作成する
    void VertexBuffer::createSRV(ID3D12Device* device) {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Buffer.NumElements = mVertexCount;
        srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
        srvDesc.Buffer.StructureByteStride = mVertexByteLength;
        device->CreateShaderResourceView(mResource.Get(), &srvDesc, mCPUHandle);
    }

} // namespace Framework::DX
