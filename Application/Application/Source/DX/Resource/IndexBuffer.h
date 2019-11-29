/**
 * @file IndexBuffer.h
 * @brief �C���f�b�N�X�o�b�t�@
 */

#pragma once
#include "DX/Resource/IBuffer.h"

namespace Framework::DX {
    using Index = UINT16;
    /**
     * @class IndexBuffer
     * @brief �C���f�b�N�X�o�b�t�@
     */
    class IndexBuffer : public IBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param device �f�o�C�X
         * @param indices �C���f�b�N�X�z��
         * @param topologyType �v���~�e�B�u�̎��
         * @param name ���\�[�X��
         */
        IndexBuffer(ID3D12Device* device, const std::vector<Index>& indices,
            D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, const std::wstring& name);
        /* *
         * @brief �f�X�g���N�^
         */
        ~IndexBuffer();
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         * @param device �f�o�C�X
         */
        void createSRV(ID3D12Device* device);

    private:
        UINT mIndexCount; //!< �C���f�b�N�X��
        D3D12_PRIMITIVE_TOPOLOGY_TYPE mTopologyType; //!< �v���~�e�B�u�̎��
    };
} // namespace Framework::DX
