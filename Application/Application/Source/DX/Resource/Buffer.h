/**
 * @file Buffer.h
 * @brief �o�b�t�@�N���X
 */

#pragma once
#include "DX/DeviceResource.h"
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    /**
     * @class Buffer
     * @brief �o�b�t�@�N���X
     */
    class Buffer {
    public:
        /**
         * @enum Usage
         * @brief �o�b�t�@�̗p�r
         */
        enum class Usage {
            ConstantBuffer,
            VertexBuffer,
            IndexBuffer,
            ShaderResource,
        };

    public:
        /**
         * @brief �R���X�g���N�^
         */
        Buffer() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~Buffer() {}
        /**
         * @brief �ʏ�̃o�b�t�@�Ƃ��ď�����
         */
        void init(
            ID3D12Device* device, Usage usage, UINT size, UINT stride, const std::wstring& name);
        void init(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
         * @brief �������̃}�b�v����
         * @details �������̏������ݗ̈�̐擪��Ԃ�
         */
        void* map();
        /**
         * @brief �������̃A���}�b�v
         */
        void unmap();
        /**
         * @brief ���\�[�X�̏�������
         */
        void writeResource(const void* data, UINT64 size);
        /**
         * @brief ���\�[�X�̏�Ԃ��ڍs����
         */
        void transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState);
        /**
         * @brief ���\�[�X���擾����
         */
        ID3D12Resource* getResource() const {
            return mResource.Get();
        }
        /**
         * @brief ���\�[�X�̎�ނ��擾����
         */
        Usage getResourceType() const {
            return mResourceType;
        }
        /**
         * @brief ���\�[�X�̃������T�C�Y���擾����
         */
        UINT64 getSize() const {
            return mSize;
        }
        /**
         * @brief ���\�[�X�̃X�g���C�h���擾����
         */
        UINT getStride() const {
            return mStride;
        }

    private:
        void init(ID3D12Device* device, Usage usage, const CD3DX12_HEAP_PROPERTIES& props,
            const CD3DX12_RESOURCE_DESC& desc, const std::wstring& name);

    private:
        Comptr<ID3D12Resource> mResource;
        Usage mResourceType = Usage::ConstantBuffer;
        D3D12_RESOURCE_STATES mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
        UINT64 mSize = 0;
        UINT mStride = 0;
    };
} // namespace Framework::DX
