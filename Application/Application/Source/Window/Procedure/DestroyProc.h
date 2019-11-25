/**
 * @file DestroyProc.h
 * @brief アプリケーション終了時のプロシージャ定義
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class DestroyProc
     * @brief 終了時のプロシージャ処理
     */
    class DestroyProc : public IWindowProc {
    public:
        /**
         * @brief コンストラクタ
         */
        DestroyProc() {}
        /**
         * @brief デストラクタ
         */
        virtual ~DestroyProc() {}
        /**
         * @brief ウィンドウプロシージャ
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
