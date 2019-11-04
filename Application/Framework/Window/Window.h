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
        /**
        * @brief フルスクリーンをと非フルスクリーンを切り替える
        * @param swapChain スワップチェイン
        */
        void toggleFullScreenWindow(IDXGISwapChain* swapChain);
        /**
        * @brief ウィンドウのZオーダーを操作する
        * @param setToTopMost 最前面に出すか
        */
        void setWindowZOrderToTopMost(bool setToTopMost) const;
        /**
        * @brief ウィンドウの幅を取得する
        */
        UINT getWidth() const { return mWindowRect.right - mWindowRect.left; }
        /**
        * @brief ウィンドウの高さを取得する
        */
        UINT getHeight() const { return mWindowRect.bottom - mWindowRect.top; }
        /**
        * @brief ウィンドウ矩形を取得する
        */
        RECT getWindowRect() const { return mWindowRect; }
    private:
        static constexpr UINT WINDOW_STYLE = WS_OVERLAPPEDWINDOW; //!< ウィンドウのスタイル
    private:
        HWND mHWnd; //!< ウィンドウハンドル
        bool mIsFullScreen; //!< フルスクリーンか
        RECT mWindowRect; //!< ウィンドウ矩形
    };

} //Framework::Window
