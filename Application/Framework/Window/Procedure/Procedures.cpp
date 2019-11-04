#include "Procedures.h"

namespace Framework::Window {
    std::vector<std::unique_ptr<IWindowProc>> Procedures::mWindowProcs = {}; //!< プロシージャ処理リスト

    //メインプロシージャ
    LRESULT Procedures::mainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        bool isReturn = false;
        //先頭から順に処理できるまで試す
        for (auto&& proc : mWindowProcs) {
            LRESULT res = proc->wndProc(hWnd, msg, wParam, lParam, &isReturn);
            if (isReturn) return res;
        }
        //何もなければデフォルトの処理をする
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    //プロシージャの追加
    void Procedures::addProc(IWindowProc* proc) {
        mWindowProcs.emplace_back(std::unique_ptr<IWindowProc>(proc));
    }
} //Framework::Window
