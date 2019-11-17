#pragma once
#include <vector>
#include <Windows.h>
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    /**
    * @class Texture2D
    * @brief 2D�e�N�X�`���N���X
    */
    class Texture2D {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Texture2D(const Desc::TextureDesc& desc);
        /**
        * @brief �f�X�g���N�^
        */
        ~Texture2D();

        void createSRV();
    private:

    };
} //Framework::DX
