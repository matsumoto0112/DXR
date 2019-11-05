#include "GPUUploadBuffer.h"
#include "Utility/Debug.h"

namespace Framework::DX {
    //コンストラクタ
    GPUUploadBuffer::GPUUploadBuffer() { }
    //デストラクタ
    GPUUploadBuffer::~GPUUploadBuffer() {
        if (mResource) {
            mResource->Unmap(0, nullptr);
        }
    }
    //アロケート
    void GPUUploadBuffer::allocate(ID3D12Device* device, UINT bufferSize, LPCWSTR resourceName) {
        CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

        MY_THROW_IF_FAILED(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mResource)));
        mResource->SetName(resourceName);
    }
    //CPU書き込み範囲を返す
    uint8_t* GPUUploadBuffer::getMapCPUWriteOnly() {
        uint8_t* mapped;
        CD3DX12_RANGE readRange(0, 0);
        MY_THROW_IF_FAILED(mResource->Map(0, &readRange, reinterpret_cast<void**>(&mapped)));
        return mapped;
    }
} //Framework::DX
