#include "Color4.h"
#include "Math/MathUtility.h"

namespace Framework::Utility {
    //定数
    const Color4 Color4::BLACK = Color4(0.0f, 0.0f, 0.0f, 1.0f);
    const Color4 Color4::WHITE = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    //コンストラクタ
    Color4::Color4()
        :r(1.0f), g(1.0f), b(1.0f), a(1.0f) { }
    //コンストラクタ
    Color4::Color4(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a) { }
    //コンストラクタ
    Color4::Color4(float color[4])
        : r(color[0]), g(color[1]), b(color[2]), a(color[3]) { }
    //コンストラクタ
    Color4::Color4(const Color4& c)
        : Color4(c.r, c.g, c.b, c.a) { }
    //デストラクタ
    Color4::~Color4() { }
    //代入演算子
    Color4& Color4::operator=(const Color4& c) & {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    //代入演算子
    Color4& Color4::operator=(Color4&& c) & noexcept {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }
    //単項プラス
    Color4 Color4::operator+() {
        return Color4(r, g, b, a);
    }
    //単項マイナス
    Color4 Color4::operator-() {
        return Color4(1.0f - r, 1.0f - g, 1.0f - b, a);
    }
    //加算代入
    Color4& Color4::operator+=(const Color4& c) {
        r += c.r; g += c.g; b += c.b; a += c.a;
        return *this;
    }
    //減算代入
    Color4& Color4::operator-=(const Color4& c) {
        r -= c.r; g -= c.g; b -= c.b; a -= c.a;
        return *this;
    }
    //乗算代入
    Color4& Color4::operator*=(const Color4& c) {
        r *= c.r; g *= c.g; b *= c.b; a *= c.a;
        return *this;
    }
    //乗算代入
    Color4& Color4::operator*=(float s) {
        r *= s; g *= s; b *= s; a *= s;
        return *this;
    }
    //除算代入
    Color4& Color4::operator/=(float s) {
        r /= s; g /= s; b /= s; a /= s;
        return *this;
    }
    //配列で取得
    void Color4::get(float c[4]) const {
        c[0] = r;
        c[1] = g;
        c[2] = b;
        c[3] = a;
    }
    //配列で取得
    std::array<float, 4> Color4::get() const {
        return std::array<float, 4>{r, g, b, a};
    }
    //0～1にクランプ
    Color4& Color4::saturate() {
        r = Math::MathUtil::clamp(r, 0.0f, 1.0f);
        g = Math::MathUtil::clamp(g, 0.0f, 1.0f);
        b = Math::MathUtil::clamp(b, 0.0f, 1.0f);
        a = Math::MathUtil::clamp(a, 0.0f, 1.0f);
        return *this;
    }
    //補間
    Color4 Color4::lerp(const Color4& a, const Color4& b, float t) {
        t = Math::MathUtil::clamp(t, 0.0f, 1.0f);
        float oneMinusT = 1.0f - t;
        float nr = a.r * oneMinusT + b.r * t;
        float ng = a.g * oneMinusT + b.g * t;
        float nb = a.b * oneMinusT + b.b * t;
        float na = a.a * oneMinusT + b.a * t;
        return Color4(nr, ng, nb, na);
    }

    //グレースケール変換
    Color4 Color4::grayScale(const Color4& c) {
        return Color4(c.r * 0.2125f, c.g * 0.7154f, c.b * 0.0721f, c.a);
    }
    //等価比較
    bool operator==(const Color4& c1, const Color4& c2) {
        return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
    }
    //等価比較
    bool operator!=(const Color4& c1, const Color4& c2) {
        return !(c1 == c2);
    }
    //加算
    Color4 operator+(const Color4& c1, const Color4& c2) {
        return Color4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
    }
    //減算
    Color4 operator-(const Color4& c1, const Color4& c2) {
        return Color4(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
    }
    //乗算
    Color4 operator*(const Color4& c1, const Color4& c2) {
        return Color4(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
    }
    //乗算
    Color4 operator*(const Color4& c, float s) {
        return Color4(c.r * s, c.g * s, c.b * s, c.a * s);
    }
    //乗算
    Color4 operator*(float s, const Color4& c) {
        return c * s;
    }
    //除算
    Color4 operator/(const Color4& c, float s) {
        return Color4(c.r / s, c.g / s, c.b / s, c.a / s);
    }
} //Framework::Utility
