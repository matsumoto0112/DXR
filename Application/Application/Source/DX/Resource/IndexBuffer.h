/**
 * @file IndexBuffer.h
 * @brief インデックスバッファ
 */

#pragma once
#include "DX/Resource/IBuffer.h"

namespace Framework::DX {
    using Index = UINT16;
    /**
     * @class IndexBuffer
     * @brief インデックスバッファ
     */
    class IndexBuffer : public IBuffer {
    public:
        /**
         * @brief コンストラクタ
         * @param device デバイス
         * @param indices インデックス配列
         * @param topologyType プリミティブの種類
         * @param name リソース名
         */
        IndexBuffer(ID3D12Device* device, const std::vector<Index>& indices,
            D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, const std::wstring& name);
        /* *
         * @brief デストラクタ
         */
        ~IndexBuffer();
        /**
         * @brief シェーダーリソースビューを作成する
         * @param device デバイス
         */
        void createSRV(ID3D12Device* device);

    private:
        UINT mIndexCount; //!< インデックス数
        D3D12_PRIMITIVE_TOPOLOGY_TYPE mTopologyType; //!< プリミティブの種類
    };
} // namespace Framework::DX
