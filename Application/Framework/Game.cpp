#include "Game.h"
#include "Device/GameDevice.h"
#include "ImGui/ImGuiManager.h"
#include "Utility/Debug.h"
#include "Window/Procedure/CreateProc.h"
#include "Window/Procedure/DestroyProc.h"
#include "Window/Procedure/PaintProc.h"
#include "Window/Procedure/Procedures.h"
#include "Window/Procedure/SysKeyDown.h"
#include "Window/Procedure/SizeChanged.h"
#include "Window/Procedure/ImGuiProc.h"
#include "Window/Procedure/WindowMoved.h"

namespace Framework {
    //コンストラクタ
    Game::Game(UINT width, UINT height, const std::wstring& title)
        :mWidth(width), mHeight(height), mTitle(title) {
        Window::Procedures::addProc(new Window::CreateProc());
        Window::Procedures::addProc(new Window::DestroyProc());
        Window::Procedures::addProc(new Window::PaintProc());
        Window::Procedures::addProc(new Window::SysKeyDown());
        Window::Procedures::addProc(new Window::SizeChanged());
        Window::Procedures::addProc(new Window::WindowMoved());
        Window::Procedures::addProc(new Window::ImGuiProc());
    }
    //デストラクタ
    Game::~Game() { }
    //実行
    int Game::run(HINSTANCE hInstance, int nCmdShow) {
        try {
            //Device::GameDevice::getInstance();
            Device::GameDevice::getInstance()->init(mWidth, mHeight, mTitle,
                hInstance,
                DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
                DX::DeviceResource::REQUIRE_TEARING_SUPPORT);
            Device::GameDevice::getInstance()->setNotify(this);
            //mWindow = std::make_unique<Window::Window>(mWidth, mHeight, mTitle, hInstance, this);

            this->onInit();
            //初期化後にウィンドウを表示
            //mWindow->show(nCmdShow);
            Device::GameDevice::getInstance()->run(nCmdShow);

            MSG msg = {};
            //メインループ
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
            MY_THROW_IF_FALSE_LOG(false, "エラー アプリケーションに問題が発生しました\n%s\n", e.what());

            this->onDestroy();
            return EXIT_FAILURE;
        }

    }

    //初期化
    void Game::onInit() {
        //mDeviceResource = std::make_unique<DX::DeviceResource>(
        //    DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
        //    DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
        //    DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

        //mDeviceResource->setWindow(mWindow.get(), mWidth, mHeight);
        //mDeviceResource->initializeDXGIAdapter();

        //mDeviceResource->createDeviceResources();
        //mDeviceResource->createWindowDependentResources();

        //mInputManager = std::make_unique<Input::InputManager>(mWindow->getHWnd());
        //mImGuiManager = std::make_unique<ImGuiManager>(mWindow->getHWnd(), mDeviceResource->getDevice(), mDeviceResource->getBackBufferFormat());
    }
    //更新
    void Game::onUpdate() {
        //mInputManager->update();
    }
    void Game::onRender() { }
    //描画開始
    void Game::renderStart() {
        //mDeviceResource->prepare();
        //mImGuiManager->beginFrame();

        //レンダーターゲットのクリア
        ID3D12GraphicsCommandList* list = Device::GameDevice::getInstance()->getDeviceResource()->getCommandList();
        D3D12_CPU_DESCRIPTOR_HANDLE rtv[] = { Device::GameDevice::getInstance()->getDeviceResource()->getRenderTargetView() };

        list->OMSetRenderTargets(1, rtv, FALSE, &Device::GameDevice::getInstance()->getDeviceResource()->getDepthStencilView());
        static float color[4] = { 0,0,0,0 };
        list->ClearRenderTargetView(Device::GameDevice::getInstance()->getDeviceResource()->getRenderTargetView(), color, 0, nullptr);
    }
    //描画終了
    void Game::renderEnd() {
        //mImGuiManager->endFrame(mDeviceResource->getCommandList());

        //mDeviceResource->present();
    }
    //終了時
    void Game::onDestroy() {
        //mDeviceResource->waitForGPU();
    }
    void Game::onFrameEvent() {
        onUpdate();
        onRender();
    }

    ////フルスクリーン切り替え
    //void Game::toggleFullScreenWindow() {
    //    if (!mDeviceResource->isTearingSupported())return;
    //    mWindow->toggleFullScreenWindow(mDeviceResource->getSwapChain());
    //}
    ////ウィンドウサイズの変更
    //void Game::updateForSizeChange(UINT clientWidth, UINT clientHeight) {
    //    mWidth = clientWidth;
    //    mHeight = clientHeight;
    //    mAspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);
    //}
    ////ウィンドウの矩形のセット
    //void Game::setWindowBounds(const RECT& rect) {
    //    mWindowBounds = rect;
    //}
    ////ウィンドウサイズの変更イベント
    //void Game::onSizeChanged(UINT width, UINT height, bool minimized) {
    //    mDeviceResource->windowSizeChanged(width, height, minimized);
    //    updateForSizeChange(width, height);
    //}
    ////ウィンドウの移動
    //void Game::onWindowMoved(int, int) { }

} //Framework
