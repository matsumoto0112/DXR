#pragma once
#include <d3d12.h>
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @enum HeapType
    * @brief ヒープの種類定義
    */
    enum class HeapType {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
    };

    /**
    * @enum HeapFlag
    * @brief ヒープのフラグ定義
    */
    enum class HeapFlag {
        None,
        ShaderVisible,
    };

    /**
    * @class DescriptorTable
    * @brief ディスクリプタヒープテーブル
    */
    class DescriptorTable {
    public:
        /**
        * @brief コンストラクタ
        */
        DescriptorTable();
        /**
        * @brief デストラクタ
        */
        ~DescriptorTable();
        /**
        * @brief ヒープの作成
        * @param device デバイス
        * @param heapType ヒープの種類
        * @param heapFlag ヒープフラグ
        * @param descriptorNum 割り当てるヒープの数
        * @param name ヒープ名
        */
        void create(ID3D12Device* device, HeapType heapType, HeapFlag heapFlag, UINT descriptorNum, LPCWSTR name = nullptr);
        /**
        * @brief ヒープのリセット
        */
        void reset();
        /**
        * @brief ディスクリプタヒープの取得
        */
        inline ID3D12DescriptorHeap* getHeap() const { return mHeap.Get(); }
        CD3DX12_CPU_DESCRIPTOR_HANDLE getCPUHandle(UINT index);
        /**
        * @brief
        */
        CD3DX12_GPU_DESCRIPTOR_HANDLE getGPUHandle(UINT index);
    private:
        ComPtr<ID3D12DescriptorHeap> mHeap; //!< ディスクリプタヒープ
        UINT mDescriptorSize; //!< ディスクリプタのサイズ
    };
} //Framework::DX
