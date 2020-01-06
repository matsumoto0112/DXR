#pragma once

namespace Framework::Desc {
    /**
     * @enum CullMode
     * @brief カリングモード
     */
    enum class CullMode {
        Back,
        Front,
        None,
    };
    /**
     * @enum FillMode
     * @brief 塗りつぶしモード
     */
    enum class FillMode {
        Solid,
        WireFrame,
    };

    /**
     * @brief DX12のカリングモードに変換する
     */
    static constexpr inline D3D12_CULL_MODE toCullMode(CullMode cull) {
        switch (cull) {
        case Framework::Desc::CullMode::Back: return D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
        case Framework::Desc::CullMode::Front: return D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
        default: return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
        }
    }

    /**
     * @brief DX12のフィルモードに変換する
     */
    static constexpr inline D3D12_FILL_MODE toFillMode(FillMode fill) {
        switch (fill) {
        case Framework::Desc::FillMode::Solid: return D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
        default: return D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
        }
    }

    /**
     * @brief ラスタライザディスク
     */
    struct RasterizerDesc : public D3D12_RASTERIZER_DESC {
        /**
         * @brief コンストラクタ
         * @param cull カリングの種類
         * @param fill フィルの種類
         */
        explicit RasterizerDesc(Desc::CullMode cull, Desc::FillMode fill) {
            CullMode = toCullMode(cull);
            FillMode = toFillMode(fill);
            FrontCounterClockwise = FALSE;
            DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
            DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
            SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
            DepthClipEnable = TRUE;
            MultisampleEnable = FALSE;
            AntialiasedLineEnable = FALSE;
            ForcedSampleCount = 0;
            ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        }

        /**
         * @brief コンストラクタ
         * @details 複雑な設定をしたいときに使用する
         */
        explicit RasterizerDesc(const D3D12_RASTERIZER_DESC& desc) : D3D12_RASTERIZER_DESC(desc) {}
    };
} // namespace Framework::Desc