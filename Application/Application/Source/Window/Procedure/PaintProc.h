/**
 * @file PaintProc.h
 * @brief 描画イベント時のプロシージャ定義
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class PaintProc
     * @brief 描画イベント処理
     */
    class PaintProc : public IWindowProc {
    public:
        /**
         * @brief コンストラクタ
         */
        PaintProc() {}
        /**
         * @brief デストラクタ
         */
        virtual ~PaintProc() {}
        /**
         * @brief ウィンドウプロシージャ
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
