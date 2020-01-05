#pragma once
#include "DX/Descriptor/DescriptorHeapFlag.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Raytracing/RaytracingDescriptorManager.h"

namespace Framework::DX {
    /**
     * @class DescriptorHeapManager
     * @brief ディスクリプタヒープ管理
     */
    class DescriptorHeapManager {
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
         * @brief ディスクリプタヒープのセット
         * @param commandList コマンドリスト
         * @param types セットするヒープの種類
         * @details typesはDescriptorHeapTypeしか受け付けない
         */
        template <class... Types>
        auto setDescriptorHeap(ID3D12GraphicsCommandList* commandList, Types... types) -> decltype(
            std::initializer_list<DescriptorHeapType>{ types... }, std::declval<void>());
        /**
         * @brief ディスクリプタのコピーをし、コンピュートシェーダーにセットする
         */
        void copyAndSetComputeDescriptorHeap(DescriptorHeapType type, DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& set);

    private:
        /**
         * @brief ヒープの種類からディスクリプタヒープを取得する
         */
        ID3D12DescriptorHeap* getHeapFromType(DescriptorHeapType type);

    private:
        RaytracingDescriptorManager mRaytracingDescriptor;
    };
    template <class... Types>
    inline auto DescriptorHeapManager::setDescriptorHeap(
        ID3D12GraphicsCommandList* commandList, Types... types)
        -> decltype(std::initializer_list<DescriptorHeapType>{ types... }, std::declval<void>()) {
        //たかだか5個しかないので予約はしないで追加していく
        std::vector<ID3D12DescriptorHeap*> heaps;
        for (auto&& type : { types... }) { heaps.emplace_back(getHeapFromType(type)); }
        commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
    }
} // namespace Framework::DX
