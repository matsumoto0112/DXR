#include "RaytracingDescriptorManager.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void RaytracingDescriptorManager::init(DeviceResource* device) {
        mGlobal.init(device->getDevice(),
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10000);
        mGlobalView.init(device->getDevice(),
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10000);
        mLocalView.init(device->getDevice(),
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10000);

        mDefaultGlobalView = mGlobalView.allocate(device->getDevice());
        mDefaultLocalView = mLocalView.allocate(device->getDevice());
    }
    void RaytracingDescriptorManager::copyAndSetComputeDescriptorTable(DeviceResource* device,
        ID3D12GraphicsCommandList* commandList, const DescriptorSet& globalSet,
        const DescriptorSet& localSet) {
        auto setDescriptorSetSingle
            = [&](const D3D12_CPU_DESCRIPTOR_HANDLE* start, UINT num, UINT index, bool set) {
                  if (num == 0) return;
                  MY_THROW_IF_FALSE_LOG(
                      mGlobal.canCopy(num), "確保したディスクリプタの大きさを超えました");
                  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> tmpHandle(num);
                  for (UINT n = 0; n < num; n++) {
                      tmpHandle[n] = start[n].ptr == 0 ? mDefaultGlobalView.cpuHandle : start[n];
                  }
                  CD3DX12_CPU_DESCRIPTOR_HANDLE dstHandle(
                      mGlobal.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                      mGlobal.mAllocatedNum, mGlobal.mDescriptorHeapSize);

                  device->getDevice()->CopyDescriptors(
                      1, &dstHandle, &num, num, tmpHandle.data(), nullptr, mGlobal.mType);
                  if (set) {
                      commandList->SetGraphicsRootDescriptorTable(index,
                          CD3DX12_GPU_DESCRIPTOR_HANDLE(
                              mGlobal.mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                              mGlobal.mAllocatedNum, mGlobal.mDescriptorHeapSize));
                  }
                  mGlobal.mAllocatedNum += num;
              };

        setDescriptorSetSingle(
            globalSet.getCbvHandle().handle.data(), globalSet.getCbvHandle().max, 0, true);
        setDescriptorSetSingle(
            globalSet.getSrvHandle().handle.data(), globalSet.getSrvHandle().max, 1, true);
        setDescriptorSetSingle(
            globalSet.getUavHandle().handle.data(), globalSet.getUavHandle().max, 2, true);

        setDescriptorSetSingle(
            localSet.getCbvHandle().handle.data(), localSet.getCbvHandle().max, 0, false);
        setDescriptorSetSingle(
            localSet.getSrvHandle().handle.data(), localSet.getSrvHandle().max, 1, false);
        setDescriptorSetSingle(
            localSet.getUavHandle().handle.data(), localSet.getUavHandle().max, 2, false);
    }
} // namespace Framework::DX
