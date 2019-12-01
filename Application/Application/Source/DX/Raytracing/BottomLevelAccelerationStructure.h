/**
 * @file BottomLevelAccelerationStructure.h
 * @brief ボトムレベルのAS
 * @details ジオメトリの形状定義
 */

#pragma once
#include "DX/Raytracing/DXRDevice.h"
#include "DX/Resource/IndexBuffer.h"
#include "DX/Resource/VertexBuffer.h"

namespace Framework::DX {
    /**
     * @class BottomLevelAccelerationStructure
     * @brief ジオメトリの形状を定義するAS
     */
    class BottomLevelAccelerationStructure {
    public:
        /**
         * @brief コンストラクタ
         * @param device DXR用デバイス
         * @param vertexBuffer 頂点バッファ
         * @param vertexSize 頂点のバイトサイズ
         * @param indexBuffer インデックスバッファ
         * @param indexSize インデックスのバイトサイズ
         */
        BottomLevelAccelerationStructure(const DXRDevice& device, const VertexBuffer& vertexBuffer,
            UINT vertexSize, const IndexBuffer& indexBuffer, UINT indexSize);
        /**
         * @brief デストラクタ
         */
        ~BottomLevelAccelerationStructure();
        /**
         * @brief バッファの取得
         */
        ID3D12Resource* getBuffer() const {
            return mBuffer.Get();
        }

    private:
        ComPtr<ID3D12Resource> mScratch;
        ComPtr<ID3D12Resource> mBuffer;
    };
} // namespace Framework::DX
