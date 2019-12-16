/**
 * @file Buffer.h
 * @brief バッファクラス
 */

#pragma once
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class Buffer
     * @brief バッファクラス
     */
    class Buffer {
    public:
        /**
         * @enum Usage
         * @brief バッファの用途
         */
        enum class Usage {
            ConstantBuffer,
            VertexBuffer,
            IndexBuffer,
            ShaderResource,
        };

    public:
        /**
         * @brief コンストラクタ
         */
        Buffer() {}
        /**
         * @brief デストラクタ
         */
        ~Buffer() {}
        /**
         * @brief 通常のバッファとして初期化
         */
        void init(DeviceResource* device, Usage usage, UINT64 size, UINT stride,
            const std::wstring& name);
        void init(DeviceResource* device, const Desc::TextureDesc& desc,
            const D3D12_CLEAR_VALUE* clearValue = nullptr);
        /**
         * @brief リソースから初期化
         */
        void init(Comptr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state);
        /**
         * @brief メモリのマップ処理
         * @details メモリの書き込み領域の先頭を返す
         */
        void* map();
        /**
         * @brief メモリのアンマップ
         */
        void unmap();
        /**
         * @brief リソースの書き込み
         */
        void writeResource(const void* data, UINT64 size);
        /**
         * @brief リソースの状態を移行する
         */
        void transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState);
        /**
         * @brief リソースを取得する
         */
        ID3D12Resource* getResource() const {
            return mResource.Get();
        }
        /**
         * @brief リソースの種類を取得する
         */
        Usage getResourceType() const {
            return mResourceType;
        }
        /**
         * @brief 現在のリソースの状態を取得する
         */
        D3D12_RESOURCE_STATES getCurrentState() const {
            return mCurrentState;
        }
        /**
         * @brief リソースのメモリサイズを取得する
         */
        UINT64 getSize() const {
            return mSize;
        }
        /**
         * @brief リソースのストライドを取得する
         */
        UINT getStride() const {
            return mStride;
        }

    private:
        Comptr<ID3D12Resource> mResource;
        Usage mResourceType = Usage::ConstantBuffer;
        D3D12_RESOURCE_STATES mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
        UINT64 mSize = 0;
        UINT mStride = 0;
    };
} // namespace Framework::DX
