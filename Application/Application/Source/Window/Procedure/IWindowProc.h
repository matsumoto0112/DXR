/**
 * @file IWindowProc.h
 * @brief ウィンドウプロシージャインターフェース定義
 */

#pragma once

namespace Framework::Window {
    /**
     * @class IWindowProc
     * @brief ウィンドウプロシージャインターフェース
     */
    class IWindowProc {
    public:
        /**
         * @brief デストラクタ
         */
        virtual ~IWindowProc() = default;
        /**
         * @brief ウィンドウプロシージャ
         * @param hWnd ウィンドウハンドル
         * @param msg メッセージの種類
         * @param wParam パラメータ
         * @param lParam パラメータ
         * @param[out] isReturn メッセージを処理したか
         */
        virtual LRESULT CALLBACK wndProc(
            HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool* isReturn)
            = 0;
    };
} // namespace Framework::Window
