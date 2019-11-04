#pragma once
#include "Vector3.h"

namespace Framework::Math {
    /**
    * @class Vector4
    * @brief 4�����x�N�g��
    */
    class Vector4 {
    public:
        float x; //!< x����
        float y; //!< y����
        float z; //!< z����
        float w; //!< w����
    public:
        static const Vector4 ZERO; //!< (0,0,0,0)
    public:
        /**
    * @brief �R���X�g���N�^
    */
        Vector4();
        /**
        * @brief �R���X�g���N�^
        */
        Vector4(float x, float y, float z, float w);
        /**
        * @brief Vector3�ŏ���������
        */
        Vector4(const Vector3& v, float w);
        /**
        * @brief ����̗v�f�ŏ���������
        */
        Vector4(float s);
        /**
        * @brief �R�s�[�R���X�g���N�^
        */
        Vector4(const Vector4& v);
        /**
        * @brief ������Z�q
        */
        Vector4& operator =(const Vector4& a) &;
        /**
        * @brief ������Z�q
        */
        Vector4& operator =(Vector4&& v) & noexcept;
        /**
        * @brief �P���v���X���Z�q
        */
        Vector4 operator +() const;
        /**
        * @brief �P���}�C�i�X���Z�q
        */
        Vector4 operator -() const;
        /**
        * @brief ���Z������Z�q
        */
        Vector4& operator +=(const Vector4& a);
        /**
        * @brief ���Z������Z�q
        */
        Vector4& operator -=(const Vector4& a);
        /**
        * @brief ��Z������Z�q
        */
        Vector4& operator *=(float a);
        /**
        * @brief ���Z������Z�q
        */
        Vector4& operator /=(float a);
        /**
        * @brief �傫����2���Ԃ�
        * @return �x�N�g���̑傫����2��
        */
        float lengthSquared() const;
        /**
        * @brief �傫����Ԃ�
        * @return �x�N�g���̑傫��
        */
        float length() const;
        /**
        * @brief �x�N�g���𐳋K������
        */
        Vector4 normalized() const;
        /**
        * @brief �x�N�g���̐��K��
        * @return ���K�����ꂽ�x�N�g����Ԃ�
        */
        static Vector4 normalize(const Vector4& v);
        /**
        * @brief ����
        * @param v1 �x�N�g��1
        * @param v2 �x�N�g��2
        */
        static float dot(const Vector4& v1, const Vector4& v2);
    };

    /**
    * @brief ���l���Z�q
    */
    bool operator ==(const Vector4& v1, const Vector4& v2);
    /**
    * @brief ���l���Z�q
    */
    bool operator !=(const Vector4& v1, const Vector4& v2);
    /**
    * @brief ���Z
    */
    Vector4 operator +(const Vector4& v1, const Vector4& v2);
    /**
    * @brief ���Z
    */
    Vector4 operator -(const Vector4& v1, const Vector4& v2);
    /**
    * @brief ��Z
    */
    Vector4 operator *(const Vector4& v, float s);
    /**
    * @brief ��Z
    */
    Vector4 operator *(float s, const Vector4& v);
    /**
    * @brief ���Z
    */
    Vector4 operator /(const Vector4& v, float s);

} //Framework::Math
