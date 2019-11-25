#include "Quaternion.h"
#include "MathUtility.h"

namespace Framework::Math {
    const Quaternion Quaternion::IDENTITY = Quaternion(0, 0, 0, 1);

    //コンストラクタ
    Quaternion::Quaternion() {}
    //コンストラクタ
    Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    //コンストラクタ
    Quaternion::Quaternion(const Vector3& nv, const Radians& angle) {
        const Radians halfTheta = angle * 0.5f;
        const float sin = MathUtil::sin(halfTheta);
        const float cos = MathUtil::cos(halfTheta);
        x = nv.x * sin;
        y = nv.y * sin;
        z = nv.z * sin;
        w = cos;
    }
    //デストラクタ
    Quaternion::~Quaternion() {}

    Quaternion Quaternion::normalized() const { return normalize(*this); }

    Quaternion Quaternion::normalize(const Quaternion& q) {
        Quaternion res(q);
        const float len = MathUtil::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        if (len > 0.0f) {
            res.x /= len;
            res.y /= len;
            res.z /= len;
            res.w /= len;
        }
        return res;
    }

    Quaternion Quaternion::conjugate() const { return Quaternion(-x, -y, -z, w); }

    Quaternion Quaternion::fromEular(
        const Radians& roll, const Radians& pitch, const Radians& yaw) {
        float c1 = MathUtil::cos(roll / 2.0);
        float s1 = MathUtil::sin(roll / 2.0);
        float c2 = MathUtil::cos(pitch / 2.0);
        float s2 = MathUtil::sin(pitch / 2.0);
        float c3 = MathUtil::cos(yaw / 2.0);
        float s3 = MathUtil::sin(yaw / 2.0);

        return Quaternion(s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3,
            c1 * c2 * s3 - s1 * s2 * c3, c1 * c2 * c3 + s1 * s2 * s3);
    }
    Quaternion Quaternion::fromEular(const Vector3& v) {
        return fromEular(Degrees(v.x), Degrees(v.y), Degrees(v.z));
    }

    Vector3 Quaternion::toEular(const Quaternion& q) {
        float x = q.x;
        float y = q.y;
        float z = q.z;
        float w = q.w;
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);
        float m01 = (2.0f * xy) + (2.0f * wz);
        float m10 = (2.0f * xy) - (2.0f * wz);
        float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);
        float m20 = (2.f * xz) + (2.0f * wy);
        float m21 = (2.0f * yz) - (2.0f * wx);
        float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);

        Radians tx, ty, tz;
        if (MathUtil::abs(m21 - 1.0f) < MathUtil::EPSILON) {
            tx = Radians(-MathUtil::PI / 2.0f);
            ty = Radians(0.0f);
            tz = MathUtil::atan2(m10, m00);
        } else if (MathUtil::abs(m21 + 1.0f) < MathUtil::EPSILON) {
            tx = Radians(MathUtil::PI / 2.0f);
            ty = Radians(0);
            tz = MathUtil::atan2(m10, m00);
        } else {
            tx = MathUtil::asin(-m21);
            ty = MathUtil::atan2(m20, m22);
            tz = MathUtil::atan2(m01, m11);
        }

        return Vector3(Degrees(tx).getDeg(), Degrees(ty).getDeg(), Degrees(tz).getDeg());
    }

    Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
        float x = q1.w * q2.x - q1.z * q2.y + q1.y * q2.z + q1.x * q2.w;
        float y = q1.z * q2.x + q1.w * q2.y - q1.x * q2.z + q1.y * q2.w;
        float z = -q1.y * q2.x + q1.x * q2.y + q1.w * q2.z + q1.z * q2.w;
        float w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
        return Quaternion(x, y, z, w);
    }

    Vector3 operator*(const Quaternion& q, const Vector3& v) {
        Quaternion r(v.x, v.y, v.z, 0.0f);
        Quaternion res = q * r * q.conjugate();
        return Vector3(res.x, res.y, res.z);
    }

} // namespace Framework::Math
