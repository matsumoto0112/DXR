#pragma once
#include "DX/Descriptor/DescriptorInfo.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RaytracingDescriptorHeap
     * @brief discription
     */
    class RaytracingDescriptorHeap {
    public:
        /**
         * @brief
         */
        RaytracingDescriptorHeap() {}
        /**
         * @brief
         */
        ~RaytracingDescriptorHeap() {}
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum,
            float localDescriptorRate = 0.2f);
        DescriptorInfo allocateLocal();
        void resetGlobal();
        //private:
        Comptr<ID3D12DescriptorHeap> mDescriptorHeap; //!< ヒープ
        UINT mDescriptorHeapSize = 0; //!< ヒープサイズ
        UINT mGlobalAllocatedCount = 0;
        UINT mLocalHeapStartPosition = 0;
        UINT mLocalAllocatedCount = 0;
        D3D12_DESCRIPTOR_HEAP_TYPE mType = {};
    };
} // namespace Framework::DX
