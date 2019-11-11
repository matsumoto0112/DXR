#pragma once
#include <vector>
#include <Windows.h>

namespace Framework::DX {
    /**
    * @brief �e�N�X�`���f�[�^
    */
    struct TextureData {
        std::vector<BYTE> pixels;
        UINT width;
        UINT height;
    };

    /**
    * @class Texture2D
    * @brief 2D�e�N�X�`���N���X
    */
    class Texture2D {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Texture2D(const TextureData& texture);
        /**
        * @brief �f�X�g���N�^
        */
        ~Texture2D();

        void createSRV();
    private:

    };
} //Framework::DX
