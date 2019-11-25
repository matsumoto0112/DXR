/**
 * @file Angle.h
 * @brief �x���@�E�ʓx�@�̊p�x�𖾎��I�ɂ���N���X
 */

#pragma once

namespace Framework::Math {
    class Radians;
    /**
     * @class Degrees
     * @brief �x���@�ɂ��p�x
     */
    class Degrees {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        Degrees();
        /**
         * @brief �R���X�g���N�^
         */
        explicit Degrees(float degree);
        /**
         * @brief �R�s�[�R���X�g���N�^
         */
        Degrees(const Degrees& deg);
        /**
         * @brief �R���X�g���N�^
         */
        Degrees(const Radians& rad);
        /**
         * @brief �f�X�g���N�^
         */
        ~Degrees();
        /**
         * @brief ������Z�q
         */
        Degrees& operator=(const Degrees& deg) &;
        /**
         * @brief ������Z�q
         */
        Degrees& operator=(const Radians& rad) &;
        /**
         * @brief ������Z�q
         */
        Degrees& operator=(Degrees&& deg) & noexcept;
        /**
         * @brief �P���v���X���Z�q
         */
        Degrees operator+() const;
        /**
         * @brief �P���}�C�i�X���Z�q
         */
        Degrees operator-() const;
        /**
         * @brief ���Z������Z�q
         */
        Degrees& operator+=(const Degrees& deg);
        /**
         * @brief ���Z������Z�q
         */
        Degrees& operator+=(float s);
        /**
         * @brief ���Z������Z�q
         */
        Degrees& operator-=(const Degrees& deg);
        /**
         * @brief ���Z������Z�q
         */
        Degrees& operator-=(float s);
        /**
         * @brief ��Z������Z�q
         */
        Degrees& operator*=(const Degrees& deg);
        /**
         * @brief ��Z������Z�q
         */
        Degrees& operator*=(float s);
        /**
         * @brief ���Z������Z�q
         */
        Degrees& operator/=(const Degrees& deg);
        /**
         * @brief ���Z������Z�q
         */
        Degrees& operator/=(float s);
        /**
         * @brief �p�x���擾����
         */
        float getDeg() const { return mDegree; }
        /**
         * @brief �p�x��ݒ肷��
         */
        void setDeg(float degree) { mDegree = degree; }
        /**
         * @brief ���W�A���p�ɂ���
         */
        Radians toRadians() const;
        /**
         * @brief ���W�A���p����x���@�ɂ���
         */
        void fromRadians(const Radians& rad);
        /**
         * @brief �p�x�𐳋K���i0�`360�j����
         */
        Degrees normalize() const;
        /**
         * @brief float�ւ̃L���X�g
         */
        explicit operator float() const noexcept;
        /**
         * @brief ���W�A���p�ւ̃L���X�g
         */
        explicit operator Radians() const noexcept;

    private:
        float mDegree; //!< �p�x
    };

    /**
     * @brief ������r���Z�q
     */
    bool operator==(const Degrees& deg1, const Degrees& deg2);
    /**
     * @brief ������r���Z�q
     */
    bool operator!=(const Degrees& deg1, const Degrees& deg2);
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator+(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) += deg2;
    }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator+(const Degrees& deg, float s) { return Degrees(deg) += s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator+(float s, const Degrees& deg) { return Degrees(deg) += s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator-(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) -= deg2;
    }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator-(const Degrees& deg, float s) { return Degrees(deg) -= s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator-(float s, const Degrees deg) { return Degrees(deg) -= s; }
    /**
     * @brief ��Z���Z�q
     */
    inline Degrees operator*(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) *= deg2;
    }
    /**
     * @brief ��Z���Z�q
     */
    inline Degrees operator*(const Degrees& deg, float s) { return Degrees(deg) *= s; }
    /**
     * @brief ��Z���Z�q
     */
    inline Degrees operator*(float s, const Degrees& deg) { return Degrees(deg) *= s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator/(const Degrees& deg1, const Degrees& deg2) {
        return Degrees(deg1) /= deg2;
    }
    /**
     * @brief ���Z���Z�q
     */
    inline Degrees operator/(const Degrees& deg, float s) { return Degrees(deg) /= s; }

    /**
     * @class Radians
     * @brief ���W�A���p
     */
    class Radians {
    public:
        Radians();
        /**
         * @brief �R���X�g���N�^
         */
        explicit Radians(float rad);
        /**
         * @brief �R���X�g���N�^
         */
        Radians(const Radians& rad);
        /**
         * @brief �R���X�g���N�^
         */
        Radians(const Degrees& deg);
        /**
         * @brief �f�X�g���N�^
         */
        ~Radians();
        /**
         * @brief ������Z�q
         */
        Radians& operator=(const Radians& rad) &;
        /**
         * @brief ������Z�q
         */
        Radians& operator=(const Degrees& deg) &;
        /**
         * @brief ������Z�q
         */
        Radians& operator=(Radians&& deg) & noexcept;
        /**
         * @brief �P���v���X���Z�q
         */
        Radians operator+() const;
        /**
         * @brief �P���}�C�i�X���Z�q
         */
        Radians operator-() const;
        /**
         * @brief ���Z������Z�q
         */
        Radians& operator+=(const Radians& rad);
        /**
         * @brief ���Z������Z�q
         */
        Radians& operator+=(float s);
        /**
         * @brief ���Z������Z�q
         */
        Radians& operator-=(const Radians& rad);
        /**
         * @brief ���Z������Z�q
         */
        Radians& operator-=(float s);
        /**
         * @brief ��Z������Z�q
         */
        Radians& operator*=(const Radians& rad);
        /**
         * @brief ��Z������Z�q
         */
        Radians& operator*=(float s);
        /**
         * @brief ���Z������Z�q
         */
        Radians& operator/=(const Radians& rad);
        /**
         * @brief ���Z������Z�q
         */
        Radians& operator/=(float s);
        /**
         * @brief ���W�A���p���擾����
         */
        inline float getRad() const { return mRadian; }
        /**
         * @brief ���W�A���p���Z�b�g����
         */
        inline void setRad(float rad) { mRadian = rad; }
        /**
         * @brief �x���@�ɕϊ�����
         */
        Degrees toDegree() const;
        /**
         * @brief �x���烉�W�A���ɂ���
         */
        void fromDegree(const Degrees& deg);
        /**
         * @brief float�ւ̃L���X�g
         */
        explicit operator float() const noexcept;
        /**
         * @brief �x���@�ւ̃L���X�g
         */
        explicit operator Degrees() const noexcept;

    private:
        float mRadian; //!< ���W�A���p
    };
    /**
     * @brief ������r���Z�q
     */
    inline bool operator==(const Radians& rad1, const Radians& rad2) {
        return rad1.getRad() == rad2.getRad();
    }
    /**
     * @brief ������r���Z�q
     */
    inline bool operator!=(const Radians& rad1, const Radians& rad2) { return !(rad1 == rad2); }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator+(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) += rad2;
    }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator+(const Radians& rad, float s) { return Radians(rad) += s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator+(float s, const Radians& rad) { return Radians(rad) += s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator-(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) -= rad2;
    }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator-(const Radians& rad, float s) { return Radians(rad) -= s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator-(float s, const Radians& rad) { return Radians(rad) -= s; }
    /**
     * @brief ��Z���Z�q
     */
    inline Radians operator*(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) *= rad2;
    }
    /**
     * @brief ��Z���Z�q
     */
    inline Radians operator*(const Radians& rad, float s) { return Radians(rad) *= s; }
    /**
     * @brief ��Z���Z�q
     */
    inline Radians operator*(float s, const Radians& rad) { return Radians(rad) *= s; }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator/(const Radians& rad1, const Radians& rad2) {
        return Radians(rad1) /= rad2;
    }
    /**
     * @brief ���Z���Z�q
     */
    inline Radians operator/(const Radians& rad, float s) { return Radians(rad) /= s; }
} // namespace Framework::Math
