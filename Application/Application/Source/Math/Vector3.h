#pragma once

namespace Framework::Math {
    /**
    * @class Vector3
    * @brief Vector3�N���X
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
        * @brief �R���X�g���N�^
        */
        Vector3();
        /**
        * @brief �R���X�g���N�^
        */
        Vector3(float x, float y, float z);
        /**
        * @brief ����̗v�f�ŏ���������
        */
        Vector3(float s);
        /**
        * @brief �R�s�[�R���X�g���N�^
        */
        Vector3(const Vector3& v);
        /**
        * @brief ������Z�q
        */
        Vector3& operator =(const Vector3& a) &;
        /**
        * @brief ������Z�q
        */
        Vector3& operator=(Vector3&& v) & noexcept;
        /**
        * @brief �P���v���X���Z�q
        */
        Vector3 operator +() const;
        /**
        * @brief �P���}�C�i�X���Z�q
        */
        Vector3 operator -() const;
        /**
        * @brief ���Z������Z�q
        */
        Vector3& operator +=(const Vector3& a);
        /**
        * @brief ���Z������Z�q
        */
        Vector3& operator -=(const Vector3& a);
        /**
        * @brief ��Z������Z�q
        */
        Vector3& operator *=(float a);
        /**
        * @brief ���Z������Z�q
        */
        Vector3& operator /=(float a);
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
        * @brief ���K���x�N�g�����擾����
        */
        Vector3 normalized() const;
        /**
        * @brief �x�N�g���𐳋K������
        */
        static Vector3 normalize(const Vector3& v);
        /**
        * @brief ����
        * @param a a�x�N�g��
        * @param b b�x�N�g��
        */
        static float dot(const Vector3& a, const Vector3& b);
        /**
        * @brief �O��
        * @param a a�x�N�g��
        * @param b b�x�N�g��
        */
        static Vector3 cross(const Vector3& a, const Vector3& b);
    };
    /**
    * @brief ������r
    */
    bool operator ==(const Vector3& v1, const Vector3& v2);
    /**
    * @brief ������r
    */
    bool operator !=(const Vector3& v1, const Vector3& v2);
    /**
    * @brief ���Z
    */
    Vector3 operator +(const Vector3& v1, const Vector3& v2);
    /**
    * @brief ���Z
    */
    Vector3 operator -(const Vector3& v1, const Vector3& v2);
    /**
    * @brief ��Z
    */
    Vector3 operator *(const Vector3& v, float s);
    /**
    * @brief ��Z
    */
    Vector3 operator *(float s, const Vector3& v);
    /**
    * @brief ���Z
    */
    Vector3 operator /(const Vector3& v, float s);

} //Framework::Math