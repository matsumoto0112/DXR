#pragma once
#include <Windows.h>
#include <string>

namespace Framework::Desc {
    /**
    * @enum HeapType
    * @brief �q�[�v�̎�ޒ�`
    */
    enum class HeapType {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
    };

    /**
    * @enum HeapFlag
    * @brief �q�[�v�̃t���O��`
    */
    enum class HeapFlag {
        None,
        ShaderVisible,
    };
    /**
    * @brief �f�[�u���f�B�X�N
    */
    struct DescriptorTableDesc {
        std::wstring name; //!< �e�[�u����
        UINT descriptorNum; //!< ���蓖�Ă鐔
        HeapType type; //!< ���
        HeapFlag flag; //!< �t���O
    };
} // namespace Framework::Desc
