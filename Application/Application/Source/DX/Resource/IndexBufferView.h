/**
 * @file IndexBufferView.h
 * @brief インデックスバッファビュー
 */

#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class IndexBufferView
     * @brief インデックスバッファビュー
     */
    class IndexBufferView {
    public:
        /**
         * @brief コンストラクタ
         */
        IndexBufferView() {}
        /**
         * @brief デストラクタ
         */
        ~IndexBufferView() {}
        /**
         * @brief 初期化処理
         */
        void init(const Buffer& buffer);
        /**
         * @brief ビューの取得
         */
        const D3D12_INDEX_BUFFER_VIEW& getView() const {
            return mView;
        }

    private:
        D3D12_INDEX_BUFFER_VIEW mView = {}; //!< ビュー
    };
} // namespace Framework::DX
