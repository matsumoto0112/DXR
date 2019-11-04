#include "Game.h"
#include "Utility/Debug.h"
#include "Window/Procedure/CreateProc.h"
#include "Window/Procedure/DestroyProc.h"
#include "Window/Procedure/PaintProc.h"
#include "Window/Procedure/Procedures.h"
#include "Window/Procedure/SysKeyDown.h"
#include "Window/Procedure/SizeChanged.h"
#include "Window/Procedure/WindowMoved.h"

namespace Framework {

    Game::Game(UINT width, UINT height, const std::wstring& title)
        :mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr) {
        Window::Procedures::addProc(new Window::CreateProc());
        Window::Procedures::addProc(new Window::DestroyProc());
        Window::Procedures::addProc(new Window::PaintProc());
        Window::Procedures::addProc(new Window::SysKeyDown());
        Window::Procedures::addProc(new Window::SizeChanged());
        Window::Procedures::addProc(new Window::WindowMoved());
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

    void Game::onInit() {
        mDeviceResource = std::make_unique<DX::DeviceResource>(
            DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
            DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

        mDeviceResource->setWindow(mWindow.get());
        mDeviceResource->initializeDXGIAdapter();

        mDeviceResource->createDeviceResources();
        mDeviceResource->createWindowDependentResources();
    }

    void Game::onUpdate() { }

    void Game::onRender() { }

    void Game::onDestroy() {
        mDeviceResource->waitForGPU();
    }

    void Game::toggleFullScreenWindow() {
        if (!mDeviceResource->isTearingSupported())return;
        mWindow->toggleFullScreenWindow(mDeviceResource->getSwapChain());
    }

    void Game::updateForSizeChange(UINT clientWidth, UINT clientHeight) {
        mWidth = clientWidth;
        mHeight = clientHeight;
        mAspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);
    }

    void Game::setWindowBounds(const RECT& rect) {
        mWindowBounds = rect;
    }

    void Game::onSizeChanged(UINT width, UINT height, bool minimized) {
        MY_DEBUG_LOG("W:%u H:%u\n", width, height);
        mDeviceResource->windowSizeChanged(width, height, minimized);
        updateForSizeChange(width, height);
    }

    void Game::onWindowMoved(int, int) { }

} //Framework
