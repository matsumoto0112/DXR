#include "RaytracingDescriptorHeap.h"
#include "DX/DeviceResource.h"
#include "Math/MathUtility.h"

namespace Framework::DX {
    //‰Šú‰»
    void RaytracingDescriptorHeap::init(DeviceResource* device, D3D12_DESCRIPTOR_HEAP_TYPE type,
        UINT descriptorNum, float localDescriptorRate) {
        mType = type;

        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = descriptorNum;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type = mType;

        MY_THROW_IF_FAILED(
            device->getDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mDescriptorHeap)));

        mDescriptorHeapSize = device->getDevice()->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        localDescriptorRate = Math::MathUtil::clamp(localDescriptorRate, 0.0f, 1.0f);
        mLocalHeapStartPosition = descriptorNum * (1.0f - localDescriptorRate);
    }
    DescriptorInfo RaytracingDescriptorHeap::allocateLocal() {
        DescriptorInfo info = {};
        UINT position = mLocalAllocatedCount + mLocalHeapStartPosition;
        info.cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), position, mDescriptorHeapSize);
        info.gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
            mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), position, mDescriptorHeapSize);
        mLocalAllocatedCount++;
        return info;
    }
    void RaytracingDescriptorHeap::resetGlobal() {
        mGlobalAllocatedCount = 0;
    }
} // namespace Framework::DX
