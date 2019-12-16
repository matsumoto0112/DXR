/**
 * @file IndexBuffer.h
 * @brief �C���f�b�N�X�o�b�t�@
 */

#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/IndexBufferView.h"
#include "DX/Resource/ShaderResourceView.h"

namespace Framework::DX {
    /**
     * @class IndexBuffer
     * @brief �C���f�b�N�X�o�b�t�@�N���X
     */
    class IndexBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        IndexBuffer() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~IndexBuffer() {}
        /**
         * @brief ����������
         */
        template <class T>
        void init(DeviceResource* device, const std::vector<T>& indices,
            D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name);
        /**
         * @brief �R�}���h���X�g�ɃZ�b�g����
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief �`�悷��
         */
        void draw(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief �o�b�t�@�̎擾
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         */
        ShaderResourceView createSRV(DeviceResource* device,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

    private:
        Buffer mBuffer; //!< �o�b�t�@
        IndexBufferView mView; //!< �r���[
        D3D_PRIMITIVE_TOPOLOGY mTopology
            = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; //!< �g�|���W�[�̎��
        UINT mIndexNum = 0; //!< �C���f�b�N�X��
    };
    //������
    template <class T>
    inline void IndexBuffer::init(DeviceResource* device, const std::vector<T>& indices,
        D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name) {
        mIndexNum = static_cast<UINT>(indices.size());
        mTopology = topology;
        mBuffer.init(device, Buffer::Usage::IndexBuffer, mIndexNum * sizeof(T), sizeof(T), name);
        mBuffer.writeResource(indices.data(), mIndexNum * sizeof(T));
        mView.init(mBuffer);
    }
} // namespace Framework::DX
