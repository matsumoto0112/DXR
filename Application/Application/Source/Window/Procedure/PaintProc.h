/**
 * @file PaintProc.h
 * @brief �`��C�x���g���̃v���V�[�W����`
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class PaintProc
     * @brief �`��C�x���g����
     */
    class PaintProc : public IWindowProc {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        PaintProc() {}
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~PaintProc() {}
        /**
         * @brief �E�B���h�E�v���V�[�W��
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
