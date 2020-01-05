#pragma once

namespace Framework::DX {
    enum class DescriptorHeapFlag : UINT {
        UseNormalHeap = 1 << 0,
        UseRaytracingGlobalHeap = UseNormalHeap << 1,
        UseRaytracingLocalHeap = UseRaytracingGlobalHeap << 1,
    };

    constexpr DescriptorHeapFlag operator|(DescriptorHeapFlag f1, DescriptorHeapFlag f2) {
        return static_cast<DescriptorHeapFlag>(static_cast<UINT>(f1) | static_cast<UINT>(f2));
    }
} // namespace Framework::DX
