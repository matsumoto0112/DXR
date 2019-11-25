/**
 * @file HrException.h
 * @brief HRESULT��O
 */

#pragma once
#include <stdexcept>

namespace Framework::Utility {
    /**
     * @brief HRESULT��O�N���X
     */
    class HrException : public std::runtime_error {
        /**
         * @brief HRESULT�𕶎���ɕϊ�����
         */
        inline std::string HrToString(HRESULT hr) {
            char s_str[64] = {};
            sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
            return std::string(s_str);
        }

    public:
        /**
         * @brief �R���X�g���N�^
         * @param hr ��������
         * @param str �A�����镶����
         */
        HrException(HRESULT hr, const std::string& str = "")
            : std::runtime_error(HrToString(hr) + "\n" + str), m_hr(hr) {}
        /**
         * @brief �G���[�̎�ނ�Ԃ�
         */
        HRESULT Error() const { return m_hr; }

    private:
        const HRESULT m_hr;
    };
} // namespace Framework::Utility
