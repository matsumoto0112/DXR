#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class SysKeyDown
    * @brief �V�X�e���L�[�������ꂽ���̏���
    */
    class SysKeyDown : public IWindowProc {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        SysKeyDown() { }
        /**
        * @brief �f�X�g���N�^
        */
        virtual ~SysKeyDown() { }
        /**
        * @brief �E�B���h�E�v���V�[�W��
        */
        virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} //Framework::Window
