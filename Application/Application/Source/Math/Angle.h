/**
 * @file Angle.h
 * @brief 度数法・弧度法の角度を明示的にするクラス
 */

#pragma once

namespace Framework::Math {
    class Radians;
    /**
     * @class Degrees
     * @brief 度数法による角度
     */
    class Degrees {
    public:
        /**
         * @brief コンストラクタ
         */
        Degrees();
        /**
         * @brief コンストラクタ
         */
        explicit Degrees(float degree);
        /**
         * @brief コピーコンストラクタ
         */
        Degrees(const Degrees& deg);
        /**
         * @brief コンストラクタ
         */
        Degrees(const Radians& rad);
        /**
         * @brief デストラクタ
         */
        ~Degrees();
        /**
         * @brief 代入演算子
         */
        Degrees& operator=(const Degrees& deg) &;
        /**
         * @brief 代入演算子
         */
        Degrees& operator=(const Radians& rad) &;
        /**
         * @brief 代入演算子
         */
        Degrees& operator=(Degrees&& deg) & noexcept;
        /**
         * @brief 単項プラス演算子
         */
        Degrees operator+() const;
        /**
         * @brief 単項マイナス演算子
         */
        Degrees operator-() const;
        /**
         * @brief 加算代入演算子
         */
        Degrees& operator+=(const Degrees& deg);
        /**
         * @brief 加算代入演算子
         */
        Degrees& operator+=(float s);
        /**
         * @brief 減算代入演算子
         */
        Degrees& operator-=(const Degrees& deg);
        /**
         * @brief 減算代入演算子
         */
        Degrees& operator-=(float s);
        /**
         * @brief 乗算代入演算子
         */
        Degrees& operator*=(const Degrees& deg);
        /**
         * @brief 乗算代入演算子
         */
        Degrees& operator*=(float s);
        /**
         * @brief 除算代入演算子
         */
        Degrees& operator/=(const Degrees& deg);
        /**
         * @brief 除算代入演算子
         */
        Degrees& operator/=(float s);
        /**
         * @brief 角度を取得する
         */
        float getDeg() const { return mDegree; }
        /**
         * @brief 角度を設定する
         */
        void setDeg(float degree) { mDegree = degree; }
        /**
         * @brief ラジアン角にする
         */
        Radians toRadians() const;
        /**
         * @brief ラジアン角から度数法にする
         */
        void fromRadians(const Radians& rad);
        /**
         * @brief 角度を正規化（0～360）する
         */
        Degrees normalize() const;
        /**
         * @brief floatへのキャスト
         */
        explicit operator float() const noexcept;
        /**
         * @brief ラジアン角へのキャスト
         */
        explicit operator Radians() const noexcept;

    private:
        float mDegree; //!< 角度
    };

    /**
     * @brief 等価比較演算子
     */
    bool operator==(const Degrees& deg1, const Degrees& deg2);
    /**
     * @brief 等価比較演算子
     */
    bool operator!=(const Degrees& deg1, const Degrees& deg2);
    /**
     * @brief 加算演算子
     */
    inline Degrees operator+(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) += deg2;
    }
    /**
     * @brief 加算演算子
     */
    inline Degrees operator+(const Degrees& deg, float s) { return Degrees(deg) += s; }
    /**
     * @brief 加算演算子
     */
    inline Degrees operator+(float s, const Degrees& deg) { return Degrees(deg) += s; }
    /**
     * @brief 減算演算子
     */
    inline Degrees operator-(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) -= deg2;
    }
    /**
     * @brief 減算演算子
     */
    inline Degrees operator-(const Degrees& deg, float s) { return Degrees(deg) -= s; }
    /**
     * @brief 減算演算子
     */
    inline Degrees operator-(float s, const Degrees deg) { return Degrees(deg) -= s; }
    /**
     * @brief 乗算演算子
     */
    inline Degrees operator*(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) *= deg2;
    }
    /**
     * @brief 乗算演算子
     */
    inline Degrees operator*(const Degrees& deg, float s) { return Degrees(deg) *= s; }
    /**
     * @brief 乗算演算子
     */
    inline Degrees operator*(float s, const Degrees& deg) { return Degrees(deg) *= s; }
    /**
     * @brief 除算演算子
     */
    inline Degrees operator/(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) /= deg2;
    }
    /**
     * @brief 除算演算子
     */
    inline Degrees operator/(const Degrees& deg, float s) { return Degrees(deg) /= s; }

    /**
     * @class Radians
     * @brief ラジアン角
     */
    class Radians {
    public:
        Radians();
        /**
         * @brief コンストラクタ
         */
        explicit Radians(float rad);
        /**
         * @brief コンストラクタ
         */
        Radians(const Radians& rad);
        /**
         * @brief コンストラクタ
         */
        Radians(const Degrees& deg);
        /**
         * @brief デストラクタ
         */
        ~Radians();
        /**
         * @brief 代入演算子
         */
        Radians& operator=(const Radians& rad) &;
        /**
         * @brief 代入演算子
         */
        Radians& operator=(const Degrees& deg) &;
        /**
         * @brief 代入演算子
         */
        Radians& operator=(Radians&& deg) & noexcept;
        /**
         * @brief 単項プラス演算子
         */
        Radians operator+() const;
        /**
         * @brief 単項マイナス演算子
         */
        Radians operator-() const;
        /**
         * @brief 加算代入演算子
         */
        Radians& operator+=(const Radians& rad);
        /**
         * @brief 加算代入演算子
         */
        Radians& operator+=(float s);
        /**
         * @brief 減算代入演算子
         */
        Radians& operator-=(const Radians& rad);
        /**
         * @brief 減算代入演算子
         */
        Radians& operator-=(float s);
        /**
         * @brief 乗算代入演算子
         */
        Radians& operator*=(const Radians& rad);
        /**
         * @brief 乗算代入演算子
         */
        Radians& operator*=(float s);
        /**
         * @brief 除算代入演算子
         */
        Radians& operator/=(const Radians& rad);
        /**
         * @brief 除算代入演算子
         */
        Radians& operator/=(float s);
        /**
         * @brief ラジアン角を取得する
         */
        inline float getRad() const { return mRadian; }
        /**
         * @brief ラジアン角をセットする
         */
        inline void setRad(float rad) { mRadian = rad; }
        /**
         * @brief 度数法に変換する
         */
        Degrees toDegree() const;
        /**
         * @brief 度からラジアンにする
         */
        void fromDegree(const Degrees& deg);
        /**
         * @brief floatへのキャスト
         */
        explicit operator float() const noexcept;
        /**
         * @brief 度数法へのキャスト
         */
        explicit operator Degrees() const noexcept;

    private:
        float mRadian; //!< ラジアン角
    };
    /**
     * @brief 等価比較演算子
     */
    inline bool operator==(const Radians& rad1, const Radians& rad2) {
        return rad1.getRad() == rad2.getRad();
    }
    /**
     * @brief 等価比較演算子
     */
    inline bool operator!=(const Radians& rad1, const Radians& rad2) { return !(rad1 == rad2); }
    /**
     * @brief 加算演算子
     */
    inline Radians operator+(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) += rad2;
    }
    /**
     * @brief 加算演算子
     */
    inline Radians operator+(const Radians& rad, float s) { return Radians(rad) += s; }
    /**
     * @brief 加算演算子
     */
    inline Radians operator+(float s, const Radians& rad) { return Radians(rad) += s; }
    /**
     * @brief 減算演算子
     */
    inline Radians operator-(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) -= rad2;
    }
    /**
     * @brief 減算演算子
     */
    inline Radians operator-(const Radians& rad, float s) { return Radians(rad) -= s; }
    /**
     * @brief 減算演算子
     */
    inline Radians operator-(float s, const Radians& rad) { return Radians(rad) -= s; }
    /**
     * @brief 乗算演算子
     */
    inline Radians operator*(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) *= rad2;
    }
    /**
     * @brief 乗算演算子
     */
    inline Radians operator*(const Radians& rad, float s) { return Radians(rad) *= s; }
    /**
     * @brief 乗算演算子
     */
    inline Radians operator*(float s, const Radians& rad) { return Radians(rad) *= s; }
    /**
     * @brief 除算演算子
     */
    inline Radians operator/(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) /= rad2;
    }
    /**
     * @brief 除算演算子
     */
    inline Radians operator/(const Radians& rad, float s) { return Radians(rad) /= s; }
} // namespace Framework::Math
