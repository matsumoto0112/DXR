#pragma once
#include <string>
#include <codecvt>
#include <vector>
#include <Windows.h>

namespace Framework::Utility {
    /**
    * @brief std::stringからstd::wstringに変換する
    */
    inline std::wstring toWString(const std::string& str) {
        const int len = ::MultiByteToWideChar(932, 0, str.c_str(), -1, nullptr, 0);
        std::wstring res(len * 2 + 2, L'\0');
        if (!::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &res[0], len)) {
            throw std::runtime_error("function toWString ERROR");
        }

        const std::size_t real_len = std::wcslen(res.c_str());
        res.resize(real_len);
        res.shrink_to_fit();
        return res;
    }

    /**
    * @brief std::wstringからstd::stringに変換する
    */
    inline std::string toString(const std::wstring& str) {
        const int len = ::WideCharToMultiByte(932, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string res(len * 2, '\0');
        if (!::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &res[0], len, nullptr, nullptr)) {
            throw std::runtime_error("function toString ERROR");
        }
        const std::size_t real_len = std::strlen(res.c_str());
        res.resize(real_len);
        res.shrink_to_fit();
        return res;
    }

    /**
    * @brief フォーマットから文字列を生成する
    */
    template <class ... Args>
    std::string format(const std::string& fmt, Args ... args) {
        size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args...);
        std::vector<char> buf(len + 1);
        std::snprintf(&buf[0], len + 1, fmt.c_str(), args...);
        return std::string(&buf[0], &buf[0] + len);
    }
    /**
    * @brief フォーマットから文字列を生成する
    */
    template <class... Args>
    std::wstring format(const std::wstring& fmt, Args... args) {
        size_t len = std::swprintf(nullptr, 0, fmt.c_str(), args...);
        std::vector<wchar_t> buf(len + 1);
        std::swprintf(&buf[0], len + 1, fmt.c_str(), args...);
        return std::wstring(&buf[0], &buf[0] + len);
    }
} //Framework::Utility
