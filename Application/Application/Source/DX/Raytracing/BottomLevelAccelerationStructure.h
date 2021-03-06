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
         */
        BottomLevelAccelerationStructure() {}
        /**
         * @brief デストラクタ
         */
        ~BottomLevelAccelerationStructure() {}
        /**
         * @brief 初期化
         * @param device DXR用デバイス
         * @param vertexBuffer 頂点バッファ
         * @param vertexSize 頂点のバイトサイズ
         * @param indexBuffer インデックスバッファ
         * @param indexSize インデックスのバイトサイズ
         */
        void init(const DXRDevice& device, const VertexBuffer& vertexBuffer, UINT vertexSize,
            const IndexBuffer& indexBuffer, UINT indexSize);
        /**
         * @brief バッファの取得
         */
        ID3D12Resource* getBuffer() const {
            return mBuffer.Get();
        }

    private:
        Comptr<ID3D12Resource> mScratch;
        Comptr<ID3D12Resource> mBuffer;
    };
} // namespace Framework::DX
