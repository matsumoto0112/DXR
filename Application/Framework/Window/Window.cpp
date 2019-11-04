#include "Window.h"
#include "Window/Procedure/Procedures.h"

namespace Framework::Window {

    Window::Window(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance, Device::ISystemEventNotify* notify) {
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = Procedures::mainWndProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = title.c_str();
        RegisterClassEx(&windowClass);

        RECT windowRect = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
        AdjustWindowRect(&windowRect, WINDOW_STYLE, FALSE);

        mHWnd = CreateWindow(
            windowClass.lpszClassName,
            title.c_str(),
            WINDOW_STYLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            hInstance,
            notify);
    }
    Window::~Window() { }

    void Window::show(int nCmdShow) {
        ShowWindow(mHWnd, nCmdShow);
    }

} //Framework::Window
