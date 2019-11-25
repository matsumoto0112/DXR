/**
 * @file IWindowProc.h
 * @brief �E�B���h�E�v���V�[�W���C���^�[�t�F�[�X��`
 */

#pragma once

namespace Framework::Window {
    /**
     * @class IWindowProc
     * @brief �E�B���h�E�v���V�[�W���C���^�[�t�F�[�X
     */
    class IWindowProc {
    public:
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~IWindowProc() = default;
        /**
         * @brief �E�B���h�E�v���V�[�W��
         * @param hWnd �E�B���h�E�n���h��
         * @param msg ���b�Z�[�W�̎��
         * @param wParam �p�����[�^
         * @param lParam �p�����[�^
         * @param[out] isReturn ���b�Z�[�W������������
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn)
            = 0;
    };
} // namespace Framework::Window
