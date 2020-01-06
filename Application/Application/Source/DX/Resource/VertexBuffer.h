#pragma once
#include "DX/DeviceResource.h"
#include "DX/Resource/Buffer.h"
#include "DX/Resource/VertexBufferView.h"

namespace Framework::DX {
    /**
     * @class VertexBuffer
     * @brief ���_�o�b�t�@�N���X
     */
    class VertexBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        VertexBuffer() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~VertexBuffer() {}
        /**
         * @brief ������
         * @tparam T ���_�\����
         */
        template <class T>
        void init(DeviceResource* device, const std::vector<T>& vertices, const std::wstring& name);
        /**
         * @brief �R�}���h���X�g�ɃZ�b�g����
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief �o�b�t�@�̎擾
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief �o�b�t�@�r���[�̎擾
         */
        const VertexBufferView& getView() const {
            return mView;
        }

    private:
        Buffer mBuffer = {};
        VertexBufferView mView = {};
        UINT mVertexCount = 0;
    };
    template <class T>
    inline void VertexBuffer::init(
        DeviceResource* device, const std::vector<T>& vertices, const std::wstring& name) {
        mVertexCount = static_cast<UINT>(vertices.size());
        mBuffer.init(device, Buffer::Usage::VertexBuffer,
            static_cast<UINT>(mVertexCount * sizeof(T)), static_cast<UINT>(sizeof(T)), name);
        mBuffer.writeResource(vertices.data(), static_cast<UINT>(mVertexCount * sizeof(T)));
        mView.init(mBuffer);
    }
} // namespace Framework::DX
