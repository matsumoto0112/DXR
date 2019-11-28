/**
 * @file VertexBuffer.h
 * @brief ���_�o�b�t�@
 */

#pragma once
#include "DX/Resource/IBuffer.h"

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
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc
            = CD3DX12_RESOURCE_DESC::Buffer(mVertexByteLength * mVertexCount);
        MY_THROW_IF_FAILED(
            device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &bufferDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                IID_PPV_ARGS(&mResource)));
        MY_THROW_IF_FAILED(mResource->SetName(name.c_str()));

        void* mapped;
        MY_THROW_IF_FAILED(mResource->Map(0, nullptr, &mapped));
        memcpy(mapped, vertices.data(), mVertexByteLength * mVertexCount);
        mResource->Unmap(0, nullptr);
    }
} // namespace Framework::DX
