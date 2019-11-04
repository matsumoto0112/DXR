#include "Vector3.h"
#include "Math/MathUtility.h"

namespace Framework::Math {
    //定数
    const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::LEFT = Vector3(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UP = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::DOWN = Vector3(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::FORWORD = Vector3(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::BACK = Vector3(0.0f, 0.0f, -1.0f);

    //コンストラクタ
    Vector3::Vector3()
        :x(0.0f), y(0.0f), z(0.0f) { }
    //コンストラクタ
    Vector3::Vector3(float x, float y, float z)
        : x(x), y(y), z(z) { }
    //コンストラクタ
    Vector3::Vector3(float s)
        : x(s), y(s), z(s) { }
    //コンストラクタ
    Vector3::Vector3(const Vector3& v)
        : x(v.x), y(v.y), z(v.z) { }
    //代入
    Vector3& Vector3::operator=(const Vector3& a) & {
        x = a.x; y = a.y; z = a.z;
        return *this;
    }
    //代入
    Vector3& Vector3::operator=(Vector3 && v) & noexcept {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }

    //単項+
    Vector3 Vector3::operator+() const {
        return Vector3(x, y, z);
    }
    //単項-
    Vector3 Vector3::operator-() const {
        return Vector3(-x, -y, -z);
    }
    //加算代入
    Vector3& Vector3::operator+=(const Vector3& a) {
        x += a.x; y += a.y; z += a.z;
        return *this;
    }
    //減算代入
    Vector3& Vector3::operator-=(const Vector3& a) {
        x -= a.x; y -= a.y; z -= a.z;
        return *this;
    }
    //乗算代入
    Vector3& Vector3::operator*=(float a) {
        x *= a; y *= a; z *= a;
        return *this;
    }
    //除算代入
    Vector3& Vector3::operator/=(float a) {
        float oneOverA = 1.0f / a;
        *this *= oneOverA;
        return *this;
    }
    //長さの二乗
    float Vector3::lengthSquared() const {
        return x * x + y * y + z * z;
    }
    //長さ
    float Vector3::length() const {
        return MathUtil::sqrt(lengthSquared());
    }
    //正規化
    Vector3 Vector3::normalized() const {
        return normalize(*this);
    }
    //正規化
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

    //内積
    float Vector3::dot(const Vector3& v1, const Vector3& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    //外積
    Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }
    // 等価比較
    bool operator==(const Vector3& v1, const Vector3& v2) {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }
    // 等価比較
    bool operator!=(const Vector3& v1, const Vector3& v2) {
        return !(v1 == v2);
    }
    //加算
    Vector3 operator+(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    //減算
    Vector3 operator-(const Vector3& v1, const Vector3& v2) {
        return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }
    //乗算
    Vector3 operator*(const Vector3& v, float s) {
        return Vector3(v.x * s, v.y * s, v.z * s);
    }
    //乗算
    Vector3 operator*(float s, const Vector3& v) {
        return v * s;
    }
    //除算
    Vector3 operator/(const Vector3& v, float s) {
        return Vector3(v.x / s, v.y / s, v.z / s);
    }
} //Framework::Math
