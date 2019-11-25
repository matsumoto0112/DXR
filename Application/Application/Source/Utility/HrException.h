/**
 * @file HrException.h
 * @brief HRESULT例外
 */

#pragma once
#include <stdexcept>

namespace Framework::Utility {
    /**
     * @brief HRESULT例外クラス
     */
    class HrException : public std::runtime_error {
        /**
         * @brief HRESULTを文字列に変換する
         */
        inline std::string HrToString(HRESULT hr) {
            char s_str[64] = {};
            sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
            return std::string(s_str);
        }

    public:
        /**
         * @brief コンストラクタ
         * @param hr 処理結果
         * @param str 連結する文字列
         */
        HrException(HRESULT hr, const std::string& str = "")
            : std::runtime_error(HrToString(hr) + "\n" + str), m_hr(hr) {}
        /**
         * @brief エラーの種類を返す
         */
        HRESULT Error() const { return m_hr; }

    private:
        const HRESULT m_hr;
    };
} // namespace Framework::Utility
