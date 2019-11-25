/**
 * @file Matrix4x4.h
 * @brief 4x4�s��
 */

#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace Framework::Math {
    /**
     * @class Matrix4x4
     * @brief 4*4�s��
     */
    class Matrix4x4 {
    public:
        std::array<std::array<float, 4>, 4> m; //!< 4x4�s��
    public:
        static const Matrix4x4 IDENTITY; //!< �P�ʍs��
        static const Matrix4x4 ZERO; //!< �[���s��
    public:
        /**
         * @brief �R���X�g���N�^
         */
        Matrix4x4();
        /**
         * @brief �R���X�g���N�^
         */
        Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23,
            float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43,
            float m44);
        /**
         * @brief �R���X�g���N�^
         */
        Matrix4x4(const std::array<std::array<float, 4>, 4>& m);
        /**
         * @brief �R���X�g���N�^
         */
        Matrix4x4(const Matrix4x4& m);
        /**
         * @brief ������Z�q
         */
        Matrix4x4& operator=(const Matrix4x4& mat) &;
        /**
         * @brief ������Z�q
         */
        Matrix4x4& operator=(Matrix4x4&& mat) & noexcept;
        /**
         * @brief �P���v���X���Z�q
         */
        Matrix4x4 operator+();
        /**
         * @brief �P���}�C�i�X���Z�q
         */
        Matrix4x4 operator-();
        /**
         * @brief ���Z������Z�q
         */
        Matrix4x4& operator+=(const Matrix4x4& mat);
        /**
         * @brief ���Z������Z�q
         */
        Matrix4x4& operator-=(const Matrix4x4& mat);
        /**
         * @brief ��Z������Z�q
         */
        Matrix4x4& operator*=(float k);
        /**
         * @brief ��Z������Z�q
         */
        Matrix4x4& operator*=(const Matrix4x4& mat);
        /**
         * @brief ���Z������Z�q
         */
        Matrix4x4& operator/=(float k);
        /**
         * @brief ���s�ړ��s��̍쐬
         * @param v �ړ���
         */
        static Matrix4x4 createTranslate(const Vector3& v);
        /**
         * @brief X����]�s��̍쐬
         * @param rad ��]��
         */
        static Matrix4x4 createRotationX(const Radians& rad);
        /**
         * @brief Y����]�s��̍쐬
         * @param rad ��]��
         */
        static Matrix4x4 createRotationY(const Radians& rad);
        /**
         * @brief Z����]�s��̍쐬
         * @param rad ��]��
         */
        static Matrix4x4 createRotationZ(const Radians& rad);
        /**
         * @brief ��]�s��̍쐬
         * @param r �e���̉�]��
         */
        static Matrix4x4 createRotation(const Vector3& r);
        /**
         * @brief �g��E�k���s��̍쐬
         * @param s �e���̊g��E�k���̑傫��
         */
        static Matrix4x4 createScale(const Vector3& s);
        /**
         * @brief �r���[�s��̍쐬
         * @param eye ���_
         * @param at �����_
         * @param up ������̃x�N�g��
         */
        static Matrix4x4 createView(const Vector3& eye, const Vector3& at, const Vector3& up);
        /**
         * @brief �v���W�F�N�V�����s��̍쐬
         * @param fovY ����p
         * @param aspect �A�X�y�N�g��
         * @param nearZ �ŋߓ_
         * @param farZ �ŉ��_
         */
        static Matrix4x4 createProjection(
            const Radians& fovY, float aspect, float nearZ, float farZ);
        /**
         * @brief �����e�s��̍쐬
         * @param screenSize ��ʂ̑傫��
         */
        static Matrix4x4 createOrthographic(const Vector2& screenSize);
        /**
         * @brief �]�u�s����擾����
         */
        Matrix4x4 transpose() const;
        /**
         * @brief �s�񎮂����߂�
         */
        float determinant() const;
        /**
         * @brief �t�s������߂�
         * @param mat ���߂�s��
         */
        Matrix4x4 inverse() const;
        /**
         * @brief �s��̕��
         * @param mat1 �s��1
         * @param mat2 �s��2
         * @param t ��ԌW��
         */
        static Matrix4x4 lerp(const Matrix4x4& mat1, const Matrix4x4& mat2, float t);
        /**
         * @brief �x�N�g���Ƃ̊|���Z(�@���x�N�g���p�ŉ�]�̂�)
         */
        Vector3 transformNormal(const Vector3& v);

        /**
         * @brief �x�N�g���ƍs��̐ς����߁Aw�ŏ��Z���ꂽ�l��Ԃ�
         */
        static Vector3 multiplyCoord(const Math::Vector3& v, const Math::Matrix4x4& m);
        /**
         * @brief �Y�������Z�q
         */
        std::array<float, 4>& operator[](int n);
    };
    /**
     * @brief ������r���Z�q
     */
    inline bool operator==(const Matrix4x4& m1, const Matrix4x4& m2) { return m1.m == m2.m; }
    /**
     * @brief ������r���Z�q
     */
    inline bool operator!=(const Matrix4x4& m1, const Matrix4x4& m2) { return !(m1 == m2); }
    /**
     * @brief ���Z
     */
    Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
    /**
     * @brief ���Z
     */
    Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
    /**
     * @brief ��Z
     */
    Matrix4x4 operator*(const Matrix4x4& m, float s);
    /**
     * @brief ��Z
     */
    Matrix4x4 operator*(float s, const Matrix4x4& m);
    /**
     * @brief ��Z
     */
    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
    /**
     * @brief ���Z
     */
    Matrix4x4 operator/(const Matrix4x4& m, float s);
} // namespace Framework::Math
