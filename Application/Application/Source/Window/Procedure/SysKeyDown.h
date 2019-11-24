#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class SysKeyDown
    * @brief システムキーが押された時の処理
    */
    class SysKeyDown : public IWindowProc {
    public:
        /**
        * @brief コンストラクタ
        */
        SysKeyDown() { }
        /**
        * @brief デストラクタ
        */
        virtual ~SysKeyDown() { }
        /**
        * @brief ウィンドウプロシージャ
        */
        virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} //Framework::Window
