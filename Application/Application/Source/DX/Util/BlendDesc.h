#pragma once

namespace Framework::Desc {
    /**
     * @brief ブレンドの種類定義
     */
    enum class BlendMode {
        Default, //!< ブレンドなし
        Add, //!< 加算合成
        Substract, //!< 減算合成
        Multiple, //!< 積算合成
        Alignment, //!< 線形合成

        Custom, //!< カスタム
    };

    /**
     * @brief デフォルトのブレンドディスクを取得する
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC defaultBlendDesc() {
        return { FALSE, FALSE, D3D12_BLEND::D3D12_BLEND_ONE, D3D12_BLEND::D3D12_BLEND_ZERO,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief 加算合成ディスクを取得する
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC addBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_SRC_ALPHA, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief 減算合成ディスクを取得する
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC substractBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_SRC_ALPHA, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND_OP::D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief 積算合成ディスクを取得する
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC multipleBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND::D3D12_BLEND_SRC_COLOR,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief 線形合成ディスクを取得する
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC alignmentBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_SRC_ALPHA,
            D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_BLEND::D3D12_BLEND_ONE, D3D12_BLEND::D3D12_BLEND_ZERO,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief ブレンドモードからディスクを取得する
     */
    static constexpr D3D12_RENDER_TARGET_BLEND_DESC fromBlendMode(BlendMode mode) {
        //カスタムの場合はデフォルトと同じものを返す
        switch (mode) {
        case Framework::Desc::BlendMode::Add: return addBlendDesc();
        case Framework::Desc::BlendMode::Substract: return substractBlendDesc();
        case Framework::Desc::BlendMode::Multiple: return multipleBlendDesc();
        case Framework::Desc::BlendMode::Alignment: return alignmentBlendDesc();
        default: return defaultBlendDesc();
        }
    }

    /**
     * @brief ブレンドディスク構造体
     */
    struct BlendDesc : public D3D12_BLEND_DESC {
        /**
         * @brief コンストラクタ
         * @param mode ブレンドの種類
         * @details modeからブレンドディスクを生成する
         */
        explicit BlendDesc(BlendMode mode) {
            AlphaToCoverageEnable = FALSE;
            IndependentBlendEnable = FALSE;

            D3D12_RENDER_TARGET_BLEND_DESC desc = fromBlendMode(mode);
            for (size_t n = 0; n < _countof(RenderTarget); n++) { RenderTarget[n] = desc; }
        }

        /**
         * @brief コンストラクタ
         */
        explicit BlendDesc(const D3D12_BLEND_DESC& desc) : D3D12_BLEND_DESC(desc) {}
    };
} // namespace Framework::Desc