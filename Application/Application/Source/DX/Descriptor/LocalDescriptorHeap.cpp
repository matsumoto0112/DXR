#include "LocalDescriptorHeap.h"

namespace Framework::DX {

    void LocalDescriptorHeap::init(
        ID3D12Device* device, UINT descriptorNum, D3D12_DESCRIPTOR_HEAP_TYPE type) {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = type;
        heapDesc.NumDescriptors = descriptorNum;
        heapDesc.NodeMask = 0;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        MY_THROW_IF_FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap)));
        mDescriptorHeapSize = device->GetDescriptorHandleIncrementSize(type);
        mDescriptorHeapNum = descriptorNum;
    }
    bool LocalDescriptorHeap::allocate(
        D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) {
        if (mCurrentAllocatedNum + 1 > mDescriptorHeapNum) return false;
        *cpuHandle
            = CD3DX12_CPU_DESCRIPTOR_HANDLE(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                mCurrentAllocatedNum, mDescriptorHeapSize);
        *gpuHandle
            = CD3DX12_GPU_DESCRIPTOR_HANDLE(mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                mCurrentAllocatedNum, mDescriptorHeapSize);
        mCurrentAllocatedNum++;
        return true;
    }
} // namespace Framework::DX
