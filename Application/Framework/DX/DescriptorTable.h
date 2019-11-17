#pragma once
#include <d3d12.h>
#include "Desc/DescriptorTableDesc.h"
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"

namespace Framework::DX {
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
        * @param desc デーブルディスク
        */
        void create(ID3D12Device* device, const Desc::DescriptorTableDesc& desc);
        /**
        * @brief ヒープのリセット
        */
        void reset();
        /**
        * @brief ディスクリプタヒープの取得
        */
        inline ID3D12DescriptorHeap* getHeap() const { return mHeap.Get(); }
        /**
        * @brief CPUハンドルの取得
        */
        CD3DX12_CPU_DESCRIPTOR_HANDLE getCPUHandle(UINT index);
        /**
        * @brief GPUハンドルの取得
        */
        CD3DX12_GPU_DESCRIPTOR_HANDLE getGPUHandle(UINT index);
    private:
        ComPtr<ID3D12DescriptorHeap> mHeap; //!< ディスクリプタヒープ
        UINT mDescriptorSize; //!< ディスクリプタのサイズ
    };
} //Framework::DX
