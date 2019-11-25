/**
 * @file CountingDescriptorTable.h
 * @brief カウンター付きディスクリプタテーブル
 */

#pragma once
#include "DX/DescriptorTable.h"
#include "DX/Resource/IBuffer.h"

namespace Framework::DX {
    /**
     * @class CountingDescriptorTable
     * @brief カウンター付きテーブル
     */
    class CountingDescriptorTable : public DescriptorTable {
    public:
        /**
         * @brief コンストラクタ
         */
        CountingDescriptorTable(ID3D12Device* device, const Desc::DescriptorTableDesc& desc);
        /**
         * @brief デストラクタ
         */
        ~CountingDescriptorTable();
        /**
         * @brief ヒープの確保
         */
        void allocate(IBuffer* buffer);
        /**
         * @brief カウンターのリセット
         */
        void resetCounter() { mIndex = 0; }

    private:
        UINT mIndex; //!< 現在のインデックス
        UINT mDescriptorNum; //!< 割り当て数
    };
} // namespace Framework::DX
