#pragma once
#include <string>
#include <filesystem>
#include "Singleton.h"

namespace Framework::Utility {
    /**
    * @class ExePath
    * @brief ���s�t�@�C���ւ̃p�X
    */
    class ExePath : public Singleton<ExePath> {
    public:
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
        std::filesystem::path mExe;
    };
} //Framework::Utility 