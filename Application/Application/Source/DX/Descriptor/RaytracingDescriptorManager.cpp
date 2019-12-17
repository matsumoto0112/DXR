#include "RaytracingDescriptorManager.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {

    void RaytracingDescriptorManager::init(DeviceResource* device) {
        mDeviceResource = device;
        mHeap.init(
            device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10000);
        mGlobalView.init(device->getDevice(),
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10000);

        mDefaultGlobalView = mGlobalView.allocate(device->getDevice());
    }
    DescriptorInfo RaytracingDescriptorManager::allocateGlobal() {
        return mGlobalView.allocate(mDeviceResource->getDevice());
    }
    DescriptorInfo RaytracingDescriptorManager::allocateLocal() {
        return mHeap.allocateLocal();
    }
    void RaytracingDescriptorManager::copyAndSetComputeDescriptorTable(DeviceResource* device,
        ID3D12GraphicsCommandList* commandList, const DescriptorSet& globalSet) {
        auto setDescriptorSetSingle
            = [&](const D3D12_CPU_DESCRIPTOR_HANDLE* start, UINT num, UINT index) {
                  if (num == 0) return;
                  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> tmpHandle(num);
                  for (UINT n = 0; n < num; n++) {
                      tmpHandle[n] = start[n].ptr == 0 ? mDefaultGlobalView.cpuHandle : start[n];
                  }
                  CD3DX12_CPU_DESCRIPTOR_HANDLE dstHandle(
                      mHeap.mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                      mHeap.mGlobalAllocatedCount, mHeap.mDescriptorHeapSize);

                  device->getDevice()->CopyDescriptors(
                      1, &dstHandle, &num, num, tmpHandle.data(), nullptr, mHeap.mType);
                  commandList->SetComputeRootDescriptorTable(index,
                      CD3DX12_GPU_DESCRIPTOR_HANDLE(
                          mHeap.mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                          mHeap.mGlobalAllocatedCount, mHeap.mDescriptorHeapSize));
                  mHeap.mGlobalAllocatedCount += num;
              };

        setDescriptorSetSingle(
            globalSet.getCbvHandle().handle.data(), globalSet.getCbvHandle().max, 0);
        setDescriptorSetSingle(
            globalSet.getSrvHandle().handle.data(), globalSet.getSrvHandle().max, 1);
        setDescriptorSetSingle(
            globalSet.getUavHandle().handle.data(), globalSet.getUavHandle().max, 2);
    }
} // namespace Framework::DX
