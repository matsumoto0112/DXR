#pragma once
#include <Windows.h>
#include <vector>

namespace Framework::Desc {
    /**
    * @brief �e�N�X�`���f�B�X�N
    */
    struct TextureDesc {
        std::string name; //!< �e�N�X�`����
        std::vector<BYTE> pixels; //!< �s�N�Z���f�[�^
        UINT width; //!< ��
        UINT height; //!< ����
    };
} //Framework::Desc
