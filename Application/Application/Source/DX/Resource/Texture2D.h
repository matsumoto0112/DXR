/**
 * @file Texture2D.h
 * @brief �e�N�X�`��2D
 */

#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/ShaderResourceView.h"
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    class DeviceResource;
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
        /**
         * @brief ����������
         */
        void init(DeviceResource* device, ID3D12GraphicsCommandList* commandList,
            const Desc::TextureDesc& desc);
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         * @param device �f�o�C�X
         */
        void createSRV(DeviceResource* device, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        /**
         * @brief �e�N�X�`���o�b�t�@�̎擾
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief �r���[�̎擾
         */
        const ShaderResourceView& getView() const {
            return mView;
        }

    private:
        Desc::TextureDesc mTextureInfo;
        Buffer mBuffer;
        Buffer mImmediateBuffer;
        ShaderResourceView mView;
    };
} // namespace Framework::DX
