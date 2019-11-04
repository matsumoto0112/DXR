#pragma once
#undef max
#undef min
#include <initializer_list>
#include <algorithm>
#include <cmath>

namespace Framework::Math {
    static constexpr float PI = 3.141592654f;//!< 円周率
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
        static inline float sin(float degree) { return std::sinf(toRadian(degree)); }

        /**
        * @brief コサイン
        */
        static inline float cos(float degree) { return std::cosf(toRadian(degree)); }

        /**
        * @brief タンジェント
        */
        static inline float tan(float degree) { return std::tanf(toRadian(degree)); }

        /**
        * @brief アークタンジェント
        * @return 角度(deg)を返す
        */
        static inline float atan2(float y, float x) { return toDegree(std::atan2f(y, x)); }
        /**
        * @brief アークサイン
        * @return 角度(deg)を返す
        */
        static inline float asin(float x) { return toDegree(std::asin(x)); }
        /**
        * @brief アークコサイン
        * @param 角度(deg)を返す
        */
        static inline float acos(float x) { return toDegree(std::acos(x)); }

        /**
        * @brief ルート
        */
        static inline float sqrt(float a) { return std::sqrtf(a); }

        /**
        * @brief 度からラジアンへ変換する
        * @param deg 度
        * @return 変換されたラジアン
        */
        static inline float toRadian(float deg) { return deg / 180.0f * PI; }

        /**
        * @brief ラジアンから度へ変換する
        * @param rad ラジアン
        * @return 変換された度
        */
        static inline float toDegree(float rad) { return rad / PI * 180.0f; }
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
        static inline T mymax(const std::initializer_list<T>& param);
        /**
        * @brief 最大値の取得
        */
        template <class T>
        static inline T mymax(const T& t1, const T& t2);

        /**
        * @brief 最小値の取得
        */
        template <class T>
        static inline T mymin(const std::initializer_list<T>& param);
        /**
        * @brief 最小値の取得
        */
        template <class T>
        static inline T mymin(const T& t1, const T& t2);
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
    inline T MathUtil::mymax(const std::initializer_list<T>& param) {
        return std::max(param);
    }

    //最大値の取得
    template<class T>
    inline T MathUtil::mymax(const T& t1, const T& t2) {
        return std::max(t1, t2);
    }

    //最小値の取得
    template<class T>
    inline T MathUtil::mymin(const std::initializer_list<T>& param) {
        return std::min(param);
    }
    //最小値の取得
    template<class T>
    inline T MathUtil::mymin(const T& t1, const T& t2) {
        return std::min(t1, t2);
    }
} //Framework::Math
