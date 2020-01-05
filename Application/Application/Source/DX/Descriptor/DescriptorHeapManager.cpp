#include "DescriptorHeapManager.h"

namespace Framework::DX {

    DescriptorHeapManager::DescriptorHeapManager() {}
    DescriptorHeapManager::~DescriptorHeapManager() {}
    void DescriptorHeapManager::init(DeviceResource* device) {
        mRaytracingDescriptor.init(device);
    }
    DescriptorInfo DescriptorHeapManager::allocate(DescriptorHeapType flag) {
        switch (flag) {
        case Framework::DX::DescriptorHeapType::RaytracingGlobal:
            return mRaytracingDescriptor.allocateGlobal();
        case Framework::DX::DescriptorHeapType::RaytracingLocal:
            return mRaytracingDescriptor.allocateLocal();
        default: MY_ASSERTION(false, "未対応のフラグが指定されました。"); return DescriptorInfo();
        }
    }
    void DescriptorHeapManager::beginFrame() {
        mRaytracingDescriptor.mHeap.resetGlobal();
    }
    void DescriptorHeapManager::copyAndSetComputeDescriptorHeap(DescriptorHeapType type,
        DeviceResource* device, ID3D12GraphicsCommandList* commandList, const DescriptorSet& set) {
        switch (type) {
        case Framework::DX::DescriptorHeapType::RaytracingGlobal:
            mRaytracingDescriptor.copyAndSetComputeDescriptorTable(device, commandList, set);
            break;
        default: MY_ASSERTION(false, "未対応のフラグが指定されました。");
        }
    }

    ID3D12DescriptorHeap* DescriptorHeapManager::getHeapFromType(DescriptorHeapType type) {
        switch (type) {
        case Framework::DX::DescriptorHeapType::RaytracingGlobal:
            return mRaytracingDescriptor.mHeap.mDescriptorHeap.Get();
        default: MY_ASSERTION(false, "未対応のフラグが指定されました。"); return nullptr;
        }
    }
} // namespace Framework::DX
