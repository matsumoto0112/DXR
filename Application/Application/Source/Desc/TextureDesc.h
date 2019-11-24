#pragma once
#include <Windows.h>
#include <vector>

namespace Framework::Desc {
    /**
    * @enum TextureFormat
    * @brief �e�N�X�`���̃t�H�[�}�b�g
    */
    enum class TextureFormat {
        R8G8B8A8,
    };

    /**
    * @struct TextureDesc
    * @brief �e�N�X�`���f�B�X�N
    */
    struct TextureDesc {
        std::wstring name; //!< �e�N�X�`����
        std::vector<BYTE> pixels; //!< �s�N�Z���f�[�^
        UINT width; //!< ��
        UINT height; //!< ����
        TextureFormat format; //!< �t�H�[�}�b�g
    };
} // namespace Framework::Desc
