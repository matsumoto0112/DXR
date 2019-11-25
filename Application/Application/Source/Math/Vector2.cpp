#include "Vector2.h"
#include "MathUtility.h"

namespace Framework::Math {
    //定数
    const Vector2 Vector2::ZERO = Vector2(0, 0);
    const Vector2 Vector2::LEFT = Vector2(-1, 0);
    const Vector2 Vector2::RIGHT = Vector2(1, 0);
    const Vector2 Vector2::UP = Vector2(0, 1);
    const Vector2 Vector2::DOWN = Vector2(0, -1);

    //コンストラクタ
    Vector2::Vector2() : x(0.0f), y(0.0f) {}
    //コンストラクタ
    Vector2::Vector2(float x, float y) : x(x), y(y) {}
    //コンストラクタ
    Vector2::Vector2(float v) : x(v), y(v) {}
    //コンストラクタ
    Vector2::Vector2(const Vector2& v) : Vector2(v.x, v.y) {}
    //代入演算子
    Vector2& Vector2::operator=(const Vector2& v) & {
        x = v.x;
        y = v.y;
        return *this;
    }
    //代入演算子
    Vector2& Vector2::operator=(Vector2&& v) & noexcept {
        x = v.x;
        y = v.y;
        return *this;
    }
    //単項プラス
    Vector2 Vector2::operator+() const { return *this; }
    //単項マイナス
    Vector2 Vector2::operator-() const { return Vector2(-x, -y); }
    //加算代入
    Vector2& Vector2::operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    //減算代入
    Vector2& Vector2::operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    //乗算代入
    Vector2& Vector2::operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }
    //除算代入
    Vector2& Vector2::operator/=(float s) {
        const float oneOverS = 1.0f / s;
        x *= oneOverS;
        y *= oneOverS;
        return *this;
    }
    //長さの二乗
    float Vector2::lengthSquared() const { return x * x + y * y; }
    //長さ
    float Vector2::length() const {
        const float lengthSquare = lengthSquared();
        //0のルートの場合を省く
        if (lengthSquare > 0.0f) { return MathUtil::sqrt(lengthSquare); }
        return 0.0f;
    }
    //正規化
    Vector2 Vector2::normalized() const { return Vector2::normalize(*this); }
    //正規化
    Vector2 Vector2::normalize(const Vector2& v) {
        Vector2 res(v);
        const float len = v.length();
        //0除算対策
        if (len) {
            res.x /= len;
            res.y /= len;
        }
        return res;
    }
    //内積
    float Vector2::dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
    //外積
    float Vector2::cross(const Vector2& v1, const Vector2& v2) { return v1.x * v2.y - v1.y * v2.x; }

    //等価比較
    bool operator==(const Vector2& v1, const Vector2& v2) { return (v1.x == v2.x && v1.y == v2.y); }
    //等価比較
    bool operator!=(const Vector2& v1, const Vector2& v2) { return !(v1 == v2); }
    //加算
    Vector2 operator+(const Vector2& v1, const Vector2& v2) {
        return Vector2(v1.x + v2.x, v1.y + v2.y);
    }
    //減算
    Vector2 operator-(const Vector2& v1, const Vector2& v2) {
        return Vector2(v1.x - v2.x, v1.y - v2.y);
    }
    //乗算
    Vector2 operator*(const Vector2& v, float s) { return Vector2(v.x * s, v.y * s); }
    //乗算
    Vector2 operator*(float s, const Vector2& v) { return v * s; }
    //除算
    Vector2 operator/(const Vector2& v, float s) { return Vector2(v.x / s, v.y / s); }
} // namespace Framework::Math
