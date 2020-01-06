#pragma once

namespace Framework::Desc {
    /**
     * @brief �u�����h�̎�ޒ�`
     */
    enum class BlendMode {
        Default, //!< �u�����h�Ȃ�
        Add, //!< ���Z����
        Substract, //!< ���Z����
        Multiple, //!< �ώZ����
        Alignment, //!< ���`����

        Custom, //!< �J�X�^��
    };

    /**
     * @brief �f�t�H���g�̃u�����h�f�B�X�N���擾����
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC defaultBlendDesc() {
        return { FALSE, FALSE, D3D12_BLEND::D3D12_BLEND_ONE, D3D12_BLEND::D3D12_BLEND_ZERO,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief ���Z�����f�B�X�N���擾����
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC addBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_SRC_ALPHA, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief ���Z�����f�B�X�N���擾����
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC substractBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_SRC_ALPHA, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND_OP::D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief �ώZ�����f�B�X�N���擾����
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC multipleBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND::D3D12_BLEND_SRC_COLOR,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_BLEND::D3D12_BLEND_ONE,
            D3D12_BLEND::D3D12_BLEND_ZERO, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief ���`�����f�B�X�N���擾����
     */
    static inline constexpr D3D12_RENDER_TARGET_BLEND_DESC alignmentBlendDesc() {
        return { TRUE, FALSE, D3D12_BLEND::D3D12_BLEND_SRC_ALPHA,
            D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD,
            D3D12_BLEND::D3D12_BLEND_ONE, D3D12_BLEND::D3D12_BLEND_ZERO,
            D3D12_BLEND_OP::D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL };
    }

    /**
     * @brief �u�����h���[�h����f�B�X�N���擾����
     */
    static constexpr D3D12_RENDER_TARGET_BLEND_DESC fromBlendMode(BlendMode mode) {
        //�J�X�^���̏ꍇ�̓f�t�H���g�Ɠ������̂�Ԃ�
        switch (mode) {
        case Framework::Desc::BlendMode::Add: return addBlendDesc();
        case Framework::Desc::BlendMode::Substract: return substractBlendDesc();
        case Framework::Desc::BlendMode::Multiple: return multipleBlendDesc();
        case Framework::Desc::BlendMode::Alignment: return alignmentBlendDesc();
        default: return defaultBlendDesc();
        }
    }

    /**
     * @brief �u�����h�f�B�X�N�\����
     */
    struct BlendDesc : public D3D12_BLEND_DESC {
        /**
         * @brief �R���X�g���N�^
         * @param mode �u�����h�̎��
         * @details mode����u�����h�f�B�X�N�𐶐�����
         */
        explicit BlendDesc(BlendMode mode) {
            AlphaToCoverageEnable = FALSE;
            IndependentBlendEnable = FALSE;

            D3D12_RENDER_TARGET_BLEND_DESC desc = fromBlendMode(mode);
            for (size_t n = 0; n < _countof(RenderTarget); n++) { RenderTarget[n] = desc; }
        }

        /**
         * @brief �R���X�g���N�^
         */
        explicit BlendDesc(const D3D12_BLEND_DESC& desc) : D3D12_BLEND_DESC(desc) {}
    };
} // namespace Framework::Desc