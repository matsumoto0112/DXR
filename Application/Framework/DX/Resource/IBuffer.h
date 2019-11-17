#pragma once
#include <d3d12.h>
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @class IBuffer
    * @brief リソースバッファ基底クラス
    */
    class IBuffer {
    public:
        /**
        * @brief デストラクタ
        */
        virtual ~IBuffer() = default;
        /**
        * @brief リソースの取得
        */
        ID3D12Resource* getResource() const { return mResource.Get(); }
        /**
        * @brief CPUハンドルのセット
        */
        void setCPUHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle) { mCPUHandle = handle; }
        /**
        * @brief CPUハンドルの取得
        */
        D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle()const { return mCPUHandle; };
        /**
        * @brief GPUハンドルのセット
        */
        void setGPUHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle) { mGPUHandle = handle; }
        /**
        * @brief GPUハンドルの取得
        */
        D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle() const { return mGPUHandle; }
    //protected:
        ComPtr<ID3D12Resource> mResource; //!< リソース
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle; //!< CPUハンドル
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle; //!< GPUハンドル
    };
} //Framework::DX
