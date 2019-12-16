/**
 * @file Buffer.h
 * @brief �o�b�t�@�N���X
 */

#pragma once
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    class DeviceResource;
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
        void init(DeviceResource* device, Usage usage, UINT64 size, UINT stride,
            const std::wstring& name);
        void init(DeviceResource* device, const Desc::TextureDesc& desc,
            const D3D12_CLEAR_VALUE* clearValue = nullptr);
        /**
         * @brief ���\�[�X���珉����
         */
        void init(Comptr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state);
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
         * @brief ���݂̃��\�[�X�̏�Ԃ��擾����
         */
        D3D12_RESOURCE_STATES getCurrentState() const {
            return mCurrentState;
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
        Comptr<ID3D12Resource> mResource;
        Usage mResourceType = Usage::ConstantBuffer;
        D3D12_RESOURCE_STATES mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
        UINT64 mSize = 0;
        UINT mStride = 0;
    };
} // namespace Framework::DX
