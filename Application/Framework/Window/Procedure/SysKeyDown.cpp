#include "SysKeyDown.h"
#include "Device/ISystemEventNotify.h"

namespace {
    inline bool pushAltEnter(WPARAM wParam, LPARAM lParam) {
        return ((wParam == VK_RETURN) && (lParam & (1 << 29)));
    }
}

namespace Framework::Window {
    //�v���V�[�W��
    LRESULT SysKeyDown::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        //WM_SYSKEYDOWN�̖���������
        if (msg != WM_SYSKEYDOWN) return 0L;
        //ALT+ENTER�̖���������
        if (!pushAltEnter(wParam, lParam))return 0L;

        *isReturn = true;
        Device::ISystemEventNotify* notify = reinterpret_cast<Device::ISystemEventNotify*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (!notify) return 0L;

        notify->toggleFullScreenWindow();
        return 0L;
    }

} //Framework::Window
