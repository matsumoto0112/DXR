#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class CreateProc
    * @brief ウィンドウ移動時の処理
    */
    class WindowMoved : public IWindowProc {
    public:
        /**
        * @brief コンストラクタ
        */
        WindowMoved() { }
        /**
        * @brief デストラクタ
        */
        virtual ~WindowMoved() { }
        /**
        * @brief ウィンドウプロシージャ
        */
        virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} //Framework::Window
