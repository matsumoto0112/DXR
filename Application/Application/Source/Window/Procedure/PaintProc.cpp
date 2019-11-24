#include "PaintProc.h"
#include "Device/ISystemEventNotify.h"

namespace Framework::Window {
    //�v���V�[�W��
    LRESULT PaintProc::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) {
        //WM_PAINT�̂ݏ�������
        if (msg != WM_PAINT) return 0L;
        *isReturn = true;
        //�`��C�x���g�𔭍s����
        Device::ISystemEventNotify* notify = reinterpret_cast<Device::ISystemEventNotify*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (!notify) return 0L;

        notify->onFrameEvent();
        return 0L;
    }

} //Framework::Window
