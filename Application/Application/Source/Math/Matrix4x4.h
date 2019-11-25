/**
 * @file Matrix4x4.h
 * @brief 4x4行列
 */

#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace Framework::Math {
    /**
     * @class Matrix4x4
     * @brief 4*4行列
     */
    class Matrix4x4 {
    public:
        std::array<std::array<float, 4>, 4> m; //!< 4x4行列
    public:
        static const Matrix4x4 IDENTITY; //!< 単位行列
        static const Matrix4x4 ZERO; //!< ゼロ行列
    public:
        /**
         * @brief コンストラクタ
         */
        Matrix4x4();
        /**
         * @brief コンストラクタ
         */
        Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23,
            float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43,
            float m44);
        /**
         * @brief コンストラクタ
         */
        Matrix4x4(const std::array<std::array<float, 4>, 4>& m);
        /**
         * @brief コンストラクタ
         */
        Matrix4x4(const Matrix4x4& m);
        /**
         * @brief 代入演算子
         */
        Matrix4x4& operator=(const Matrix4x4& mat) &;
        /**
         * @brief 代入演算子
         */
        Matrix4x4& operator=(Matrix4x4&& mat) & noexcept;
        /**
         * @brief 単項プラス演算子
         */
        Matrix4x4 operator+();
        /**
         * @brief 単項マイナス演算子
         */
        Matrix4x4 operator-();
        /**
         * @brief 加算代入演算子
         */
        Matrix4x4& operator+=(const Matrix4x4& mat);
        /**
         * @brief 減算代入演算子
         */
        Matrix4x4& operator-=(const Matrix4x4& mat);
        /**
         * @brief 乗算代入演算子
         */
        Matrix4x4& operator*=(float k);
        /**
         * @brief 乗算代入演算子
         */
        Matrix4x4& operator*=(const Matrix4x4& mat);
        /**
         * @brief 除算代入演算子
         */
        Matrix4x4& operator/=(float k);
        /**
         * @brief 平行移動行列の作成
         * @param v 移動量
         */
        static Matrix4x4 createTranslate(const Vector3& v);
        /**
         * @brief X軸回転行列の作成
         * @param rad 回転量
         */
        static Matrix4x4 createRotationX(const Radians& rad);
        /**
         * @brief Y軸回転行列の作成
         * @param rad 回転量
         */
        static Matrix4x4 createRotationY(const Radians& rad);
        /**
         * @brief Z軸回転行列の作成
         * @param rad 回転量
         */
        static Matrix4x4 createRotationZ(const Radians& rad);
        /**
         * @brief 回転行列の作成
         * @param r 各軸の回転量
         */
        static Matrix4x4 createRotation(const Vector3& r);
        /**
         * @brief 拡大・縮小行列の作成
         * @param s 各軸の拡大・縮小の大きさ
         */
        static Matrix4x4 createScale(const Vector3& s);
        /**
         * @brief ビュー行列の作成
         * @param eye 視点
         * @param at 注視点
         * @param up 上方向のベクトル
         */
        static Matrix4x4 createView(const Vector3& eye, const Vector3& at, const Vector3& up);
        /**
         * @brief プロジェクション行列の作成
         * @param fovY 視野角
         * @param aspect アスペクト比
         * @param nearZ 最近点
         * @param farZ 最遠点
         */
        static Matrix4x4 createProjection(
            const Radians& fovY, float aspect, float nearZ, float farZ);
        /**
         * @brief 正投影行列の作成
         * @param screenSize 画面の大きさ
         */
        static Matrix4x4 createOrthographic(const Vector2& screenSize);
        /**
         * @brief 転置行列を取得する
         */
        Matrix4x4 transpose() const;
        /**
         * @brief 行列式を求める
         */
        float determinant() const;
        /**
         * @brief 逆行列を求める
         * @param mat 求める行列
         */
        Matrix4x4 inverse() const;
        /**
         * @brief 行列の補間
         * @param mat1 行列1
         * @param mat2 行列2
         * @param t 補間係数
         */
        static Matrix4x4 lerp(const Matrix4x4& mat1, const Matrix4x4& mat2, float t);
        /**
         * @brief ベクトルとの掛け算(法線ベクトル用で回転のみ)
         */
        Vector3 transformNormal(const Vector3& v);

        /**
         * @brief ベクトルと行列の積を求め、wで除算された値を返す
         */
        static Vector3 multiplyCoord(const Math::Vector3& v, const Math::Matrix4x4& m);
        /**
         * @brief 添え字演算子
         */
        std::array<float, 4>& operator[](int n);
    };
    /**
     * @brief 等価比較演算子
     */
    inline bool operator==(const Matrix4x4& m1, const Matrix4x4& m2) { return m1.m == m2.m; }
    /**
     * @brief 等価比較演算子
     */
    inline bool operator!=(const Matrix4x4& m1, const Matrix4x4& m2) { return !(m1 == m2); }
    /**
     * @brief 加算
     */
    Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
    /**
     * @brief 減算
     */
    Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
    /**
     * @brief 乗算
     */
    Matrix4x4 operator*(const Matrix4x4& m, float s);
    /**
     * @brief 乗算
     */
    Matrix4x4 operator*(float s, const Matrix4x4& m);
    /**
     * @brief 乗算
     */
    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
    /**
     * @brief 除算
     */
    Matrix4x4 operator/(const Matrix4x4& m, float s);
} // namespace Framework::Math
