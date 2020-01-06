/**
 * @file DescriptorAllocator.h
 * @brief ディスクリプタのアロケート管理
 */

#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Descriptor/LocalDescriptorHeap.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class DescriptorAllocator
     * @brief ディスクリプタの確保管理
     */
    class DescriptorAllocator {
    public:
        /**
         * @brief コンストラクタ
         */
        DescriptorAllocator();
        /**
         * @brief デストラクタ
         */
        ~DescriptorAllocator();
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum);
        /**
         * @brief ディスクリプタの確保
         */
        DescriptorInfo allocate();

    private:
        /**
         * @brief ヒープの追加
         */
        void addHeap();

    private:
        DeviceResource* mDevice; //!< デバイス
        std::vector<LocalDescriptorHeap> mStackHeaps; //!< 確保済みのヒープ
        D3D12_DESCRIPTOR_HEAP_TYPE mHeapType; //!< ヒープの種類k
        UINT mHeapNum; //!< ヒープの確保数
    };
} // namespace Framework::DX
