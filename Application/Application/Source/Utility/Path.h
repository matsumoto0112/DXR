#pragma once
#include <string>
#include <filesystem>
#include "Singleton.h"

namespace Framework::Utility {
    /**
    * @class ExePath
    * @brief 実行ファイルへのパス
    */
    class ExePath : public Singleton<ExePath> {
    public:
        const std::filesystem::path& exe() const { return mExe; }
    protected:
        /**
        * @brief コンストラクタ
        */
        ExePath();
        /**
        * @brief デストラクタ
        */
        ~ExePath();
    private:
        std::filesystem::path mExe;
    };
} //Framework::Utility 