#include "Vector4.h"
#include "MathUtility.h"

namespace Framework::Math {
    //定数
    const Vector4 Vector4::ZERO = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    //コンストラクタ
    Vector4::Vector4()
        :x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }

    //コンストラクタ
    Vector4::Vector4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) { }
    //コンストラクタ
    Vector4::Vector4(const Vector3& v, float w)
        : x(v.x), y(v.y), z(v.z), w(w) { }
    //コンストラクタ
    Vector4::Vector4(float s)
        : x(s), y(s), z(s), w(s) { }
    //コンストラクタ
    Vector4::Vector4(const Vector4& v)
        : x(v.x), y(v.y), z(v.z), w(v.w) { }
    //代入
    Vector4& Vector4::operator=(const Vector4& a) & {
        x = a.x; y = a.y; z = a.z; w = a.w;
        return *this;
    }
    //代入
    Vector4& Vector4::operator=(Vector4&& v) & noexcept {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this;
    }
    //単項プラス
    Vector4 Vector4::operator+() const {
        return Vector4(x, y, z, w);
    }
    //単項マイナス
    Vector4 Vector4::operator-() const {
        return Vector4(-x, -y, -z, -w);
    }
    //加算代入
    Vector4& Vector4::operator+=(const Vector4& a) {
        x += a.x; y += a.y; z += a.z; w += a.w;
        return *this;
    }
    //減算代入
    Vector4& Vector4::operator-=(const Vector4& a) {
        x -= a.x; y -= a.y; z -= a.z; w -= a.w;
        return *this;
    }
    //乗算代入
    Vector4& Vector4::operator*=(float a) {
        x *= a; y *= a; z *= a; w *= a;
        return *this;
    }
    //除算代入
    Vector4& Vector4::operator/=(float a) {
        float oneOverA = 1.0f / a;
        *this *= oneOverA;
        return *this;
    }
    //長さの二乗
    float Vector4::lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    //長さ
    float Vector4::length() const {
        return MathUtil::sqrt(lengthSquared());
    }
    //正規化
    Vector4 Vector4::normalized() const {
        return Vector4::normalize(*this);
    }
    //正規化
    Vector4 Vector4::normalize(const Vector4& v) {
        Vector4 res(v);
        const float len = v.length();
        //0除算対策
        if (len > 0.0f) {
            res.x /= len;
            res.y /= len;
            res.z /= len;
            res.w /= len;
        }
        return res;
    }
    //内積
    float Vector4::dot(const Vector4& v1, const Vector4& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }
    //等価比較
    bool operator==(const Vector4& v1, const Vector4& v2) {
        return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
    }
    //等価比較
    bool operator!=(const Vector4& v1, const Vector4& v2) {
        return !(v1 == v2);
    }

    //加算
    Vector4 operator+(const Vector4& v1, const Vector4& v2) {
        return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
    }
    //減算
    Vector4 operator-(const Vector4& v1, const Vector4& v2) {
        return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
    }
    //乗算
    Vector4 operator*(const Vector4& v, float s) {
        return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
    }
    //乗算
    Vector4 operator*(float a, const Vector4& v) {
        return v * a;
    }
    //除算
    Vector4 operator/(const Vector4& v, float s) {
        return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);
    }
} //Framework::Math