/**
 * @file IDXInterfaceAccessor.h
 * @brief DX�C���^�[�t�F�[�X�ւ̃A�N�Z�T�̒�
 */

#pragma once

namespace Framework::DX {
    /**
     * @class IDXInterfaceAccessor
     * @brief DX�C���^�[�t�F�[�X�ւ̃A�N�Z�T�̒�
     */
    class IDXInterfaceAccessor {
    public:
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~IDXInterfaceAccessor() = default;
        /**
         * @brief �f�o�C�X�̎擾
         */
        virtual ID3D12Device* getDevice() const = 0;
    };
} // namespace Framework::DX
