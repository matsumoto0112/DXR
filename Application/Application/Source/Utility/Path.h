/**
 * @file Path.h
 * @brief パス管理
 */

#pragma once
#include "Singleton.h"

namespace Framework::Utility {
    /**
     * @class ExePath
     * @brief 実行ファイルへのパス
     */
    class ExePath : public Singleton<ExePath> {
    public:
        /**
         * @brief .exeファイルへのパスを取得する
         */
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
        std::filesystem::path mExe; //!< .exeファイルへのパス
    };
} // namespace Framework::Utility
