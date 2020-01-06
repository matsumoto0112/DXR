#pragma once
#include "DX/Descriptor/DescriptorHeapFlag.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Descriptor/GlobalDescriptorHeap.h"
#include "DX/Raytracing/RaytracingDescriptorHeapManager.h"

namespace Framework::DX {
    /**
     * @class DescriptorHeapManager
     * @brief ディスクリプタヒープ管理
     */
    class DescriptorHeapManager {
    private:
        static constexpr UINT GLOBAL_RESOURCE_HEAP_SIZE = 100000;
        static constexpr UINT GLOBAL_SAMPLER_HEAP_SIZE = 1000;
        static constexpr UINT RESOURCE_VIEW_ALLOCATE_SIZE = 2000;
        static constexpr UINT SAMPLER_VIEW_ALLOCATOR_SIZE = 1000;
        static constexpr UINT RTV_ALLOCATOR_SIZE = 10;
        static constexpr UINT DSV_ALLOCATOR_SIZE = 5;

    public:
        /**
         * @brief コンストラクタ
         */
        DescriptorHeapManager();
        /**
         * @brief デストラクタ
         */
        ~DescriptorHeapManager();
        /**
         * @brief 初期化
         * @param device デバイス
         */
        void init(DeviceResource* device);
        /**
         * @brief ヒープのアロケート
         * @param type ヒープの種類
         */
        DescriptorInfo allocate(DescriptorHeapType type);
        /**
         * @brief フレーム開始時処理を行う
         */
        void beginFrame();
        /**
         * @brief ディスクリプタのコピーをし、コンピュートシェーダーにセットする
         */
        void copyAndSetGraphicsDescriptorHeap(DescriptorHeapType type, DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& set);
        /**
         * @brief ディスクリプタのコピーをし、コンピュートシェーダーにセットする
         */
        void copyAndSetComputeDescriptorHeap(DescriptorHeapType type, DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& set);
        /**
         * @brief ディスクリプタヒープのセット
         * @param commandList コマンドリスト
         * @param types セットするヒープの種類
         * @details typesはDescriptorHeapTypeしか受け付けない
         */
        template <class... Types>
        void setDescriptorHeap(ID3D12GraphicsCommandList* commandList, Types... types);

    private:
        /**
         * @brief ヒープの種類からディスクリプタヒープを取得する
         */
        ID3D12DescriptorHeap* getHeapFromType(DescriptorHeapType type);

    private:
        GlobalDescriptorHeap mGlobalHeap;
        GlobalDescriptorHeap mSamplerHeap;
        DescriptorAllocator mCbvSrvUavAllocator;
        DescriptorAllocator mSamplerAllocator;
        DescriptorAllocator mRtvAllocator;
        DescriptorAllocator mDsvAllocator;
        DescriptorInfo mDefaultResourceInfo;
        DescriptorInfo mDefaultSamplerInfo;
        RaytracingDescriptorHeapManager mRaytracingDescriptor;
    };
    template <class... Types>
    inline void DescriptorHeapManager::setDescriptorHeap(
        ID3D12GraphicsCommandList* commandList, Types... types) {
        //たかだか5個しかないので予約はしないで追加していく
        std::vector<ID3D12DescriptorHeap*> heaps;
        for (auto&& type : { types... }) { heaps.emplace_back(getHeapFromType(type)); }
        commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
    }
} // namespace Framework::DX
