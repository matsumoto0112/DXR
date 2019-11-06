#include "Vector4.h"
#include "MathUtility.h"

namespace Framework::Math {
    //�萔
    const Vector4 Vector4::ZERO = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    //�R���X�g���N�^
    Vector4::Vector4()
        :x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }

    //�R���X�g���N�^
    Vector4::Vector4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) { }
    //�R���X�g���N�^
    Vector4::Vector4(const Vector3& v, float w)
        : x(v.x), y(v.y), z(v.z), w(w) { }
    //�R���X�g���N�^
    Vector4::Vector4(float s)
        : x(s), y(s), z(s), w(s) { }
    //�R���X�g���N�^
    Vector4::Vector4(const Vector4& v)
        : x(v.x), y(v.y), z(v.z), w(v.w) { }
    //���
    Vector4& Vector4::operator=(const Vector4& a) & {
        x = a.x; y = a.y; z = a.z; w = a.w;
        return *this;
    }
    //���
    Vector4& Vector4::operator=(Vector4&& v) & noexcept {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this;
    }
    //�P���v���X
    Vector4 Vector4::operator+() const {
        return Vector4(x, y, z, w);
    }
    //�P���}�C�i�X
    Vector4 Vector4::operator-() const {
        return Vector4(-x, -y, -z, -w);
    }
    //���Z���
    Vector4& Vector4::operator+=(const Vector4& a) {
        x += a.x; y += a.y; z += a.z; w += a.w;
        return *this;
    }
    //���Z���
    Vector4& Vector4::operator-=(const Vector4& a) {
        x -= a.x; y -= a.y; z -= a.z; w -= a.w;
        return *this;
    }
    //��Z���
    Vector4& Vector4::operator*=(float a) {
        x *= a; y *= a; z *= a; w *= a;
        return *this;
    }
    //���Z���
    Vector4& Vector4::operator/=(float a) {
        float oneOverA = 1.0f / a;
        *this *= oneOverA;
        return *this;
    }
    //�����̓��
    float Vector4::lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    //����
    float Vector4::length() const {
        return MathUtil::sqrt(lengthSquared());
    }
    //���K��
    Vector4 Vector4::normalized() const {
        return Vector4::normalize(*this);
    }
    //���K��
    Vector4 Vector4::normalize(const Vector4& v) {
        Vector4 res(v);
        const float len = v.length();
        //0���Z�΍�
        if (len > 0.0f) {
            res.x /= len;
            res.y /= len;
            res.z /= len;
            res.w /= len;
        }
        return res;
    }
    //����
    float Vector4::dot(const Vector4& v1, const Vector4& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }
    //������r
    bool operator==(const Vector4& v1, const Vector4& v2) {
        return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
    }
    //������r
    bool operator!=(const Vector4& v1, const Vector4& v2) {
        return !(v1 == v2);
    }

    //���Z
    Vector4 operator+(const Vector4& v1, const Vector4& v2) {
        return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
    }
    //���Z
    Vector4 operator-(const Vector4& v1, const Vector4& v2) {
        return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
    }
    //��Z
    Vector4 operator*(const Vector4& v, float s) {
        return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
    }
    //��Z
    Vector4 operator*(float a, const Vector4& v) {
        return v * a;
    }
    //���Z
    Vector4 operator/(const Vector4& v, float s) {
        return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);
    }
} //Framework::Math