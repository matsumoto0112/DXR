#include "GameDevice.h"
#include "Window/Procedure/CreateProc.h"
#include "Window/Procedure/DestroyProc.h"
#include "Window/Procedure/ImGuiProc.h"
#include "Window/Procedure/PaintProc.h"
#include "Window/Procedure/Procedures.h"
#include "Window/Procedure/SizeChanged.h"
#include "Window/Procedure/SysKeyDown.h"
#include "Window/Procedure/WindowMoved.h"

namespace Framework::Device {
    //コンストラクタ
    GameDevice::GameDevice() {
        Window::Procedures::addProc(new Window::CreateProc());
        Window::Procedures::addProc(new Window::DestroyProc());
        Window::Procedures::addProc(new Window::PaintProc());
        Window::Procedures::addProc(new Window::SysKeyDown());
        Window::Procedures::addProc(new Window::SizeChanged());
        Window::Procedures::addProc(new Window::WindowMoved());
        Window::Procedures::addProc(new Window::ImGuiProc());
    }
    //デストラクタ
    GameDevice::~GameDevice() {}
    //初期化処理
    void GameDevice::init(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance,
        DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, ISystemEventNotify* notify, UINT flags) {
        mNotify = notify;
        mWindow = std::make_unique<Window::Window>(width, height, title, hInstance, this);

        mDeviceResource
            = std::make_unique<DX::DeviceResource>(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

        mDeviceResource->setWindow(mWindow.get(), width, height);
        mDeviceResource->initializeDXGIAdapter();

        mDeviceResource->createDeviceResources();
        mDeviceResource->createWindowDependentResources();

        mInputManager = std::make_unique<Input::InputManager>(mWindow->getHWnd());
        mImGuiManager
            = std::make_unique<ImGuiManager>(mWindow->getHWnd(), mDeviceResource->getDevice(),
                mDeviceResource->getBackBufferFormat(), mDeviceResource->getBackBufferCount());
    }
    //アプリケーション開始
    void GameDevice::run(int nCmdShow) {
        mWindow->show(nCmdShow);
    }
    //終了時処理
    void GameDevice::finalize() {
        mDeviceResource->waitForGPU();
    }
    //フレームイベント
    void GameDevice::onFrameEvent() {
        beginFrame();

        mNotify->onFrameEvent();

        endFrame();
    }
    //フルスクリーンモード切替
    void GameDevice::toggleFullScreenWindow() {
        if (!mDeviceResource->isTearingSupported()) return;
        mWindow->toggleFullScreenWindow(mDeviceResource->getSwapChain());
        mNotify->toggleFullScreenWindow();
    }
    //ウィンドウ境界の設定
    void GameDevice::setWindowBounds(const RECT& rect) {
        mNotify->setWindowBounds(rect);
    }
    //ウィンドウサイズ切り替え
    void GameDevice::onSizeChanged(UINT width, UINT height, bool minimized) {
        mDeviceResource->windowSizeChanged(width, height, minimized);
        mNotify->onSizeChanged(width, height, minimized);
    }
    //ウィンドウ移動
    void GameDevice::onWindowMoved(int x, int y) {
        mNotify->onWindowMoved(x, y);
    }
    //フレーム開始時処理
    void GameDevice::beginFrame() {
        mDeviceResource->prepare();
        mInputManager->update();
        mImGuiManager->beginFrame();
    }
    //フレーム終了時処理
    void GameDevice::endFrame() {
        mImGuiManager->endFrame(mDeviceResource->getCommandList());
        mDeviceResource->present();
    }
} // namespace Framework::Device
