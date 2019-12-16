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
        GlobalDescriptorHeap* getLocalView() {
            return &mGlobal;
        }
        void copyAndSetComputeDescriptorTable(DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& globalSet);

        //private:
        GlobalDescriptorHeap mGlobal;
        DescriptorAllocator mGlobalView;

        DescriptorInfo mDefaultGlobalView;
    };
} // namespace Framework::DX
