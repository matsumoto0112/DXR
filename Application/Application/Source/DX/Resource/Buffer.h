#pragma once
#include "DX/DeviceResource.h"

namespace Framework::DX {
    /**
     * @class Buffer
     * @brief �o�b�t�@�N���X
     */
    class Buffer {
    public:
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
        ~Buffer();
        /**
         * @brief ������
         */
        void init(
            ID3D12Device* device, Usage usage, UINT size, UINT stride, const std::wstring& name);
        void init(ID3D12Device* device, Usage usage, DXGI_FORMAT format, UINT width, UINT height,
            const std::wstring& name);
        /**
         * @brief �������̃}�b�v����
         * @details �������̏������ݗ̈�̐擪��Ԃ�
         */
        void* map();
        /**
         * @brief �������̃A���}�b�v
         */
        void unmap();
        void writeResource(const void* data, UINT size);
        ID3D12Resource* getResource() const {
            return mResource.Get();
        }
        Usage getResourceType() const {
            return mResourceType;
        }
        UINT getSize() const {
            return mSize;
        }
        UINT getStride() const {
            return mStride;
        }

    private:
        Comptr<ID3D12Resource> mResource;
        Usage mResourceType;
        D3D12_RESOURCE_STATES mCurrentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
        UINT mSize = 0;
        UINT mStride = 0;
        void* mMapped = nullptr;
    };
} // namespace Framework::DX
