#include "Game.h"
#include "Utility/Debug.h"
#include "Window/Procedure/CreateProc.h"
#include "Window/Procedure/DestroyProc.h"
#include "Window/Procedure/PaintProc.h"
#include "Window/Procedure/Procedures.h"

namespace Framework {

    Game::Game(UINT width, UINT height, const std::wstring& title)
        :mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr) {
        Window::Procedures::addProc(new Window::CreateProc());
        Window::Procedures::addProc(new Window::DestroyProc());
        Window::Procedures::addProc(new Window::PaintProc());
    }

    Game::~Game() { }

    int Game::run(HINSTANCE hInstance, int nCmdShow) {
        try {
            mWindow = std::make_unique<Window::Window>(mWidth, mHeight, mTitle, hInstance, this);

            this->onInit();

            mWindow->show(nCmdShow);

            MSG msg = {};
            while (msg.message != WM_QUIT) {
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            this->onDestroy();

            return static_cast<char>(msg.wParam);
        }
        catch (const std::exception& e) {
            MY_DEBUG_LOG("エラー アプリケーションに問題が発生しました\n%s\n", e.what());

            this->onDestroy();
            return EXIT_FAILURE;
        }

    }

    void Game::onInit() { }

    void Game::onUpdate() { }

    void Game::onRender() { }

    void Game::onDestroy() { }

} //Framework
