#pragma once
#include <Windows.h>
#include <dxgi1_6.h>
#include "Device/ISystemEventNotify.h"

namespace Framework::Window {
    /**
    * @class Window
    * @brief ウィンドウクラス
    */
    class Window {
    public:
        /**
        * @brief コンストラクタ
        */
        Window(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance, Device::ISystemEventNotify* notify);
        /**
        * @brief デストラクタ
        */
        ~Window();
        /**
        * @brief ウィンドウを表示する
        */
        void show(int nCmdShow);
        /**
        * @brief ウィンドウハンドルを取得する
        */
        inline HWND getHWnd() const { return mHWnd; }
        /**
        * @brief フルスクリーンか？
        */
        inline bool isFullScreen()const { return mIsFullScreen; }

        void toggleFullScreenWindow(IDXGISwapChain* swapChain);

        void setWindowZOrderToTopMost(bool setToTopMost) const;
        UINT getWidth() const { return mWindowRect.right - mWindowRect.left; }
        UINT getHeight() const { return mWindowRect.bottom - mWindowRect.top; }
        RECT getWindowRect() const { return mWindowRect; }
    private:
        static constexpr UINT WINDOW_STYLE = WS_OVERLAPPEDWINDOW;
    private:
        HWND mHWnd; //!< ウィンドウハンドル
        bool mIsFullScreen; //!< フルスクリーンか
        RECT mWindowRect; //!< ウィンドウ矩形
    };

} //Framework::Window
