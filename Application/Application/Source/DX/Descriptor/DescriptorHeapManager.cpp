#include "DescriptorHeapManager.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    DescriptorHeapManager::DescriptorHeapManager() {}
    DescriptorHeapManager::~DescriptorHeapManager() {}
    void DescriptorHeapManager::init(DeviceResource* device) {
        mRaytracingDescriptor.init(device);
        mGlobalHeap.init(device->getDevice(),
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
            GLOBAL_RESOURCE_HEAP_SIZE);
        mCbvSrvUavAllocator.init(device,
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
            RESOURCE_VIEW_ALLOCATE_SIZE);
        mDefaultResourceInfo = mCbvSrvUavAllocator.allocate();

        mSamplerAllocator.init(device,
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
            SAMPLER_VIEW_ALLOCATOR_SIZE);
        mDefaultSamplerInfo = mSamplerAllocator.allocate();

        mRtvAllocator.init(
            device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_ALLOCATOR_SIZE);
        mDsvAllocator.init(
            device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_ALLOCATOR_SIZE);
    }
    DescriptorInfo DescriptorHeapManager::allocate(DescriptorHeapType flag) {
        switch (flag) {
        case DescriptorHeapType::CbvSrvUav: return mCbvSrvUavAllocator.allocate();
        case DescriptorHeapType::Sampler: return mSamplerAllocator.allocate();
        case DescriptorHeapType::Rtv: return mRtvAllocator.allocate();
        case DescriptorHeapType::Dsv: return mDsvAllocator.allocate();
        case DescriptorHeapType::RaytracingGlobal: return mRaytracingDescriptor.allocateGlobal();
        case DescriptorHeapType::RaytracingLocal: return mRaytracingDescriptor.allocateLocal();
        default: MY_ASSERTION(false, "未対応のフラグが指定されました。"); return DescriptorInfo();
        }
    }
    void DescriptorHeapManager::beginFrame() {
        mRaytracingDescriptor.mHeap.resetGlobal();
        mGlobalHeap.reset();
    }
    void DescriptorHeapManager::copyAndSetComputeDescriptorHeap(DescriptorHeapType type,
        DeviceResource* device, ID3D12GraphicsCommandList* commandList, const DescriptorSet& set) {
        auto setDescriptorSetSingle
            = [&](const D3D12_CPU_DESCRIPTOR_HANDLE* start, UINT num, UINT index) {
                  if (num == 0) return;
                  MY_THROW_IF_FALSE_LOG(
                      mGlobalHeap.canCopy(num), "確保したディスクリプタの大きさを超えました");
                  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> tmpHandle(num);
                  for (UINT n = 0; n < num; n++) {
                      tmpHandle[n] = start[n].ptr == 0 ? mDefaultResourceInfo.cpuHandle : start[n];
                  }
                  CD3DX12_CPU_DESCRIPTOR_HANDLE dstHandle(
                      mGlobalHeap.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                      mGlobalHeap.mAllocatedNum, mGlobalHeap.mDescriptorHeapSize);

                  device->getDevice()->CopyDescriptors(
                      1, &dstHandle, &num, num, tmpHandle.data(), nullptr, mGlobalHeap.mType);

                  commandList->SetGraphicsRootDescriptorTable(index,
                      CD3DX12_GPU_DESCRIPTOR_HANDLE(
                          mGlobalHeap.mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                          mGlobalHeap.mAllocatedNum, mGlobalHeap.mDescriptorHeapSize));
                  mGlobalHeap.mAllocatedNum += num;
              };

        switch (type) {
        case DescriptorHeapType::CbvSrvUav:
            setDescriptorSetSingle(set.getCbvHandle().handle.data(), set.getCbvHandle().max, 0);
            setDescriptorSetSingle(set.getSrvHandle().handle.data(), set.getSrvHandle().max, 1);
            setDescriptorSetSingle(set.getUavHandle().handle.data(), set.getUavHandle().max, 2);
            break;
        case Framework::DX::DescriptorHeapType::RaytracingGlobal:
            mRaytracingDescriptor.copyAndSetComputeDescriptorTable(device, commandList, set);
            break;
        default: MY_ASSERTION(false, "未対応のフラグが指定されました。");
        }
    }

    ID3D12DescriptorHeap* DescriptorHeapManager::getHeapFromType(DescriptorHeapType type) {
        switch (type) {
        case DescriptorHeapType::CbvSrvUav: return mGlobalHeap.getHeap();
        case DescriptorHeapType::RaytracingGlobal:
            return mRaytracingDescriptor.mHeap.mDescriptorHeap.Get();
        default: MY_ASSERTION(false, "未対応のフラグが指定されました。"); return nullptr;
        }
    }
} // namespace Framework::DX
