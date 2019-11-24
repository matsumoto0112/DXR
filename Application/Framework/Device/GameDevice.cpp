#include "GameDevice.h"

namespace Framework::Device {

    GameDevice::GameDevice() { }
    GameDevice::~GameDevice() { }

    void GameDevice::init(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance,
        DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat,
        UINT flags) {
        mWindow = std::make_unique<Window::Window>(width, height, title, hInstance, this);

        mDeviceResource = std::make_unique<DX::DeviceResource>(
            DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
            DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

        mDeviceResource->setWindow(mWindow.get(), width, height);
        mDeviceResource->initializeDXGIAdapter();

        mDeviceResource->createDeviceResources();
        mDeviceResource->createWindowDependentResources();

        mInputManager = std::make_unique<Input::InputManager>(mWindow->getHWnd());
        mImGuiManager = std::make_unique<ImGuiManager>(mWindow->getHWnd(), mDeviceResource->getDevice(), mDeviceResource->getBackBufferFormat());
    }
    void GameDevice::run(int nCmdShow) {
        mWindow->show(nCmdShow);
    }
    void GameDevice::finalize() {
        mDeviceResource->waitForGPU();
    }
    void GameDevice::onFrameEvent() {
        beginFrame();

        mNotify->onFrameEvent();

        endFrame();
    }
    void GameDevice::toggleFullScreenWindow() {
        if (!mDeviceResource->isTearingSupported())return;
        mWindow->toggleFullScreenWindow(mDeviceResource->getSwapChain());
        mNotify->toggleFullScreenWindow();
    }
    void GameDevice::updateForSizeChange(UINT clientWidth, UINT cliendHeight) { }
    void GameDevice::setWindowBounds(const RECT& rect) { }
    void GameDevice::onSizeChanged(UINT width, UINT height, bool minimized) {
        mDeviceResource->windowSizeChanged(width, height, minimized);
        updateForSizeChange(width, height);
    }
    void GameDevice::onWindowMoved(int x, int y) { }
    void GameDevice::beginFrame() {
        mDeviceResource->prepare();
        mInputManager->update();
        mImGuiManager->beginFrame();
    }
    void GameDevice::endFrame() {
        mImGuiManager->endFrame(mDeviceResource->getCommandList());
        mDeviceResource->present();
    }
} //Framework::Device
