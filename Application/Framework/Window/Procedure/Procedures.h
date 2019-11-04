#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class Procedures
    * @brief �v���V�[�W������
    */
    class Procedures {
    public:
        /**
        * @brief ���C���E�B���h�E�̃v���V�[�W������
        * @param hWnd �E�B���h�E�n���h��
        * @param msg ���b�Z�[�W
        * @param wParam �p�����[�^
        * @param lParam �p�����[�^
        */
        static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        /**
        * @brief �v���V�[�W���̒ǉ�
        */
        static void addProc(IWindowProc* proc);
    private:
        static std::vector<std::unique_ptr<IWindowProc>> mWindowProcs; //!< �v���V�[�W���������X�g
    };
} //Framework::Window
