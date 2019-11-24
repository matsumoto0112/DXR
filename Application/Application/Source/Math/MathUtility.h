#pragma once
#undef max
#undef min
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include "Angle.h"

namespace Framework::Math {
    static constexpr float PI = 3.1415926536f; //!< 円周率
    static constexpr float PI2 = PI * 2; //!< 2π
    static constexpr float EPSILON = 0.001f; //!< 誤差

    /**
    * @class MathUtil
    * @brief 数学関係のユーティリティクラス
    */
    class MathUtil {
    public:
        /**
        * @brief サイン
        */
        static inline float sin(const Radians& rad) { return std::sinf(rad.getRad()); }

        /**
        * @brief コサイン
        */
        static inline float cos(const Radians& rad) { return std::cosf(rad.getRad()); }

        /**
        * @brief タンジェント
        */
        static inline float tan(const Radians& rad) { return std::tanf(rad.getRad()); }

        /**
        * @brief アークタンジェント
        * @return ラジアン角を返す
        */
        static inline Radians atan2(float y, float x) { return Radians(std::atan2f(y, x)); }
        /**
        * @brief アークサイン
        * @return ラジアン角を返す
        */
        static inline Radians asin(float x) { return Radians(std::asin(x)); }
        /**
        * @brief アークコサイン
        * @return ラジアン角を返す
        */
        static inline Radians acos(float x) { return Radians(std::acos(x)); }

        /**
        * @brief ルート
        */
        static inline float sqrt(float a) { return std::sqrtf(a); }
        /**
        * @brief 累乗
        * @param X 基数
        * @param e 指数
        */
        static inline float pow(float X, float e) { return std::powf(X, e); }
        /**
        * @brief 絶対値
        */
        static inline float abs(float X) { return std::fabsf(X); }

        /**
        * @brief クランプ処理
        * @tparam t クランプする値
        * @tparam min 下限値
        * @tparam max 上限値
        * @return クランプされた値
        */
        template<class T>
        static inline T clamp(const T& t, const T& minValue, const T& maxValue);

        /**
        * @brief 補間
        * @tparam a 開始値
        * @tparam b 終了値
        * @tparam t 補間値（０～１）
        */
        template <class T>
        static inline T lerp(const T& a, const T& b, float t);

        /**
        * @brief 最大値の取得
        */
        template <class T>
        static inline constexpr T mymax(const std::initializer_list<T>& param);
        /**
        * @brief 最大値の取得
        */
        template <class T>
        static inline constexpr T mymax(const T& t1, const T& t2);

        /**
        * @brief 最小値の取得
        */
        template <class T>
        static inline constexpr T mymin(const std::initializer_list<T>& param);
        /**
        * @brief 最小値の取得
        */
        template <class T>
        static inline constexpr T mymin(const T& t1, const T& t2);
    };

    //クランプ処理
    template<class T>
    T MathUtil::clamp(const T& t, const T& minValue, const T& maxValue) {
        T res(t);
        if (res < minValue)res = minValue;
        else if (res > maxValue) res = maxValue;
        return res;
    }

    //補間
    template<class T>
    inline T MathUtil::lerp(const T& a, const T& b, float t) {
        //0～1にクランプ
        t = clamp(t, 0.0f, 1.0f);
        return a * (1.0f - t) + b * t;
    }

    //最大値の取得
    template<class T>
    inline constexpr T MathUtil::mymax(const std::initializer_list<T>& param) {
        return std::max(param);
    }

    //最大値の取得
    template<class T>
    inline constexpr T MathUtil::mymax(const T& t1, const T& t2) {
        return std::max(t1, t2);
    }

    //最小値の取得
    template<class T>
    inline constexpr T MathUtil::mymin(const std::initializer_list<T>& param) {
        return std::min(param);
    }
    //最小値の取得
    template<class T>
    inline constexpr T MathUtil::mymin(const T& t1, const T& t2) {
        return std::min(t1, t2);
    }
} //Framework::Math
