#pragma once

#include <array>

namespace Framework::Utility {
    /**
    * @class Color4
    * @brief �F
    */
    class Color4 {
    public:
        float r; //!< r����
        float g; //!< g����
        float b; //!< b����
        float a; //!< a����
    public:
        static const Color4 BLACK; //!< ��
        static const Color4 WHITE; //!< ��
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Color4();
        /**
        * @brief �R���X�g���N�^
        * @param r r����(0.0�`1.0)
        * @param g g����(0.0�`1.0)
        * @param b b����(0.0�`1.0)
        * @param a a����(0.0�`1.0)
        */
        Color4(float r, float g, float b, float a);
        /**
        * @brief �R���X�g���N�^
        * @param color �F�z��
        */
        Color4(float color[4]);
        /**
        * @brief �R�s�[�R���X�g���N�^
        */
        Color4(const Color4& c);
        /**
        * @brief �f�X�g���N�^
        */
        ~Color4();
        /**
        * @brief ������Z�q
        */
        Color4& operator=(const Color4& c) &;
        /**
        * @brief ������Z�q
        */
        Color4& operator=(Color4&& c) & noexcept;
        /**
        * v�P���v���X���Z�q
        */
        Color4 operator+();
        /**
        * @brief �P���}�C�i�X���Z�q
        */
        Color4 operator-();
        /**
        * @brief ���Z������Z�q
        */
        Color4& operator +=(const Color4& c);
        /**
        * @brief ���Z������Z�q
        */
        Color4& operator -=(const Color4& c);
        /**
        * @brief ��Z������Z�q
        */
        Color4& operator *=(const Color4& c);
        /**
        * @brief ��Z������Z�q
        */
        Color4& operator *=(float s);
        /**
        * @brief ���Z������Z�q
        */
        Color4& operator /=(float s);

        /**
        * @brief �z��̎擾
        * @param c �߂�l
        */
        void get(float c[4]) const;
        /**
        * @brief �F��z��Ŏ擾
        * @return r,g,b,a�̏��Ɋi�[���ꂽ�z��
        */
        std::array<float, 4> get() const;
        /**
        * @brief 0�`1�̊ԂɃN�����v����
        */
        Color4& saturate();
        /**
        * @brief ���
        * @param a �F1
        * @param b �F2
        * @param t ��ԌW��
        */
        static Color4 lerp(const Color4& a, const Color4& b, float t);
        /**
        * @brief �O���[�X�P�[���ɕϊ�����
        */
        static Color4 grayScale(const Color4& c);
    };
    /**
    * @brief ������r
    */
    bool operator ==(const Color4& c1, const Color4& c2);
    /**
    * @brief ������r
    */
    bool operator !=(const Color4& c1, const Color4& c2);

    /**
    * @brief ���Z
    */
    Color4 operator +(const Color4& c1, const Color4& c2);
    /**
    * @brief ���Z
    */
    Color4 operator -(const Color4& c1, const Color4& c2);
    /**
    * @brief ��Z
    */
    Color4 operator *(const Color4& c1, const Color4& c2);
    /**
    * @brief ��Z
    */
    Color4 operator *(const Color4& c, float s);
    /**
    * @brief ��Z
    */
    Color4 operator *(float s, const Color4& c);
    /**
    * @brief ���Z
    */
    Color4 operator /(const Color4& c, float s);
} //Framework::Utility
