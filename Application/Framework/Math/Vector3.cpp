#include "Vector3.h"
#include "Math/MathUtility.h"

namespace Framework::Math {
    //�萔
    const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::LEFT = Vector3(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UP = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::DOWN = Vector3(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::FORWORD = Vector3(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::BACK = Vector3(0.0f, 0.0f, -1.0f);

    //�R���X�g���N�^
    Vector3::Vector3()
        :x(0.0f), y(0.0f), z(0.0f) { }
    //�R���X�g���N�^
    Vector3::Vector3(float x, float y, float z)
        : x(x), y(y), z(z) { }
    //�R���X�g���N�^
    Vector3::Vector3(float s)
        : x(s), y(s), z(s) { }
    //�R���X�g���N�^
    Vector3::Vector3(const Vector3& v)
        : x(v.x), y(v.y), z(v.z) { }
    //���
    Vector3& Vector3::operator=(const Vector3& a) & {
        x = a.x; y = a.y; z = a.z;
        return *this;
    }
    //���
    Vector3& Vector3::operator=(Vector3 && v) & noexcept {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }

    //�P��+
    Vector3 Vector3::operator+() const {
        return Vector3(x, y, z);
    }
    //�P��-
    Vector3 Vector3::operator-() const {
        return Vector3(-x, -y, -z);
    }
    //���Z���
    Vector3& Vector3::operator+=(const Vector3& a) {
        x += a.x; y += a.y; z += a.z;
        return *this;
    }
    //���Z���
    Vector3& Vector3::operator-=(const Vector3& a) {
        x -= a.x; y -= a.y; z -= a.z;
        return *this;
    }
    //��Z���
    Vector3& Vector3::operator*=(float a) {
        x *= a; y *= a; z *= a;
        return *this;
    }
    //���Z���
    Vector3& Vector3::operator/=(float a) {
        float oneOverA = 1.0f / a;
        *this *= oneOverA;
        return *this;
    }
    //�����̓��
    float Vector3::lengthSquared() const {
        return x * x + y * y + z * z;
    }
    //����
    float Vector3::length() const {
        return MathUtil::sqrt(lengthSquared());
    }
    //���K��
    Vector3 Vector3::normalized() const {
        return normalize(*this);
    }
    //���K��
    Vector3 Vector3::normalize(const Vector3 & v) {
        Vector3 res(v);
        const float len = v.length();
        if (len > 0.0f) {
            res.x /= len;
            res.y /= len;
            res.z /= len;
        }
        return res;
    }

    //����
    float Vector3::dot(const Vector3& v1, const Vector3& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    //�O��
    Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }
    // ������r
    bool operator==(const Vector3& v1, const Vector3& v2) {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }
    // ������r
    bool operator!=(const Vector3& v1, const Vector3& v2) {
        return !(v1 == v2);
    }
    //���Z
    Vector3 operator+(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    //���Z
    Vector3 operator-(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }
    //��Z
    Vector3 operator*(const Vector3& v, float s) {
        return Vector3(v.x * s, v.y * s, v.z * s);
    }
    //��Z
    Vector3 operator*(float s, const Vector3& v) {
        return v * s;
    }
    //���Z
    Vector3 operator/(const Vector3& v, float s) {
        return Vector3(v.x / s, v.y / s, v.z / s);
    }
} //Framework::Math
