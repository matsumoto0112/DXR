/**
 * @file DestroyProc.h
 * @brief �A�v���P�[�V�����I�����̃v���V�[�W����`
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class DestroyProc
     * @brief �I�����̃v���V�[�W������
     */
    class DestroyProc : public IWindowProc {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        DestroyProc() {}
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~DestroyProc() {}
        /**
         * @brief �E�B���h�E�v���V�[�W��
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
