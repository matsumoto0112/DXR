#include "PaintProc.h"
#include "Device/ISystemEventNotify.h"

namespace Framework::Window {
    //�v���V�[�W��
    LRESULT PaintProc::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        if (msg == WM_PAINT) {
            *isReturn = true;
            //�`��C�x���g�𔭍s����
            Device::ISystemEventNotify* notify = reinterpret_cast<Device::ISystemEventNotify*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            if (notify) {
                notify->onUpdate();
                notify->onRender();
            }
        }
        return 0L;
    }

} //Framework::Window
