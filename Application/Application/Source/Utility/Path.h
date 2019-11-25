/**
 * @file Path.h
 * @brief �p�X�Ǘ�
 */

#pragma once
#include "Singleton.h"

namespace Framework::Utility {
    /**
     * @class ExePath
     * @brief ���s�t�@�C���ւ̃p�X
     */
    class ExePath : public Singleton<ExePath> {
    public:
        /**
         * @brief .exe�t�@�C���ւ̃p�X���擾����
         */
        const std::filesystem::path& exe() const { return mExe; }

    protected:
        /**
         * @brief �R���X�g���N�^
         */
        ExePath();
        /**
         * @brief �f�X�g���N�^
         */
        ~ExePath();

    private:
        std::filesystem::path mExe; //!< .exe�t�@�C���ւ̃p�X
    };
} // namespace Framework::Utility
