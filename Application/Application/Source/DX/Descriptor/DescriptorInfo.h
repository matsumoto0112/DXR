#pragma once

namespace Framework::DX {
    class DescriptorAllocator;
    /**
     * @class DescriptorInfo
     * @brief discription
     */
    struct DescriptorInfo {
        DescriptorAllocator* parent;
        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
    };
} // namespace Framework::DX