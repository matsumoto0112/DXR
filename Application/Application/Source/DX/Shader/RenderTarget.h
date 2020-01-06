#pragma once
#include "DX/Shader/RenderTargetView.h"
#include "Utility/Color4.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RenderTarget
     * @brief �����_�[�^�[�Q�b�g
     */
    class RenderTarget {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        RenderTarget() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~RenderTarget() {}
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
         * @brief �����_�[�^�[�Q�b�g�̃N���A
         */
        void clear(ID3D12GraphicsCommandList* commandList, const Utility::Color4& color);
        /**
         * @brief ���\�[�X���珉����������
         */
        void initAsResource(DeviceResource* device, Comptr<ID3D12Resource> resource);
        /**
         * @brief �o�b�t�@���擾����
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief �r���[�̎擾
         */
        const RenderTargetView& getView() const {
            return mView;
        }

    private:
        Buffer mBuffer;
        RenderTargetView mView;
    };
} // namespace Framework::DX