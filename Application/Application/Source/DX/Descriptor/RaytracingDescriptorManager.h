#pragma once
#include "DX/Descriptor/DescriptorAllocator.h"
#include "DX/Descriptor/GlobalDescriptorHeap.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RaytracingDescriptorManager
     * @brief discription
     */
    class RaytracingDescriptorManager {
    public:
        /**
         * @brief
         */
        RaytracingDescriptorManager() {}
        /**
         * @brief
         */
        ~RaytracingDescriptorManager() {}
        void init(DeviceResource* device);
        DescriptorAllocator* getGlobalView() {
            return &mGlobalView;
        }
        DescriptorAllocator* getLocalView() {
            return &mLocalView;
        }
        void copyAndSetComputeDescriptorTable(DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& globalSet,
            const DescriptorSet& localSet);

    private:
        GlobalDescriptorHeap mGlobal;
        DescriptorAllocator mGlobalView;
        DescriptorAllocator mLocalView;

        DescriptorInfo mDefaultGlobalView;
        DescriptorInfo mDefaultLocalView;
    };
} // namespace Framework::DX
