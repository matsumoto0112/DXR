/**
 * @file IndexBufferView.h
 * @brief �C���f�b�N�X�o�b�t�@�r���[
 */

#pragma once
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class IndexBufferView
     * @brief �C���f�b�N�X�o�b�t�@�r���[
     */
    class IndexBufferView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        IndexBufferView() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~IndexBufferView() {}
        /**
         * @brief ����������
         */
        void init(const Buffer& buffer);
        /**
         * @brief �r���[�̎擾
         */
        const D3D12_INDEX_BUFFER_VIEW& getView() const {
            return mView;
        }

    private:
        D3D12_INDEX_BUFFER_VIEW mView = {}; //!< �r���[
    };
} // namespace Framework::DX
