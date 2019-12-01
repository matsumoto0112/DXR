#pragma once

namespace Framework::DX {
    inline ComPtr<ID3D12Resource> createUploadBuffer(
        ID3D12Device* device, UINT64 size, const std::wstring& name) {
        ComPtr<ID3D12Resource> resource;
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &bufferDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&resource)));
        MY_THROW_IF_FAILED(resource->SetName(name.c_str()));

        return resource;
    }

    inline void writeToResource(ID3D12Resource* resource, const void* data, size_t size) {
        void* mapped;
        MY_THROW_IF_FAILED(resource->Map(0, nullptr, &mapped));
        memcpy(mapped, data, size);
        resource->Unmap(0, nullptr);
    }

    inline ComPtr<ID3D12Resource> createUAVBuffer(ID3D12Device* device, UINT64 size,
        D3D12_RESOURCE_STATES initResourceState, const std::wstring& name) {
        ComPtr<ID3D12Resource> result;
        CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(
            size, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &bufferDesc, initResourceState, nullptr, IID_PPV_ARGS(&result)));
        result->SetName(name.c_str());

        return result;
    };

} // namespace Framework::DX
