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
         * @brief ������
         */
        void init(DeviceResource* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum,
            float localDescriptorRate = 0.2f);
        DescriptorInfo allocateLocal();
        void resetGlobal();
        //private:
        Comptr<ID3D12DescriptorHeap> mDescriptorHeap; //!< �q�[�v
        UINT mDescriptorHeapSize = 0; //!< �q�[�v�T�C�Y
        UINT mGlobalAllocatedCount = 0;
        UINT mLocalHeapStartPosition = 0;
        UINT mLocalAllocatedCount = 0;
        D3D12_DESCRIPTOR_HEAP_TYPE mType = {};
    };
} // namespace Framework::DX
