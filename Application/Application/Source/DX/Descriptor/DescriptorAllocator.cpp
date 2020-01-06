#include "DescriptorAllocator.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {
    //コンストラクタ
    DescriptorAllocator::DescriptorAllocator() : mDevice(nullptr), mHeapNum(0) {}
    //デストラクタ
    DescriptorAllocator::~DescriptorAllocator() {
        mDevice = nullptr;
        mStackHeaps.clear();
    }
    //初期化
    void DescriptorAllocator::init(
        DeviceResource* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum) {
        mDevice = device;
        mHeapType = type;
        mHeapNum = descriptorNum;

        //最初に一つ追加する
        //足りなくなったらその都度追加する
        addHeap();
    }
    //ハンドルの確保
    DescriptorInfo DescriptorAllocator::allocate() {
        DescriptorInfo result;
        result.parent = this;
        //アロケートできたらそれを返す
        if (mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle)) return result;
        //確保できなければ新しく作成しアロケートする
        addHeap();
        mStackHeaps.back().allocate(&result.cpuHandle, &result.gpuHandle);
        return result;
    }
    //ヒープの追加
    void DescriptorAllocator::addHeap() {
        LocalDescriptorHeap newHeap;
        newHeap.init(mDevice->getDevice(), mHeapNum, mHeapType);
        mStackHeaps.emplace_back(newHeap);
    }
} // namespace Framework::DX
