/**
 * @file CreateProc.h
 * @brief ウィンドウ作成時のプロシージャ定義
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class CreateProc
     * @brief ウィンドウ作成時のプロシージャ処理
     */
    class CreateProc : public IWindowProc {
    public:
        /**
         * @brief コンストラクタ
         */
        CreateProc() {}
        /**
         * @brief デストラクタ
         */
        virtual ~CreateProc() {}
        /**
         * @brief ウィンドウプロシージャ
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
