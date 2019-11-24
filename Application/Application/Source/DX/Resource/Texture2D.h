#pragma once
#include <d3d12.h>
#include "Desc/TextureDesc.h"
#include "DX/Resource/IBuffer.h"
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @class Texture2D
    * @brief 2D�e�N�X�`���N���X
    */
    class Texture2D : public IBuffer {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Texture2D(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
        * @brief �f�X�g���N�^
        */
        ~Texture2D();

        void createSRV(ID3D12Device* device);
    private:
        UINT mWidth;
        UINT mHeight;
        Desc::TextureFormat mFormat;
    };
} //Framework::DX
