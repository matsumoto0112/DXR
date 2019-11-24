#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class CreateProc
    * @brief �E�B���h�E�ړ����̏���
    */
    class WindowMoved : public IWindowProc {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        WindowMoved() { }
        /**
        * @brief �f�X�g���N�^
        */
        virtual ~WindowMoved() { }
        /**
        * @brief �E�B���h�E�v���V�[�W��
        */
        virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} //Framework::Window
