#include "Angle.h"
#include "MathUtility.h"

namespace {
    static constexpr float RAD_2_DEG = 180.0f / Framework::Math::PI;
    static constexpr float DEG_2_RAD = 1.0f / RAD_2_DEG;
}

namespace Framework::Math {

    Degrees::Degrees()
        :mDegree(0.0f) { }

    Degrees::Degrees(float degree)
        : mDegree(degree) { }

    Degrees::Degrees(const Degrees& deg)
        : mDegree(deg.mDegree) { }

    Degrees::Degrees(const Radians& rad)
        : mDegree(rad.getRad() * RAD_2_DEG) { }

    Degrees::~Degrees() { }

    Degrees& Degrees::operator=(const Degrees& deg)& {
        mDegree = deg.mDegree;
        return *this;
    }

    Degrees& Degrees::operator=(const Radians& rad)& {
        mDegree = rad.getRad() * RAD_2_DEG;
        return *this;
    }

    Degrees& Degrees::operator=(Degrees&& deg)& noexcept {
        mDegree = deg.mDegree;
        return *this;
    }

    Degrees Degrees::operator+() const {
        return Degrees(mDegree);
    }

    Degrees Degrees::operator-() const {
        return Degrees(-mDegree);
    }

    Degrees& Degrees::operator+=(const Degrees& deg) {
        mDegree += deg.mDegree;
        return *this;
    }

    Degrees& Degrees::operator+=(float s) {
        mDegree += s;
        return *this;
    }

    Degrees& Degrees::operator-=(const Degrees& deg) {
        mDegree -= deg.mDegree;
        return *this;
    }

    Degrees& Degrees::operator-=(float s) {
        mDegree -= s;
        return *this;
    }

    Degrees& Degrees::operator*=(const Degrees& deg) {
        mDegree *= deg.mDegree;
        return *this;
    }

    Degrees& Degrees::operator*=(float s) {
        mDegree *= s;
        return *this;
    }

    Degrees& Degrees::operator/=(const Degrees& deg) {
        mDegree /= deg.mDegree;
        return *this;
    }

    Degrees& Degrees::operator/=(float s) {
        mDegree /= s;
        return *this;
    }

    Radians Degrees::toRadians() const {
        return Radians(mDegree * DEG_2_RAD);
    }

    void Degrees::fromRadians(const Radians& rad) {
        mDegree = rad.getRad() * RAD_2_DEG;
    }

    Degrees Degrees::normalize() const {
        Degrees res(*this);
        if (res.mDegree < 0.0f)res.mDegree += 180.0f;
        return res;
    }

    Degrees::operator float() const noexcept { return mDegree; }

    Degrees::operator Radians() const noexcept { return Radians(mDegree * DEG_2_RAD); }

    Radians::Radians()
        :mRadian(0.0f) { }

    Radians::Radians(float rad)
        : mRadian(rad) { }

    Radians::Radians(const Radians& rad)
        : mRadian(rad.mRadian) { }

    Radians::Radians(const Degrees& deg)
        : mRadian(deg.getDeg() * DEG_2_RAD) { }

    Radians::~Radians() { }

    Radians& Radians::operator=(const Radians& rad)& {
        mRadian = rad.mRadian;
        return *this;
    }

    Radians& Radians::operator=(const Degrees& deg)& {
        mRadian = deg.getDeg() * DEG_2_RAD;
        return *this;
    }

    Radians& Radians::operator=(Radians&& rad)& noexcept {
        mRadian = rad.mRadian;
        return *this;
    }

    Radians Radians::operator+() const {
        return Radians(mRadian);
    }

    Radians Radians::operator-() const {
        return Radians(-mRadian);
    }

    Radians& Radians::operator+=(const Radians& rad) {
        mRadian += rad.mRadian;
        return *this;
    }

    Radians & Radians::operator+=(float s) {
        mRadian += s;
        return *this;
    }

    Radians& Radians::operator-=(const Radians& rad) {
        mRadian -= rad.mRadian;
        return *this;
    }

    Radians & Radians::operator-=(float s) {
        mRadian -= s;
        return *this;
    }

    Radians& Radians::operator*=(const Radians& rad) {
        mRadian *= rad.mRadian;
        return *this;
    }

    Radians & Radians::operator*=(float s) {
        mRadian *= s;
        return *this;
    }

    Radians& Radians::operator/=(const Radians& rad) {
        mRadian /= rad.mRadian;
        return *this;
    }

    Radians & Radians::operator/=(float s) {
        mRadian /= s;
        return *this;
    }

    Degrees Radians::toDegree() const {
        return Degrees(mRadian * RAD_2_DEG);
    }

    void Radians::fromDegree(const Degrees& deg) {
        mRadian = deg.getDeg() * DEG_2_RAD;
    }

    Radians::operator float() const noexcept {
        return mRadian;
    }

    Radians::operator Degrees() const noexcept {
        return Degrees(mRadian * RAD_2_DEG);
    }

    bool operator==(const Degrees& deg1, const Degrees& deg2) {
        return deg1.getDeg() == deg2.getDeg();
    }

    bool operator!=(const Degrees& deg1, const Degrees& deg2) {
        return !(deg1 == deg2);
    }

} //Framework::Math
