#include "CreateProc.h"

namespace Framework::Window {
    //プロシージャ
    LRESULT CreateProc::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        //WM_CREATEのみ処理する
        if (msg != WM_CREATE) return 0L;
        *isReturn = true;
        //ウィンドウにクラスを登録する
        LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        return 0L;
    }
} //Framework::Window
