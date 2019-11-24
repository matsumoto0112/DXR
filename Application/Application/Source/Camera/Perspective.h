/**
 * @file Perspective.h
 * @brief パースペクティブカメラ
 */

#pragma once
#include <d3d12.h>
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

namespace Framework::Camera {
    /**
     * @class Perspective
     * @brief パースペクティブカメラ
     */
    class Perspective {
    public:
        /**
         * @brief コンストラクタ
         */
        Perspective();
        /**
         * @brief コンストラクタ
         * @param position 視点
         * @param lookat 注視点
         * @param up 上方向
         * @param fov 視野角
         * @param aspect アスペクト比
         * @param nearZ 近クリップ点
         * @param farZ 遠クリップ点
         */
        Perspective(const Math::Vector3& position, const Math::Vector3& lookat,
            const Math::Vector3& up, const Math::Radians& fov, float aspect, float nearZ,
            float farZ);
        /**
         * @brief デストラクタ
         */
        ~Perspective();
        /**
         * @brief 座標を設定する
         */
        void setPosition(const Math::Vector3& position) {
            mPosition = position;
            updateView();
        }
        /**
         * @brief 座標を取得する
         */
        const Math::Vector3& getPosition() const { return mPosition; }
        /**
         * @brief 注視点を設定する
         */
        void setLookAt(const Math::Vector3& lookat) {
            mLookAt = lookat;
            updateView();
        }
        /**
         * @brief 上方向ベクトルを取得するする
         */
        const Math::Vector3& getLookAt() const { return mLookAt; }
        /**
         * @brief 上方向ベクトルを設定する
         */
        void setUpVector(const Math::Vector3& up) {
            mUpVector = up;
            updateView();
        }
        /**
         * @brief 上方向ベクトルを取得する
         */
        const Math::Vector3& getUpVector() const { return mUpVector; }
        /**
         * @brief 視野角を設定する
         */
        void setFov(const Math::Radians& fov) {
            mFov = fov;
            updateProj();
        }
        /**
         * @brief 視野角を取得する
         */
        const Math::Radians& getFov() const { return mFov; }
        /**
         * @brief アスペクト比を設定する
         */
        void setAspectRatio(float aspect) {
            mAspect = aspect;
            updateProj();
        }
        /**
         * @brief アスペクト比を取得する
         */
        float getAspectRatio() const { return mAspect; }
        /**
         * @brief 最近点までの距離を設定する
         */
        void setNearClip(float nearZ) {
            mNearClip = nearZ;
            updateProj();
        }
        /**
         * @brief 最近点までの距離を取得する
         */
        float getNearClip() const { return mNearClip; }
        /**
         * @brief 最遠点までの距離を設定する
         */
        void setFarClip(float farZ) {
            mFarClip = farZ;
            updateProj();
        }
        /**
         * @brief 最遠点までの距離を設定する
         */
        float getFarClip() const { return mFarClip; }
        /**
         * @brief ビュー行列を取得する
         */
        const Math::Matrix4x4& getView() const { return mViewMatrix; }
        /**
         * @brief プロジェクション行列を取得する
         */
        const Math::Matrix4x4& getProjection() const { return mProjMatrix; }

    private:
        /**
         * @brief ビュー行列を更新する
         */
        void updateView();
        /**
         * @brief プロジェクション行列を更新する
         */
        void updateProj();

    private:
        Math::Vector3 mPosition; //!< 視点
        Math::Vector3 mLookAt; //!< 注視点
        Math::Vector3 mUpVector; //!< 上方向
        Math::Radians mFov; //!< 視野角
        float mAspect; //!< アスペクト比
        float mNearClip; //!< 最近点
        float mFarClip; //!< 最遠点
        Math::Matrix4x4 mViewMatrix; //!< ビュー行列
        Math::Matrix4x4 mProjMatrix; //!< プロジェクション行列
    };
} // namespace Framework::Camera
