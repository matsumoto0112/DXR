#include "DescriptorAllocator.h"
#include "DX/Descriptor/DescriptorParameter.h"

namespace Framework::DX {

    void DescriptorAllocator::init(
        ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum) {
        mHeapType = type;
        mHeapNum = descriptorNum;
        addHeap(device);
    }
    DescriptorInfo DescriptorAllocator::allocate(ID3D12Device* device) {
        DescriptorInfo result;
        result.parent = this;
        //�A���P�[�g�ł����炻���Ԃ�
        if (mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle)) return result;
        //�m�ۂł��Ȃ���ΐV�����쐬���A���P�[�g����
        addHeap(device);
        mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle);
        return result;
    }
    void DescriptorAllocator::addHeap(ID3D12Device* device) {
        LocalDescriptorHeap newHeap;
        newHeap.init(device, mHeapNum, mHeapType);
        mStackHeaps.emplace_back(newHeap);
    }
} // namespace Framework::DX
