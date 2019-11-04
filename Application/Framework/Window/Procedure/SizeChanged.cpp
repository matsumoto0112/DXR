#include "SizeChanged.h"
#include "Device/ISystemEventNotify.h"

namespace Framework::Window {
    //プロシージャ
    LRESULT SizeChanged::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        //WM_SIZEの未処理する
        if (msg != WM_SIZE) return 0L;
        *isReturn = true;
        Device::ISystemEventNotify* notify = reinterpret_cast<Device::ISystemEventNotify*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (!notify) return 0L;

        RECT windowRect = {};
        GetWindowRect(hWnd, &windowRect);
        notify->setWindowBounds(windowRect);

        RECT clientRect = {};
        GetClientRect(hWnd, &clientRect);
        notify->onSizeChanged(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, wParam == SIZE_MINIMIZED);
        return 0L;
    }
} //Framework::Window
