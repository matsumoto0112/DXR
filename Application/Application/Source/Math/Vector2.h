#pragma once

namespace Framework::Math {
    /**
    * @class Vector2
    * @brief Vector2�N���X
    */
    class Vector2 {
    public:
        float x; //!< x����
        float y; //!< y����
    public:
        static const Vector2 ZERO; //!< (0,0)
        static const Vector2 LEFT; //!< (-1,0)
        static const Vector2 RIGHT; //!< (1,0)
        static const Vector2 UP; //!< (0,1)
        static const Vector2 DOWN; //!< (0,-1)
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Vector2();
        /**
        * @brief �R���X�g���N�^
        */
        Vector2(float x, float y);
        /**
        * @brief ����v�f�ŏ�����
        */
        Vector2(float v);
        /**
        * @brief �R�s�[�R���X�g���N�^
        */
        Vector2(const Vector2& v);

        /**
        * @brief ������Z�q
        */
        Vector2& operator=(const Vector2& v)&;
        /**
        * @brief ������Z�q
        */
        Vector2& operator=(Vector2&& v) & noexcept;
        /**
        * @brief �P���v���X���Z�q
        */
        Vector2 operator+() const;
        /**
        * @brief �P���}�C�i�X���Z�q
        */
        Vector2 operator-() const;
        /**
        * @brief ���Z������Z�q
        */
        Vector2& operator +=(const Vector2& v);
        /**
        * @brief ���Z������Z�q
        */
        Vector2& operator -=(const Vector2& v);
        /**
        * @brief ��Z������Z�q
        */
        Vector2& operator *=(float s);
        /**
        * @brief ���Z������Z�q
        */
        Vector2& operator /=(float s);
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
        Vector2 normalized() const;
        /**
        * @brief �x�N�g���𐳋K������
        */
        static Vector2 normalize(const Vector2& v);
        /**
        * @brief ����
        * @param v1 �x�N�g��1
        * @param v2 �x�N�g��2
        */
        static float dot(const Vector2& v1, const Vector2& v2);
        /**
        * @brief �O��
        * @param v1 �x�N�g��1
        * @param v2 �x�N�g��2
        */
        static float cross(const Vector2& v1, const Vector2& v2);
    };

    /**
    * @brief ���l���Z�q
    */
    bool operator ==(const Vector2& v1, const Vector2& v2);
    /**
    * @brief ���l���Z�q
    */
    bool operator !=(const Vector2& v1, const Vector2& v2);
    /**
    * @brief ���Z
    */
    Vector2 operator +(const Vector2& v1, const Vector2& v2);
    /**
    * @brief ���Z
    */
    Vector2 operator -(const Vector2& v1, const Vector2& v2);
    /**
    * @brief ��Z
    */
    Vector2 operator *(const Vector2& v, float s);
    /**
    * @brief ��Z
    */
    Vector2 operator *(float s, const Vector2& v);
    /**
    * @brief ���Z
    */
    Vector2 operator /(const Vector2& v, float s);

} //Framework::Math