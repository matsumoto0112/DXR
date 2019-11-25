/**
 * @file CreateProc.h
 * @brief �E�B���h�E�쐬���̃v���V�[�W����`
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class CreateProc
     * @brief �E�B���h�E�쐬���̃v���V�[�W������
     */
    class CreateProc : public IWindowProc {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        CreateProc() {}
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~CreateProc() {}
        /**
         * @brief �E�B���h�E�v���V�[�W��
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
