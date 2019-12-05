#pragma once
#include "DX/Resource/Buffer.h"
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
        Texture2D();
        /**
         * @brief �R���X�g���N�^
         * @param device �f�o�C�X
         * @param desc �e�N�X�`���f�B�X�N
         */
        Texture2D(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
         * @brief �f�X�g���N�^
         */
        ~Texture2D();
        /**
         * @brief �o�b�t�@���쐬����
         * @param device �f�o�C�X
         * @param desc �e�N�X�`���f�B�X�N
         */
        void createBuffer(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         * @param device �f�o�C�X
         */
        void createSRV(ID3D12Device* device);

        //private:
        Buffer mBuffer;
        UINT mWidth = 0;
        UINT mHeight = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
        Desc::TextureFormat mFormat;
    };
} // namespace Framework::DX
