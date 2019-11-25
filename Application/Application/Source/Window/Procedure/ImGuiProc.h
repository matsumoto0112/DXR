/**
 * @file ImGuiProc.h
 * @brief ImGuiのプロシージャ定義
 */

#pragma once
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
     * @class ImGuiProc
     * @brief ImGuiプロシージャ
     */
    class ImGuiProc : public IWindowProc {
    public:
        /**
         * @brief コンストラクタ
         */
        ImGuiProc() {}
        /**
         * @brief デストラクタ
         */
        virtual ~ImGuiProc() {}
        /**
         * @brief ウィンドウプロシージャ
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn) override;
    };
} // namespace Framework::Window
