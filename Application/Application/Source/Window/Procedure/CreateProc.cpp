#include "CreateProc.h"

namespace Framework::Window {
    //�v���V�[�W��
    LRESULT CreateProc::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        //WM_CREATE�̂ݏ�������
        if (msg != WM_CREATE) return 0L;
        *isReturn = true;
        //�E�B���h�E�ɃN���X��o�^����
        LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        return 0L;
    }
} //Framework::Window
