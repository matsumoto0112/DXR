#include "DescriptorAllocator.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {
    //�R���X�g���N�^
    DescriptorAllocator::DescriptorAllocator() : mDevice(nullptr), mHeapNum(0) {}
    //�f�X�g���N�^
    DescriptorAllocator::~DescriptorAllocator() {
        mDevice = nullptr;
        mStackHeaps.clear();
    }
    //������
    void DescriptorAllocator::init(
        DeviceResource* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum) {
        mDevice = device;
        mHeapType = type;
        mHeapNum = descriptorNum;

        //�ŏ��Ɉ�ǉ�����
        //����Ȃ��Ȃ����炻�̓s�x�ǉ�����
        addHeap();
    }
    //�n���h���̊m��
    DescriptorInfo DescriptorAllocator::allocate() {
        DescriptorInfo result;
        result.parent = this;
        //�A���P�[�g�ł����炻���Ԃ�
        if (mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle)) return result;
        //�m�ۂł��Ȃ���ΐV�����쐬���A���P�[�g����
        addHeap();
        mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle);
        return result;
    }
    //�q�[�v�̒ǉ�
    void DescriptorAllocator::addHeap() {
        LocalDescriptorHeap newHeap;
        newHeap.init(mDevice->getDevice(), mHeapNum, mHeapType);
        mStackHeaps.emplace_back(newHeap);
    }
} // namespace Framework::DX
