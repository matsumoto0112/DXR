#pragma once
#include "DX/Shader/DepthStencilView.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class DepthStencil
     * @brief �f�v�X�E�X�e���V��
     */
    class DepthStencil {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        DepthStencil() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~DepthStencil() {}
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
         * @brief �r���[�̃N���A����
         */
        void clear(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief �o�b�t�@���擾����
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief �r���[�̎擾
         */
        const DepthStencilView& getView() const {
            return mDSV;
        }

    private:
        Buffer mBuffer;
        DepthStencilView mDSV;
    };
} // namespace Framework::DX
