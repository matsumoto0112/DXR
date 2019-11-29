/**
 * @file VertexBuffer.h
 * @brief ���_�o�b�t�@
 */

#pragma once
#include "DX/Resource/IBuffer.h"
#include "DX/Util/Helper.h"

namespace Framework::DX {
    /**
     * @class VertexBuffer
     * @brief ���_�o�b�t�@
     */
    class VertexBuffer : public IBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         * @tparam T ���_�\����
         * @param device �f�o�C�X
         * @param vertices ���_�f�[�^
         * @param name ���\�[�X��
         */
        template <class T>
        VertexBuffer(
            ID3D12Device* device, const std::vector<T>& vertices, const std::wstring& name);
        /**
         * @brief �f�X�g���N�^
         */
        ~VertexBuffer();
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         * @param device �f�o�C�X
         */
        void createSRV(ID3D12Device* device);

    private:
        const UINT mVertexByteLength; //!< ���_�̑傫��
        const UINT mVertexCount; //!< ���_��
    };
    template <class T>
    inline VertexBuffer::VertexBuffer(
        ID3D12Device* device, const std::vector<T>& vertices, const std::wstring& name)
        : mVertexByteLength(sizeof(T)), mVertexCount(static_cast<UINT>(vertices.size())) {
        size_t size = mVertexByteLength * mVertexCount;
        mResource = createUploadBuffer(device, size, name);
        writeToResource(mResource.Get(), vertices.data(), size);
    }
} // namespace Framework::DX
