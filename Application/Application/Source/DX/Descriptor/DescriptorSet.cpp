#include "DescriptorSet.h"
#include "Math/MathUtility.h"

namespace Framework::DX {
    void DescriptorSet::setCbvHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) {
        mCbvHandle.max = Math::MathUtil::mymax(mCbvHandle.max, index + 1);
        mCbvHandle.handle[index] = cpuHandle;
    }
    void DescriptorSet::setSrvHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) {
        mSrvHandle.max = Math::MathUtil::mymax(mSrvHandle.max, index + 1);
        mSrvHandle.handle[index] = cpuHandle;
    }
    void DescriptorSet::setUavHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) {
        mUavHandle.max = Math::MathUtil::mymax(mUavHandle.max, index + 1);
        mUavHandle.handle[index] = cpuHandle;
    }
    void DescriptorSet::setSamplerHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) {
        mSamplerHandle.max = Math::MathUtil::mymax(mSamplerHandle.max, index + 1);
        mSamplerHandle.handle[index] = cpuHandle;
    }
} // namespace Framework::DX
