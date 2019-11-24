#pragma once
#include "Vector3.h"
#include "Angle.h"

namespace Framework::Math {
    /**
    * @class Quaternion
    * @brief �l����
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
        * @brief �R���X�g���N�^
        */
        Quaternion();
        /**
        * @brief �R���X�g���N�^
        */
        Quaternion(float x, float y, float z, float w);
        /**
        * @brief �R���X�g���N�^
        * @param nv ���K�����ꂽ��]��
        * @param angle ��]��
        */
        Quaternion(const Vector3& nv, const Radians& angle);
        /**
        * @brief �f�X�g���N�^
        */
        ~Quaternion();
        /**
        * @brief ���K���l�������擾����
        */
        Quaternion normalized() const;
        /**
        * @brief �l�����𐳋K������
        */
        static Quaternion normalize(const Quaternion& q);
        /**
        * @brief �����̎l���������߂�
        */
        Quaternion conjugate() const;
        /**
        * @brief ���[���E�s�b�`�E���[����l�����𐶐�����
        */
        static Quaternion fromEular(const Radians& roll, const Radians& pitch, const Radians& yaw);
        /**
        * @brief ���[���E�s�b�`�E���[����l�����𐶐�����
        */
        static Quaternion fromEular(const Vector3& v);
        /**
        * @brief �l�������I�C���[�p�ɕϊ�����
        */
        static Vector3 toEular(const Quaternion& q);
    private:

    };
    /**
    * @brief ��Z���Z�q
    */
    Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
    /**
    * @brief �x�N�g���Ƃ̐�
    */
    Vector3 operator*(const Quaternion& q, const Vector3& v);
} //Framework::Math
