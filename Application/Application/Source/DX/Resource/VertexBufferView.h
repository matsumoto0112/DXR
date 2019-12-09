/**
 * @file VertexBufferView.h
 * @brief 頂点バッファビュー
 */

#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class VertexBufferView
     * @brief 頂点バッファビュー
     */
    class VertexBufferView {
    public:
        /**
         * @brief コンストラクタ
         */
        VertexBufferView() {}
        /**
         * @brief デストラクタ
         */
        ~VertexBufferView() {}
        /**
         * @brief 初期化
         */
        void init(const Buffer& buffer);
        /**
         * @brief ビューの取得
         */
        const D3D12_VERTEX_BUFFER_VIEW& getView() const {
            return mView;
        }

    private:
        D3D12_VERTEX_BUFFER_VIEW mView;
    };
} // namespace Framework::DX
