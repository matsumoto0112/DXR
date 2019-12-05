#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/ShaderResourceView.h"
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
        Texture2D() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~Texture2D() {}

        void init(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         * @param device �f�o�C�X
         */
        void createSRV(ID3D12Device* device, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

        //private:
        Buffer mBuffer;
        UINT mWidth = 0;
        UINT mHeight = 0;
        ShaderResourceView mView;
        Desc::TextureFormat mFormat;
    };
} // namespace Framework::DX
