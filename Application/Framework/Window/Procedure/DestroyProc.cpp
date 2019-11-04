#include "DestroyProc.h"

namespace Framework::Window {
    //�v���V�[�W��
    LRESULT DestroyProc::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        if (msg == WM_DESTROY) {
            *isReturn = true;
            //�I���w�߂𔭍s����
            PostQuitMessage(0);
        }
        return 0L;
    }
} //Framework::Window
