/**
 * @file Debug.h
 * @brief �f�o�b�O���[�e�B���e�B
 */

#pragma once
#include <assert.h>
#include <crtdbg.h>
#include <stdarg.h>
#include <stdio.h>
#include <wtypes.h>
#include "HrException.h"
#include "StringUtil.h"

#ifdef _DEBUG
/**
 * @def MY_ASSERTION
 * @brief �A�T�[�V����
 * @param[in] expr ������
 * @param[in] format �����ݒ�
 * @details expr��false�̎��A���������֐����ƍs���A�G���[���b�Z�[�W���\�������
 */
#define MY_ASSERTION(expr, format, ...)                                                         \
    do {                                                                                        \
        Framework::Utility::assertion(expr, std::string("error an occurred %s %d:\n") + format, \
            __FUNCTION__, __LINE__, __VA_ARGS__);                                               \
    } while (0)
/**
 * @def MY_DEBUG_LOG
 * @brief �f�o�b�O���O
 * @param[in] format �����ݒ�
 */
#define MY_DEBUG_LOG(format, ...) \
    do { Framework::Utility::debugLog(format, __VA_ARGS__); } while (0)
#else
#define MY_ASSERTION(expr, format, ...)
#define MY_DEBUG_LOG(format, ...)
#endif
/**
 * @def MY_THROW_IF_FALSE_FORMAT_LOG
 * @brief ���s���Ă������O�𓊂���
 * @param[in] expr ������
 * @paran format �����ݒ�
 */
#define MY_THROW_IF_FALSE_LOG(expr, format, ...) \
    do { Framework::Utility::throwIfFalse(expr, format, __VA_ARGS__); } while (0)
/**
 * @def MY_THROW_IF_FALSE
 * @brief ���s���Ă������O�𓊂���
 * @param[in] expr ������
 */
#define MY_THROW_IF_FALSE(expr) \
    do { Framework::Utility::throwIfFalse(expr); } while (0)
/**
 * @def MY_THROW_IF_FAILED_LOG
 * @brief ���s���Ă������O�𓊂���
 * @param[in] hr ��������
 * @param format �����ݒ�
 */
#define MY_THROW_IF_FAILED_LOG(hr, format, ...) \
    do { Framework::Utility::throwIfFailed(hr, format, __VA_ARGS__); } while (0)
/**
 * @def MY_THROW_IF_FAILED
 * @brief ���s���Ă������O�𓊂���
 * @param[in] hr ��������
 */
#define MY_THROW_IF_FAILED(hr) \
    do { Framework::Utility::throwIfFailed(hr); } while (0)

namespace Framework::Utility {
    /**
     * @brief �A�T�[�V�����֐�
     * @param expr ������
     * @param format �t�H�[�}�b�g
     */
    template <class... Args>
    inline void assertion(bool expr, const std::string& fmt, Args... args) {
        std::string mes = format(fmt, args...);
        std::wstring wmes = toWString(mes);
        _ASSERT_EXPR(expr, wmes.c_str());
    }
    /**
     * @brief �o�̓E�B���h�E�Ƀ��O���o�͂���
     * @param fmt �t�H�[�}�b�g
     */
    template <class... Args>
    inline void debugLog(const std::string& fmt, Args... args) {
        _RPTN(_CRT_WARN, fmt.c_str(), args...);
    }

    /**
     * @brief ���s���Ă������O�𓊂���
     */
    template <class... Args>
    inline void throwIfFailed(HRESULT hr, const std::string& fmt, Args... args) {
        if (FAILED(hr)) {
            std::string mes = format(fmt, args...);
            throw HrException(hr, mes);
        }
    }

    /**
     * @brief ���s���Ă������O�𓊂���
     */
    inline void throwIfFailed(HRESULT hr) {
        if (FAILED(hr)) { throw HrException(hr); }
    }

    /**
     * @brief ���s���Ă������O�𓊂���
     */
    template <class... Args>
    inline void throwIfFalse(bool expr, const std::string& fmt, Args... args) {
        throwIfFailed(expr ? S_OK : E_FAIL, fmt, args...);
    }

    /**
     * @brief ���s���Ă������O�𓊂���
     */
    inline void throwIfFalse(bool expr) { throwIfFailed(expr ? S_OK : E_FAIL); }

    /**
     * @brief �G���[�E�B���h�E��\������
     */
    template <class... Args>
    inline void errorWindow(bool expr, const std::string& fmt, Args... args) {
        if (!expr) {
            std::string mes = format(fmt, args...);
            MessageBoxA(nullptr, mes.c_str(), "�G���[", MB_APPLMODAL);
        }
    }
} // namespace Framework::Utility
