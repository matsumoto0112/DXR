/**
 * @file CountingDescriptorTable.h
 * @brief �J�E���^�[�t���f�B�X�N���v�^�e�[�u��
 */

#pragma once
#include "DX/DescriptorTable.h"
#include "DX/Resource/IBuffer.h"

namespace Framework::DX {
    /**
     * @class CountingDescriptorTable
     * @brief �J�E���^�[�t���e�[�u��
     */
    class CountingDescriptorTable : public DescriptorTable {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        CountingDescriptorTable(ID3D12Device* device, const Desc::DescriptorTableDesc& desc);
        /**
         * @brief �f�X�g���N�^
         */
        ~CountingDescriptorTable();
        /**
         * @brief �q�[�v�̊m��
         */
        void allocate(IBuffer* buffer);
        /**
         * @brief �J�E���^�[�̃��Z�b�g
         */
        void resetCounter() { mIndex = 0; }

    private:
        UINT mIndex; //!< ���݂̃C���f�b�N�X
        UINT mDescriptorNum; //!< ���蓖�Đ�
    };
} // namespace Framework::DX
