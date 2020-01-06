#pragma once
#include "DX/Shader/RenderTarget.h"
#include "DX/Resource/ShaderResourceView.h"

namespace Framework::DX {
    enum class RenderTargetState {
        RenderTarget,
        ShaderResource,
    };
    /**
     * @class RenderTargetTexture
     * @brief �e�N�X�`���Ƃ��ė��p�ł��郌���_�[�^�[�Q�b�g
     */
    class RenderTargetTexture {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        RenderTargetTexture() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~RenderTargetTexture() {}
        /**
         * @brief ������
         */
        void init(DeviceResource* device, UINT width, UINT height, DXGI_FORMAT format,
            const Utility::Color4& clearColor, const std::wstring& name);
        /**
         * @brief ��Ԃ�J�ڂ�����
         */
        void transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState);
        /**
         * @brief ���[�h��؂�ւ���
         */
        void transition(ID3D12GraphicsCommandList* commandList, RenderTargetState nextMode);
        /**
         * @brief �����_�[�^�[�Q�b�g�̃N���A
         */
        void clear(ID3D12GraphicsCommandList* commandList, const Utility::Color4& clearColor);
        /**
         * @brief �����_�[�^�[�Q�b�g�r���[���擾����
         */
        const RenderTargetView& getRenderTargetView() const {
            return mRenderTarget.getView();
        }
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���擾����
         */
        const ShaderResourceView& getShaderResourceView() const {
            return mSRV;
        }

    private:
        RenderTarget mRenderTarget;
        ShaderResourceView mSRV;
    };
} // namespace Framework::DX