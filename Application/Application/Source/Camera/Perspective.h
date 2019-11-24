/**
 * @file Perspective.h
 * @brief �p�[�X�y�N�e�B�u�J����
 */

#pragma once
#include <d3d12.h>
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

namespace Framework::Camera {
    /**
     * @class Perspective
     * @brief �p�[�X�y�N�e�B�u�J����
     */
    class Perspective {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        Perspective();
        /**
         * @brief �R���X�g���N�^
         * @param position ���_
         * @param lookat �����_
         * @param up �����
         * @param fov ����p
         * @param aspect �A�X�y�N�g��
         * @param nearZ �߃N���b�v�_
         * @param farZ ���N���b�v�_
         */
        Perspective(const Math::Vector3& position, const Math::Vector3& lookat,
            const Math::Vector3& up, const Math::Radians& fov, float aspect, float nearZ,
            float farZ);
        /**
         * @brief �f�X�g���N�^
         */
        ~Perspective();
        /**
         * @brief ���W��ݒ肷��
         */
        void setPosition(const Math::Vector3& position) {
            mPosition = position;
            updateView();
        }
        /**
         * @brief ���W���擾����
         */
        const Math::Vector3& getPosition() const { return mPosition; }
        /**
         * @brief �����_��ݒ肷��
         */
        void setLookAt(const Math::Vector3& lookat) {
            mLookAt = lookat;
            updateView();
        }
        /**
         * @brief ������x�N�g�����擾���邷��
         */
        const Math::Vector3& getLookAt() const { return mLookAt; }
        /**
         * @brief ������x�N�g����ݒ肷��
         */
        void setUpVector(const Math::Vector3& up) {
            mUpVector = up;
            updateView();
        }
        /**
         * @brief ������x�N�g�����擾����
         */
        const Math::Vector3& getUpVector() const { return mUpVector; }
        /**
         * @brief ����p��ݒ肷��
         */
        void setFov(const Math::Radians& fov) {
            mFov = fov;
            updateProj();
        }
        /**
         * @brief ����p���擾����
         */
        const Math::Radians& getFov() const { return mFov; }
        /**
         * @brief �A�X�y�N�g���ݒ肷��
         */
        void setAspectRatio(float aspect) {
            mAspect = aspect;
            updateProj();
        }
        /**
         * @brief �A�X�y�N�g����擾����
         */
        float getAspectRatio() const { return mAspect; }
        /**
         * @brief �ŋߓ_�܂ł̋�����ݒ肷��
         */
        void setNearClip(float nearZ) {
            mNearClip = nearZ;
            updateProj();
        }
        /**
         * @brief �ŋߓ_�܂ł̋������擾����
         */
        float getNearClip() const { return mNearClip; }
        /**
         * @brief �ŉ��_�܂ł̋�����ݒ肷��
         */
        void setFarClip(float farZ) {
            mFarClip = farZ;
            updateProj();
        }
        /**
         * @brief �ŉ��_�܂ł̋�����ݒ肷��
         */
        float getFarClip() const { return mFarClip; }
        /**
         * @brief �r���[�s����擾����
         */
        const Math::Matrix4x4& getView() const { return mViewMatrix; }
        /**
         * @brief �v���W�F�N�V�����s����擾����
         */
        const Math::Matrix4x4& getProjection() const { return mProjMatrix; }

    private:
        /**
         * @brief �r���[�s����X�V����
         */
        void updateView();
        /**
         * @brief �v���W�F�N�V�����s����X�V����
         */
        void updateProj();

    private:
        Math::Vector3 mPosition; //!< ���_
        Math::Vector3 mLookAt; //!< �����_
        Math::Vector3 mUpVector; //!< �����
        Math::Radians mFov; //!< ����p
        float mAspect; //!< �A�X�y�N�g��
        float mNearClip; //!< �ŋߓ_
        float mFarClip; //!< �ŉ��_
        Math::Matrix4x4 mViewMatrix; //!< �r���[�s��
        Math::Matrix4x4 mProjMatrix; //!< �v���W�F�N�V�����s��
    };
} // namespace Framework::Camera
