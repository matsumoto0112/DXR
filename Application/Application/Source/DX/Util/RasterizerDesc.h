#pragma once

namespace Framework::Desc {
    /**
     * @enum CullMode
     * @brief �J�����O���[�h
     */
    enum class CullMode {
        Back,
        Front,
        None,
    };
    /**
     * @enum FillMode
     * @brief �h��Ԃ����[�h
     */
    enum class FillMode {
        Solid,
        WireFrame,
    };

    /**
     * @brief DX12�̃J�����O���[�h�ɕϊ�����
     */
    static constexpr inline D3D12_CULL_MODE toCullMode(CullMode cull) {
        switch (cull) {
        case Framework::Desc::CullMode::Back: return D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
        case Framework::Desc::CullMode::Front: return D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
        default: return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
        }
    }

    /**
     * @brief DX12�̃t�B�����[�h�ɕϊ�����
     */
    static constexpr inline D3D12_FILL_MODE toFillMode(FillMode fill) {
        switch (fill) {
        case Framework::Desc::FillMode::Solid: return D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
        default: return D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
        }
    }

    /**
     * @brief ���X�^���C�U�f�B�X�N
     */
    struct RasterizerDesc : public D3D12_RASTERIZER_DESC {
        /**
         * @brief �R���X�g���N�^
         * @param cull �J�����O�̎��
         * @param fill �t�B���̎��
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
         * @brief �R���X�g���N�^
         * @details ���G�Ȑݒ���������Ƃ��Ɏg�p����
         */
        explicit RasterizerDesc(const D3D12_RASTERIZER_DESC& desc) : D3D12_RASTERIZER_DESC(desc) {}
    };
} // namespace Framework::Desc