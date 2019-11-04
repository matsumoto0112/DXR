#pragma once
#undef max
#undef min
#include <initializer_list>
#include <algorithm>
#include <cmath>

namespace Framework::Math {
    static constexpr float PI = 3.141592654f;//!< �~����
    static constexpr float PI2 = PI * 2; //!< 2��
    static constexpr float EPSILON = 0.001f; //!< �덷

    /**
    * @class MathUtil
    * @brief ���w�֌W�̃��[�e�B���e�B�N���X
    */
    class MathUtil {
    public:
        /**
        * @brief �T�C��
        */
        static inline float sin(float degree) { return std::sinf(toRadian(degree)); }

        /**
        * @brief �R�T�C��
        */
        static inline float cos(float degree) { return std::cosf(toRadian(degree)); }

        /**
        * @brief �^���W�F���g
        */
        static inline float tan(float degree) { return std::tanf(toRadian(degree)); }

        /**
        * @brief �A�[�N�^���W�F���g
        * @return �p�x(deg)��Ԃ�
        */
        static inline float atan2(float y, float x) { return toDegree(std::atan2f(y, x)); }
        /**
        * @brief �A�[�N�T�C��
        * @return �p�x(deg)��Ԃ�
        */
        static inline float asin(float x) { return toDegree(std::asin(x)); }
        /**
        * @brief �A�[�N�R�T�C��
        * @param �p�x(deg)��Ԃ�
        */
        static inline float acos(float x) { return toDegree(std::acos(x)); }

        /**
        * @brief ���[�g
        */
        static inline float sqrt(float a) { return std::sqrtf(a); }

        /**
        * @brief �x���烉�W�A���֕ϊ�����
        * @param deg �x
        * @return �ϊ����ꂽ���W�A��
        */
        static inline float toRadian(float deg) { return deg / 180.0f * PI; }

        /**
        * @brief ���W�A������x�֕ϊ�����
        * @param rad ���W�A��
        * @return �ϊ����ꂽ�x
        */
        static inline float toDegree(float rad) { return rad / PI * 180.0f; }
        /**
        * @brief �ݏ�
        * @param X �
        * @param e �w��
        */
        static inline float pow(float X, float e) { return std::powf(X, e); }
        /**
        * @brief ��Βl
        */
        static inline float abs(float X) { return std::fabsf(X); }

        /**
        * @brief �N�����v����
        * @tparam t �N�����v����l
        * @tparam min �����l
        * @tparam max ����l
        * @return �N�����v���ꂽ�l
        */
        template<class T>
        static inline T clamp(const T& t, const T& minValue, const T& maxValue);

        /**
        * @brief ���
        * @tparam a �J�n�l
        * @tparam b �I���l
        * @tparam t ��Ԓl�i�O�`�P�j
        */
        template <class T>
        static inline T lerp(const T& a, const T& b, float t);

        /**
        * @brief �ő�l�̎擾
        */
        template <class T>
        static inline T mymax(const std::initializer_list<T>& param);
        /**
        * @brief �ő�l�̎擾
        */
        template <class T>
        static inline T mymax(const T& t1, const T& t2);

        /**
        * @brief �ŏ��l�̎擾
        */
        template <class T>
        static inline T mymin(const std::initializer_list<T>& param);
        /**
        * @brief �ŏ��l�̎擾
        */
        template <class T>
        static inline T mymin(const T& t1, const T& t2);
    };

    //�N�����v����
    template<class T>
    T MathUtil::clamp(const T& t, const T& minValue, const T& maxValue) {
        T res(t);
        if (res < minValue)res = minValue;
        else if (res > maxValue) res = maxValue;
        return res;
    }

    //���
    template<class T>
    inline T MathUtil::lerp(const T& a, const T& b, float t) {
        //0�`1�ɃN�����v
        t = clamp(t, 0.0f, 1.0f);
        return a * (1.0f - t) + b * t;
    }

    //�ő�l�̎擾
    template<class T>
    inline T MathUtil::mymax(const std::initializer_list<T>& param) {
        return std::max(param);
    }

    //�ő�l�̎擾
    template<class T>
    inline T MathUtil::mymax(const T& t1, const T& t2) {
        return std::max(t1, t2);
    }

    //�ŏ��l�̎擾
    template<class T>
    inline T MathUtil::mymin(const std::initializer_list<T>& param) {
        return std::min(param);
    }
    //�ŏ��l�̎擾
    template<class T>
    inline T MathUtil::mymin(const T& t1, const T& t2) {
        return std::min(t1, t2);
    }
} //Framework::Math
