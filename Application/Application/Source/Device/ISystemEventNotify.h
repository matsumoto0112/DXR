/**
 * @file ISystemEventNotify.h
 * @brief �V�X�e���C�x���g�����ʒm
 */

#pragma once
#include <Windows.h>

namespace Framework::Device {
    /**
     * @brief �V�X�e���C�x���g�������l
     */
    class ISystemEventNotify {
    public:
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~ISystemEventNotify() = default;
        /**
         * @brief �t���[���C�x���g
         */
        virtual void onFrameEvent() = 0;
        /**
         * @brief �t���X�N���[�����[�h�ؑ�
         */
        virtual void toggleFullScreenWindow() = 0;
        /**
         * @brief �E�B���h�E�̋��E�ݒ�
         */
        virtual void setWindowBounds(const RECT& rect) = 0;
        /**
         * @brief �E�B���h�E�T�C�Y�̕ύX
         */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) = 0;
        /**
         * @brief �E�B���h�E�̈ړ��C�x���g
         */
        virtual void onWindowMoved(int x, int y) = 0;
    };
} // namespace Framework::Device
