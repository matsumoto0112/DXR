/**
 * @file VertexBufferView.h
 * @brief ���_�o�b�t�@�r���[
 */

#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class VertexBufferView
     * @brief ���_�o�b�t�@�r���[
     */
    class VertexBufferView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        VertexBufferView() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~VertexBufferView() {}
        /**
         * @brief ������
         */
        void init(const Buffer& buffer);
        /**
         * @brief �r���[�̎擾
         */
        const D3D12_VERTEX_BUFFER_VIEW& getView() const {
            return mView;
        }

    private:
        D3D12_VERTEX_BUFFER_VIEW mView;
    };
} // namespace Framework::DX
