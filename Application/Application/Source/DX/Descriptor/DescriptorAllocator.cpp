#include "DescriptorAllocator.h"

namespace Framework::DX {
    //初期化
    void DescriptorAllocator::init(
        ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum) {
        mHeapType = type;
        mHeapNum = descriptorNum;
        addHeap(device);
    }
    //ハンドルの確保
    DescriptorInfo DescriptorAllocator::allocate(ID3D12Device* device) {
        DescriptorInfo result;
        result.parent = this;
        //アロケートできたらそれを返す
        if (mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle)) return result;
        //確保できなければ新しく作成しアロケートする
        addHeap(device);
        mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle);
        return result;
    }
    //ヒープの追加
    void DescriptorAllocator::addHeap(ID3D12Device* device) {
        LocalDescriptorHeap newHeap;
        newHeap.init(device, mHeapNum, mHeapType);
        mStackHeaps.emplace_back(newHeap);
    }
} // namespace Framework::DX
