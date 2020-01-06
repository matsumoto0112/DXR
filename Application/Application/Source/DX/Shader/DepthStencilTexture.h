#pragma once
#include "DX/Shader/DepthStencil.h"
#include "DX/Resource/ShaderResourceView.h"

namespace Framework::DX {
    /**
     * @brief �o�b�t�@�̏�Ԓ�`
     */
    enum class DepthStencilState {
        DepthStencil,
        ShaderResource,
    };
    /**
     * @class DepthStencilTexture
     * @brief �e�N�X�`���Ƃ��ė��p�ł���f�v�X�E�X�e���V���o�b�t�@
     */
    class DepthStencilTexture {
    public:
    public:
        /**
         * @brief �R���X�g���N�^
         */
        DepthStencilTexture() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~DepthStencilTexture() {}
        /**
         * @brief ������
         */
        void init(DeviceResource* device, UINT width, UINT height, DXGI_FORMAT format,
            const std::wstring& name);
        /**
         * @brief ��Ԃ�J�ڂ�����
         */
        void transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState);
        /**
         * @brief ��Ԃ�J�ڂ�����
         */
        void transition(ID3D12GraphicsCommandList* commandList, DepthStencilState nextMode);
        /**
         * @brief �f�v�X�E�X�e���V���̃N���A
         */
        void clear(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief �f�v�X�E�X�e���V���r���[�̎擾
         */
        const DepthStencilView& getDepthStencilView() const {
            return mDepthStencil.getView();
        }
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[�̎擾
         */
        const ShaderResourceView& getShaderResourceView() const {
            return mSRV;
        }

    private:
        DepthStencil mDepthStencil;
        ShaderResourceView mSRV;
    };
} // namespace Framework::DX