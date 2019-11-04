#include "SysKeyDown.h"
#include "Device/ISystemEventNotify.h"

namespace Framework::Window {
    //プロシージャ
    LRESULT SysKeyDown::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        if (msg == WM_SYSKEYDOWN) {
            //ALT+ENTER
            if ((wParam == VK_RETURN) && (lParam & (1 << 29))) {
                *isReturn = true;
                Device::ISystemEventNotify* notify = reinterpret_cast<Device::ISystemEventNotify*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
                if (notify) {
                    notify->toggleFullScreenWindow();
                }
            }
        }
        return 0L;
    }

} //Framework::Window
