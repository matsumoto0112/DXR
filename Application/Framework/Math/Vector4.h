#pragma once
#include "Vector3.h"

namespace Framework::Math {
    /**
    * @class Vector4
    * @brief 4次元ベクトル
    */
    class Vector4 {
    public:
        float x; //!< x成分
        float y; //!< y成分
        float z; //!< z成分
        float w; //!< w成分
    public:
        static const Vector4 ZERO; //!< (0,0,0,0)
    public:
        /**
    * @brief コンストラクタ
    */
        Vector4();
        /**
        * @brief コンストラクタ
        */
        Vector4(float x, float y, float z, float w);
        /**
        * @brief Vector3で初期化する
        */
        Vector4(const Vector3& v, float w);
        /**
        * @brief 同一の要素で初期化する
        */
        Vector4(float s);
        /**
        * @brief コピーコンストラクタ
        */
        Vector4(const Vector4& v);
        /**
        * @brief 代入演算子
        */
        Vector4& operator =(const Vector4& a) &;
        /**
        * @brief 代入演算子
        */
        Vector4& operator =(Vector4&& v) & noexcept;
        /**
        * @brief 単項プラス演算子
        */
        Vector4 operator +() const;
        /**
        * @brief 単項マイナス演算子
        */
        Vector4 operator -() const;
        /**
        * @brief 加算代入演算子
        */
        Vector4& operator +=(const Vector4& a);
        /**
        * @brief 減算代入演算子
        */
        Vector4& operator -=(const Vector4& a);
        /**
        * @brief 乗算代入演算子
        */
        Vector4& operator *=(float a);
        /**
        * @brief 除算代入演算子
        */
        Vector4& operator /=(float a);
        /**
        * @brief 大きさの2乗を返す
        * @return ベクトルの大きさの2乗
        */
        float lengthSquared() const;
        /**
        * @brief 大きさを返す
        * @return ベクトルの大きさ
        */
        float length() const;
        /**
        * @brief ベクトルを正規化する
        */
        Vector4 normalized() const;
        /**
        * @brief ベクトルの正規化
        * @return 正規化されたベクトルを返す
        */
        static Vector4 normalize(const Vector4& v);
        /**
        * @brief 内積
        * @param v1 ベクトル1
        * @param v2 ベクトル2
        */
        static float dot(const Vector4& v1, const Vector4& v2);
    };

    /**
    * @brief 等値演算子
    */
    bool operator ==(const Vector4& v1, const Vector4& v2);
    /**
    * @brief 等値演算子
    */
    bool operator !=(const Vector4& v1, const Vector4& v2);
    /**
    * @brief 加算
    */
    Vector4 operator +(const Vector4& v1, const Vector4& v2);
    /**
    * @brief 減算
    */
    Vector4 operator -(const Vector4& v1, const Vector4& v2);
    /**
    * @brief 乗算
    */
    Vector4 operator *(const Vector4& v, float s);
    /**
    * @brief 乗算
    */
    Vector4 operator *(float s, const Vector4& v);
    /**
    * @brief 除算
    */
    Vector4 operator /(const Vector4& v, float s);

} //Framework::Math
