#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "Window/Procedure/IWindowProc.h"

namespace Framework::Window {
    /**
    * @class Procedures
    * @brief プロシージャ処理
    */
    class Procedures {
    public:
        /**
        * @brief メインウィンドウのプロシージャ処理
        * @param hWnd ウィンドウハンドル
        * @param msg メッセージ
        * @param wParam パラメータ
        * @param lParam パラメータ
        */
        static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        /**
        * @brief プロシージャの追加
        */
        static void addProc(IWindowProc* proc);
    private:
        static std::vector<std::unique_ptr<IWindowProc>> mWindowProcs; //!< プロシージャ処理リスト
    };
} //Framework::Window
