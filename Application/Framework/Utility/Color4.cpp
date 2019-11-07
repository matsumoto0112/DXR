#include "Color4.h"
#include "Math/MathUtility.h"

namespace Framework::Utility {
    //�萔
    const Color4 Color4::BLACK = Color4(0.0f, 0.0f, 0.0f, 1.0f);
    const Color4 Color4::WHITE = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    //�R���X�g���N�^
    Color4::Color4()
        :r(1.0f), g(1.0f), b(1.0f), a(1.0f) { }
    //�R���X�g���N�^
    Color4::Color4(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a) { }
    //�R���X�g���N�^
    Color4::Color4(float color[4])
        : r(color[0]), g(color[1]), b(color[2]), a(color[3]) { }
    //�R���X�g���N�^
    Color4::Color4(const Color4& c)
        : Color4(c.r, c.g, c.b, c.a) { }
    //�f�X�g���N�^
    Color4::~Color4() { }
    //������Z�q
    Color4& Color4::operator=(const Color4& c) & {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    //������Z�q
    Color4& Color4::operator=(Color4&& c) & noexcept {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }
    //�P���v���X
    Color4 Color4::operator+() {
        return Color4(r, g, b, a);
    }
    //�P���}�C�i�X
    Color4 Color4::operator-() {
        return Color4(1.0f - r, 1.0f - g, 1.0f - b, a);
    }
    //���Z���
    Color4& Color4::operator+=(const Color4& c) {
        r += c.r; g += c.g; b += c.b; a += c.a;
        return *this;
    }
    //���Z���
    Color4& Color4::operator-=(const Color4& c) {
        r -= c.r; g -= c.g; b -= c.b; a -= c.a;
        return *this;
    }
    //��Z���
    Color4& Color4::operator*=(const Color4& c) {
        r *= c.r; g *= c.g; b *= c.b; a *= c.a;
        return *this;
    }
    //��Z���
    Color4& Color4::operator*=(float s) {
        r *= s; g *= s; b *= s; a *= s;
        return *this;
    }
    //���Z���
    Color4& Color4::operator/=(float s) {
        r /= s; g /= s; b /= s; a /= s;
        return *this;
    }
    //�z��Ŏ擾
    void Color4::get(float c[4]) const {
        c[0] = r;
        c[1] = g;
        c[2] = b;
        c[3] = a;
    }
    //�z��Ŏ擾
    std::array<float, 4> Color4::get() const {
        return std::array<float, 4>{r, g, b, a};
    }
    //0�`1�ɃN�����v
    Color4& Color4::saturate() {
        r = Math::MathUtil::clamp(r, 0.0f, 1.0f);
        g = Math::MathUtil::clamp(g, 0.0f, 1.0f);
        b = Math::MathUtil::clamp(b, 0.0f, 1.0f);
        a = Math::MathUtil::clamp(a, 0.0f, 1.0f);
        return *this;
    }
    //���
    Color4 Color4::lerp(const Color4& a, const Color4& b, float t) {
        t = Math::MathUtil::clamp(t, 0.0f, 1.0f);
        float oneMinusT = 1.0f - t;
        float nr = a.r * oneMinusT + b.r * t;
        float ng = a.g * oneMinusT + b.g * t;
        float nb = a.b * oneMinusT + b.b * t;
        float na = a.a * oneMinusT + b.a * t;
        return Color4(nr, ng, nb, na);
    }

    //�O���[�X�P�[���ϊ�
    Color4 Color4::grayScale(const Color4& c) {
        return Color4(c.r * 0.2125f, c.g * 0.7154f, c.b * 0.0721f, c.a);
    }
    //������r
    bool operator==(const Color4& c1, const Color4& c2) {
        return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
    }
    //������r
    bool operator!=(const Color4& c1, const Color4& c2) {
        return !(c1 == c2);
    }
    //���Z
    Color4 operator+(const Color4& c1, const Color4& c2) {
        return Color4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
    }
    //���Z
    Color4 operator-(const Color4& c1, const Color4& c2) {
        return Color4(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
    }
    //��Z
    Color4 operator*(const Color4& c1, const Color4& c2) {
        return Color4(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
    }
    //��Z
    Color4 operator*(const Color4& c, float s) {
        return Color4(c.r * s, c.g * s, c.b * s, c.a * s);
    }
    //��Z
    Color4 operator*(float s, const Color4& c) {
        return c * s;
    }
    //���Z
    Color4 operator/(const Color4& c, float s) {
        return Color4(c.r / s, c.g / s, c.b / s, c.a / s);
    }
} //Framework::Utility
