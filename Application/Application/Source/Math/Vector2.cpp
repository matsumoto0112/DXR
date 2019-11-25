#include "Vector2.h"
#include "MathUtility.h"

namespace Framework::Math {
    //�萔
    const Vector2 Vector2::ZERO = Vector2(0, 0);
    const Vector2 Vector2::LEFT = Vector2(-1, 0);
    const Vector2 Vector2::RIGHT = Vector2(1, 0);
    const Vector2 Vector2::UP = Vector2(0, 1);
    const Vector2 Vector2::DOWN = Vector2(0, -1);

    //�R���X�g���N�^
    Vector2::Vector2() : x(0.0f), y(0.0f) {}
    //�R���X�g���N�^
    Vector2::Vector2(float x, float y) : x(x), y(y) {}
    //�R���X�g���N�^
    Vector2::Vector2(float v) : x(v), y(v) {}
    //�R���X�g���N�^
    Vector2::Vector2(const Vector2& v) : Vector2(v.x, v.y) {}
    //������Z�q
    Vector2& Vector2::operator=(const Vector2& v) & {
        x = v.x;
        y = v.y;
        return *this;
    }
    //������Z�q
    Vector2& Vector2::operator=(Vector2&& v) & noexcept {
        x = v.x;
        y = v.y;
        return *this;
    }
    //�P���v���X
    Vector2 Vector2::operator+() const { return *this; }
    //�P���}�C�i�X
    Vector2 Vector2::operator-() const { return Vector2(-x, -y); }
    //���Z���
    Vector2& Vector2::operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    //���Z���
    Vector2& Vector2::operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    //��Z���
    Vector2& Vector2::operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }
    //���Z���
    Vector2& Vector2::operator/=(float s) {
        const float oneOverS = 1.0f / s;
        x *= oneOverS;
        y *= oneOverS;
        return *this;
    }
    //�����̓��
    float Vector2::lengthSquared() const { return x * x + y * y; }
    //����
    float Vector2::length() const {
        const float lengthSquare = lengthSquared();
        //0�̃��[�g�̏ꍇ���Ȃ�
        if (lengthSquare > 0.0f) { return MathUtil::sqrt(lengthSquare); }
        return 0.0f;
    }
    //���K��
    Vector2 Vector2::normalized() const { return Vector2::normalize(*this); }
    //���K��
    Vector2 Vector2::normalize(const Vector2& v) {
        Vector2 res(v);
        const float len = v.length();
        //0���Z�΍�
        if (len) {
            res.x /= len;
            res.y /= len;
        }
        return res;
    }
    //����
    float Vector2::dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
    //�O��
    float Vector2::cross(const Vector2& v1, const Vector2& v2) { return v1.x * v2.y - v1.y * v2.x; }

    //������r
    bool operator==(const Vector2& v1, const Vector2& v2) { return (v1.x == v2.x && v1.y == v2.y); }
    //������r
    bool operator!=(const Vector2& v1, const Vector2& v2) { return !(v1 == v2); }
    //���Z
    Vector2 operator+(const Vector2& v1, const Vector2& v2) {
        return Vector2(v1.x + v2.x, v1.y + v2.y);
    }
    //���Z
    Vector2 operator-(const Vector2& v1, const Vector2& v2) {
        return Vector2(v1.x - v2.x, v1.y - v2.y);
    }
    //��Z
    Vector2 operator*(const Vector2& v, float s) { return Vector2(v.x * s, v.y * s); }
    //��Z
    Vector2 operator*(float s, const Vector2& v) { return v * s; }
    //���Z
    Vector2 operator/(const Vector2& v, float s) { return Vector2(v.x / s, v.y / s); }
} // namespace Framework::Math
