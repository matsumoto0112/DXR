#pragma once
#include "DX/Descriptor/DescriptorAllocator.h"
#include "DX/Descriptor/GlobalDescriptorHeap.h"
#include "DX/Raytracing/RaytracingDescriptorHeap.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RaytracingDescriptorManager
     * @brief レイトレーシング用ディスクリプタ管理
     */
    class RaytracingDescriptorManager {
    public:
        /**
         * @brief コンストラクタ
         */
        RaytracingDescriptorManager() {}
        /**
         * @brief デストラクタ
         */
        ~RaytracingDescriptorManager() {}
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device);

        DescriptorInfo allocateGlobal();
        DescriptorInfo allocateLocal();
        void copyAndSetComputeDescriptorTable(DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& globalSet);
        //private:
        DeviceResource* mDeviceResource = nullptr;
        DescriptorAllocator mGlobalView; //!<グローバル用アロケータ
        RaytracingDescriptorHeap mHeap; //!<レイトレーシング専用ディスクリプタ

        DescriptorInfo mDefaultGlobalView;
    };
} // namespace Framework::DX
