#pragma once

namespace Framework::Math {
    /**
    * @class Vector2
    * @brief Vector2クラス
    */
    class Vector2 {
    public:
        float x; //!< x成分
        float y; //!< y成分
    public:
        static const Vector2 ZERO; //!< (0,0)
        static const Vector2 LEFT; //!< (-1,0)
        static const Vector2 RIGHT; //!< (1,0)
        static const Vector2 UP; //!< (0,1)
        static const Vector2 DOWN; //!< (0,-1)
    public:
        /**
        * @brief コンストラクタ
        */
        Vector2();
        /**
        * @brief コンストラクタ
        */
        Vector2(float x, float y);
        /**
        * @brief 同一要素で初期化
        */
        Vector2(float v);
        /**
        * @brief コピーコンストラクタ
        */
        Vector2(const Vector2& v);

        /**
        * @brief 代入演算子
        */
        Vector2& operator=(const Vector2& v)&;
        /**
        * @brief 代入演算子
        */
        Vector2& operator=(Vector2&& v) & noexcept;
        /**
        * @brief 単項プラス演算子
        */
        Vector2 operator+() const;
        /**
        * @brief 単項マイナス演算子
        */
        Vector2 operator-() const;
        /**
        * @brief 加算代入演算子
        */
        Vector2& operator +=(const Vector2& v);
        /**
        * @brief 減算代入演算子
        */
        Vector2& operator -=(const Vector2& v);
        /**
        * @brief 乗算代入演算子
        */
        Vector2& operator *=(float s);
        /**
        * @brief 除算代入演算子
        */
        Vector2& operator /=(float s);
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
        * @brief 正規化ベクトルを取得する
        */
        Vector2 normalized() const;
        /**
        * @brief ベクトルを正規化する
        */
        static Vector2 normalize(const Vector2& v);
        /**
        * @brief 内積
        * @param v1 ベクトル1
        * @param v2 ベクトル2
        */
        static float dot(const Vector2& v1, const Vector2& v2);
        /**
        * @brief 外積
        * @param v1 ベクトル1
        * @param v2 ベクトル2
        */
        static float cross(const Vector2& v1, const Vector2& v2);
    };

    /**
    * @brief 等値演算子
    */
    bool operator ==(const Vector2& v1, const Vector2& v2);
    /**
    * @brief 等値演算子
    */
    bool operator !=(const Vector2& v1, const Vector2& v2);
    /**
    * @brief 加算
    */
    Vector2 operator +(const Vector2& v1, const Vector2& v2);
    /**
    * @brief 減算
    */
    Vector2 operator -(const Vector2& v1, const Vector2& v2);
    /**
    * @brief 乗算
    */
    Vector2 operator *(const Vector2& v, float s);
    /**
    * @brief 乗算
    */
    Vector2 operator *(float s, const Vector2& v);
    /**
    * @brief 除算
    */
    Vector2 operator /(const Vector2& v, float s);

} //Framework::Math