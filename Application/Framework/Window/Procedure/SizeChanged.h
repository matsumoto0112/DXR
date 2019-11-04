#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class SizeChanged
    * @brief ウィンドウのサイズが変更された時の処理
    */
    class SizeChanged : public IWindowProc {
    public:
        /**
        * @brief コンストラクタ
        */
        SizeChanged() { }
        /**
        * @brief デストラクタ
        */
        virtual ~SizeChanged() { }
        /**
        * @brief ウィンドウプロシージャ
        */
        virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} //Framework::Window
