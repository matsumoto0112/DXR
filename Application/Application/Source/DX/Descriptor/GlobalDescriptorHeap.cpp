#include "GlobalDescriptorHeap.h"
#include "DX/Descriptor/DescriptorParameter.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Descriptor/LocalDescriptorHeap.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void GlobalDescriptorHeap::init(
        ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorHeapNum) {
        mType = type;

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = mType;
        heapDesc.NumDescriptors = descriptorHeapNum;
        heapDesc.NodeMask = 0;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        MY_THROW_IF_FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap)));
        mDescriptorHeapSize = device->GetDescriptorHandleIncrementSize(mType);
    }
} // namespace Framework::DX
