#pragma once
#include <Windows.h>
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
    private:
        static constexpr UINT WINDOW_STYLE = WS_OVERLAPPEDWINDOW;
    private:
        HWND mHWnd;
        bool mIsFullScreen;
        RECT mWindowRect;
    };

} //Framework::Window
