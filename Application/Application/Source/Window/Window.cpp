#include "Window.h"
#include "Window/Procedure/Procedures.h"

namespace Framework::Window {
    //コンストラクタ
    Window::Window(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance,
        Device::ISystemEventNotify* notify) {
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = Procedures::mainWndProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = title.c_str();
        RegisterClassEx(&windowClass);

        RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
        AdjustWindowRect(&windowRect, WINDOW_STYLE, FALSE);

        mHWnd = CreateWindow(windowClass.lpszClassName, title.c_str(), WINDOW_STYLE, CW_USEDEFAULT,
            CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
            nullptr, nullptr, hInstance, notify);
    }
    //デストラクタ
    Window::~Window() {}
    //ウィンドウの表示
    void Window::show(int nCmdShow) {
        ShowWindow(mHWnd, nCmdShow);
    }
    //フルスクリーンモード切替
    void Window::toggleFullScreenWindow(IDXGISwapChain* swapChain) {
        //フルスクリーンからウィンドウモード
        if (mIsFullScreen) {
            SetWindowLong(mHWnd, GWL_STYLE, WINDOW_STYLE);

            SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowRect.left, mWindowRect.top,
                mWindowRect.right - mWindowRect.left, mWindowRect.bottom - mWindowRect.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ShowWindow(mHWnd, SW_NORMAL);
        }
        //ウィンドウモードからフルスクリーンモード
        else {
            GetWindowRect(mHWnd, &mWindowRect);

            SetWindowLong(mHWnd, GWL_STYLE,
                WINDOW_STYLE
                    & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

            RECT fullScreenWindowRect;
            try {
                if (swapChain) {
                    Comptr<IDXGIOutput> output;
                    MY_THROW_IF_FAILED(swapChain->GetContainingOutput(&output));
                    DXGI_OUTPUT_DESC desc;
                    MY_THROW_IF_FAILED(output->GetDesc(&desc));
                    fullScreenWindowRect = desc.DesktopCoordinates;
                } else {
                    throw Utility::HrException(S_FALSE);
                }
            } catch (const std::exception&) {
                DEVMODE devMode = {};
                devMode.dmSize = sizeof(DEVMODE);
                EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

                fullScreenWindowRect = { devMode.dmPosition.x, devMode.dmPosition.y,
                    devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
                    devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight) };
            }

            SetWindowPos(mHWnd, HWND_TOPMOST, fullScreenWindowRect.left, fullScreenWindowRect.top,
                fullScreenWindowRect.right, fullScreenWindowRect.bottom,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ShowWindow(mHWnd, SW_MAXIMIZE);
        }

        mIsFullScreen = !mIsFullScreen;
    }
    //ウィンドウのZオーダーを設定
    void Window::setWindowZOrderToTopMost(bool setToTopMost) const {
        RECT windowRect;
        GetWindowRect(mHWnd, &windowRect);

        SetWindowPos(mHWnd, (setToTopMost) ? HWND_TOPMOST : HWND_NOTOPMOST, windowRect.left,
            windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);
    }

} // namespace Framework::Window
