#pragma once

namespace Framework::Desc {
    /**
     * @brief ヒープの種類
     */
    enum class HeapType {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
    };

    /**
     * @brief ヒープのフラグ
     */
    enum class HeapFlag {
        None,
        ShaderVisible,
    };

    /**
     * @brief ディスクリプタヒープのデスク
     */
    struct DescriptorHeapDesc {
        UINT descriptorNum; //<! 割り当てる数
        HeapType type; //!< 種類
        HeapFlag flags; //!< フラグ
        UINT mask; //!< マスク
        /**
         * @brief コンストラクタ
         */
        DescriptorHeapDesc() : DescriptorHeapDesc(0, HeapType::CBV_SRV_UAV, HeapFlag::None, 0) {}
        /**
         * @brief コンストラクタ
         */
        DescriptorHeapDesc(
            UINT num, HeapType type, HeapFlag flags = HeapFlag::ShaderVisible, UINT mask = 0)
            : descriptorNum(num), type(type), flags(flags), mask(mask) {}
    };
} // namespace Framework::Desc