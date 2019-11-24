#pragma once
#include "Vector3.h"
#include "Angle.h"

namespace Framework::Math {
    /**
    * @class Quaternion
    * @brief 四元数
    */
    class Quaternion {
    public:
        float x;
        float y;
        float z;
        float w;
    public:

    public:
        /**
        * @brief コンストラクタ
        */
        Quaternion();
        /**
        * @brief コンストラクタ
        */
        Quaternion(float x, float y, float z, float w);
        /**
        * @brief コンストラクタ
        * @param nv 正規化された回転軸
        * @param angle 回転量
        */
        Quaternion(const Vector3& nv, const Radians& angle);
        /**
        * @brief デストラクタ
        */
        ~Quaternion();
        /**
        * @brief 正規化四元数を取得する
        */
        Quaternion normalized() const;
        /**
        * @brief 四元数を正規化する
        */
        static Quaternion normalize(const Quaternion& q);
        /**
        * @brief 共役の四元数を求める
        */
        Quaternion conjugate() const;
        /**
        * @brief ロール・ピッチ・ヨーから四元数を生成する
        */
        static Quaternion fromEular(const Radians& roll, const Radians& pitch, const Radians& yaw);
        /**
        * @brief ロール・ピッチ・ヨーから四元数を生成する
        */
        static Quaternion fromEular(const Vector3& v);
        /**
        * @brief 四元数をオイラー角に変換する
        */
        static Vector3 toEular(const Quaternion& q);
    private:

    };
    /**
    * @brief 乗算演算子
    */
    Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
    /**
    * @brief ベクトルとの積
    */
    Vector3 operator*(const Quaternion& q, const Vector3& v);
} //Framework::Math
