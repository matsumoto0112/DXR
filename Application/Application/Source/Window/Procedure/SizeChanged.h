#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class SizeChanged
    * @brief �E�B���h�E�̃T�C�Y���ύX���ꂽ���̏���
    */
    class SizeChanged : public IWindowProc {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        SizeChanged() { }
        /**
        * @brief �f�X�g���N�^
        */
        virtual ~SizeChanged() { }
        /**
        * @brief �E�B���h�E�v���V�[�W��
        */
        virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} //Framework::Window
