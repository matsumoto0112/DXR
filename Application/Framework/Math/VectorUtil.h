#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MathUtility.h"

namespace Framework::Math {
    /**
    * @class VectorUtil
    * @brief ベクトルのユーティリティクラス
    */
    class VectorUtil {
    public:
        /**
        * @brief 反射ベクトルを計算する
        * @param v 入射ベクトル
        * @param n 法線ベクトル
        */
        static Vector2 reflect(const Vector2& v, const Vector2& n) {
            return v - 2.0f * Vector2::dot(v, n) * n;
        }
        /**
        * @brief 反射ベクトルを計算する
        * @param v 入射ベクトル
        * @param n 法線ベクトル
        */
        static Vector3 reflect(const Vector3& v, const Vector3& n) {
            return v - 2.0f * Vector3::dot(v, n) * n;
        }
        /**
        * @brief ベクトルの各要素の積を返す
        */
        static Vector2 mulEach(const Vector2& v1, const Vector2& v2) {
            return Vector2(v1.x * v2.x, v1.y * v2.y);
        }
        /**
        * @brief ベクトルの各要素の積を返す
        */
        static Vector3 mulEach(const Vector3& v1, const Vector3& v2) {
            return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
        }
        /**
        * @brief ベクトルの各要素の積を返す
        */
        static Vector4 mulEach(const Vector4& v1, const Vector4& v2) {
            return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
        }
    };
} //Framework::Math 