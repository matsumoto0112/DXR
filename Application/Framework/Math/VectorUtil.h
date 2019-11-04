#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MathUtility.h"

namespace Framework::Math {
    /**
    * @class VectorUtil
    * @brief �x�N�g���̃��[�e�B���e�B�N���X
    */
    class VectorUtil {
    public:
        /**
        * @brief ���˃x�N�g�����v�Z����
        * @param v ���˃x�N�g��
        * @param n �@���x�N�g��
        */
        static Vector2 reflect(const Vector2& v, const Vector2& n) {
            return v - 2.0f * Vector2::dot(v, n) * n;
        }
        /**
        * @brief ���˃x�N�g�����v�Z����
        * @param v ���˃x�N�g��
        * @param n �@���x�N�g��
        */
        static Vector3 reflect(const Vector3& v, const Vector3& n) {
            return v - 2.0f * Vector3::dot(v, n) * n;
        }
        /**
        * @brief �x�N�g���̊e�v�f�̐ς�Ԃ�
        */
        static Vector2 mulEach(const Vector2& v1, const Vector2& v2) {
            return Vector2(v1.x * v2.x, v1.y * v2.y);
        }
        /**
        * @brief �x�N�g���̊e�v�f�̐ς�Ԃ�
        */
        static Vector3 mulEach(const Vector3& v1, const Vector3& v2) {
            return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
        }
        /**
        * @brief �x�N�g���̊e�v�f�̐ς�Ԃ�
        */
        static Vector4 mulEach(const Vector4& v1, const Vector4& v2) {
            return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
        }
    };
} //Framework::Math 