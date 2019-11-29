#include "IndexBuffer.h"
#include "DX/Util/Helper.h"

namespace Framework::DX {
    IndexBuffer::IndexBuffer(ID3D12Device* device, const std::vector<Index>& indices,
        D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, const std::wstring& name)
        : mIndexCount(static_cast<UINT>(indices.size())), mTopologyType(topologyType) {
        mResource = createUploadBuffer(device, mIndexCount * sizeof(Index), name);
        writeToResource(mResource.Get(), indices.data(), mIndexCount * sizeof(Index));
    }
    IndexBuffer::~IndexBuffer() {}
    void IndexBuffer::createSRV(ID3D12Device* device) {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        //Indexは2Byteで確保しているので生データとして扱う
        srvDesc.Buffer.NumElements = mIndexCount * sizeof(Index) / sizeof(float);
        srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
        srvDesc.Buffer.StructureByteStride = 0;
        device->CreateShaderResourceView(mResource.Get(), &srvDesc, mCPUHandle);
    }
} // namespace Framework::DX
