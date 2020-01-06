#pragma once

namespace Framework::Desc {
    /**
     * @brief �q�[�v�̎��
     */
    enum class HeapType {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
    };

    /**
     * @brief �q�[�v�̃t���O
     */
    enum class HeapFlag {
        None,
        ShaderVisible,
    };

    /**
     * @brief �f�B�X�N���v�^�q�[�v�̃f�X�N
     */
    struct DescriptorHeapDesc {
        UINT descriptorNum; //<! ���蓖�Ă鐔
        HeapType type; //!< ���
        HeapFlag flags; //!< �t���O
        UINT mask; //!< �}�X�N
        /**
         * @brief �R���X�g���N�^
         */
        DescriptorHeapDesc() : DescriptorHeapDesc(0, HeapType::CBV_SRV_UAV, HeapFlag::None, 0) {}
        /**
         * @brief �R���X�g���N�^
         */
        DescriptorHeapDesc(
            UINT num, HeapType type, HeapFlag flags = HeapFlag::ShaderVisible, UINT mask = 0)
            : descriptorNum(num), type(type), flags(flags), mask(mask) {}
    };
} // namespace Framework::Desc