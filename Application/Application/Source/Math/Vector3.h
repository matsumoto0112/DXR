#pragma once

namespace Framework::Math {
    /**
    * @class Vector3
    * @brief Vector3クラス
    */
    class Vector3 {
    public:
        float x; //!< x
        float y; //!< y
        float z; //!< z
    public:
        static const Vector3 ZERO; //!< (0,0,0)
        static const Vector3 LEFT; //!< (-1,0,0)
        static const Vector3 RIGHT; //!< (1,0,0)
        static const Vector3 UP; //!< (0,1,0)
        static const Vector3 DOWN; //!< (0,-1,0)
        static const Vector3 FORWORD; //!< (0,0,1)
        static const Vector3 BACK; //!< (0,0,-1)
    public:
        /**
        * @brief コンストラクタ
        */
        Vector3();
        /**
        * @brief コンストラクタ
        */
        Vector3(float x, float y, float z);
        /**
        * @brief 同一の要素で初期化する
        */
        Vector3(float s);
        /**
        * @brief コピーコンストラクタ
        */
        Vector3(const Vector3& v);
        /**
        * @brief 代入演算子
        */
        Vector3& operator =(const Vector3& a) &;
        /**
        * @brief 代入演算子
        */
        Vector3& operator=(Vector3&& v) & noexcept;
        /**
        * @brief 単項プラス演算子
        */
        Vector3 operator +() const;
        /**
        * @brief 単項マイナス演算子
        */
        Vector3 operator -() const;
        /**
        * @brief 加算代入演算子
        */
        Vector3& operator +=(const Vector3& a);
        /**
        * @brief 減算代入演算子
        */
        Vector3& operator -=(const Vector3& a);
        /**
        * @brief 乗算代入演算子
        */
        Vector3& operator *=(float a);
        /**
        * @brief 除算代入演算子
        */
        Vector3& operator /=(float a);
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
        Vector3 normalized() const;
        /**
        * @brief ベクトルを正規化する
        */
        static Vector3 normalize(const Vector3& v);
        /**
        * @brief 内積
        * @param a aベクトル
        * @param b bベクトル
        */
        static float dot(const Vector3& a, const Vector3& b);
        /**
        * @brief 外積
        * @param a aベクトル
        * @param b bベクトル
        */
        static Vector3 cross(const Vector3& a, const Vector3& b);
    };
    /**
    * @brief 等価比較
    */
    bool operator ==(const Vector3& v1, const Vector3& v2);
    /**
    * @brief 等価比較
    */
    bool operator !=(const Vector3& v1, const Vector3& v2);
    /**
    * @brief 加算
    */
    Vector3 operator +(const Vector3& v1, const Vector3& v2);
    /**
    * @brief 減算
    */
    Vector3 operator -(const Vector3& v1, const Vector3& v2);
    /**
    * @brief 乗算
    */
    Vector3 operator *(const Vector3& v, float s);
    /**
    * @brief 乗算
    */
    Vector3 operator *(float s, const Vector3& v);
    /**
    * @brief 除算
    */
    Vector3 operator /(const Vector3& v, float s);

} //Framework::Math