#include "Perspective.h"

namespace Framework::Camera {
    //コンストラクタ
    Perspective::Perspective()
        : mPosition(Math::Vector3::ZERO),
          mLookAt(Math::Vector3::ZERO),
          mUpVector(Math::Vector3::UP),
          mFov(30.0f),
          mAspect(1.0f),
          mNearClip(0.1f),
          mFarClip(1000.0f) {}
    //コンストラクタ
    Perspective::Perspective(const Math::Vector3& position,
        const Math::Vector3& lookat, const Math::Vector3& up,
        const Math::Radians& fov, float aspect, float nearZ, float farZ)
        : mPosition(mPosition),
          mLookAt(lookat),
          mUpVector(up),
          mFov(fov),
          mAspect(aspect),
          mNearClip(nearZ),
          mFarClip(farZ),
          mViewMatrix(Math::Matrix4x4::createView(position, lookat, up)),
          mProjMatrix(Math::Matrix4x4::createProjection(fov, aspect, nearZ, farZ)) {}
    //デストラクタ
    Perspective::~Perspective() {}
    //ビュー行列の更新
    void Perspective::updateView() {
        mViewMatrix = Math::Matrix4x4::createView(mPosition, mLookAt, mUpVector);
    }
    //プロジェクション行列の更新
    void Perspective::updateProj() {
        mProjMatrix = Math::Matrix4x4::createProjection(
            mFov, mAspect, mNearClip, mFarClip);
    }
} // namespace Framework::Camera
