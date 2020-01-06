#include "Device/GameDevice.h"
#include "Game.h"
#include "Scene.h"
#include "Utility/Debug.h"

class MainApp : public Framework::Game {
public:
    /**
     * @brief コンストラクタ
     */
    MainApp(UINT width, UINT height, const std::wstring& title) : Game(width, height, title) {}
    /**
     * @brief
     */
    ~MainApp() {}
    /**
     * @brief
     */
    void onInit() override {
        Game::onInit();

        mScene = std::make_unique<Scene>(
            Framework::Device::GameDevice::getInstance()->getDeviceResource(),
            Framework::Device::GameDevice::getInstance()->getInputManager(), mWidth, mHeight);
        mScene->create();
    }
    /**
     * @brief
     */
    void onUpdate() override {
        Game::onUpdate();
        mScene->update();
    }
    /**
     * @brief
     */
    void onRender() override {
        auto deviceResource = Framework::Device::GameDevice::getInstance()->getDeviceResource();
        ID3D12GraphicsCommandList* list = deviceResource->getCommandList();
        D3D12_CPU_DESCRIPTOR_HANDLE rtv[]
            = { deviceResource->getRenderTarget()->getView().getInfo().cpuHandle };

        list->OMSetRenderTargets(
            1, rtv, FALSE, &deviceResource->getDepthStencil()->getView().getInfo().cpuHandle);
        deviceResource->getRenderTarget()->clear(list, Framework::Utility::Color4(0, 0, 0, 0));
        deviceResource->getDepthStencil()->clear(list);

        mScene->render();
    }
    /**
     * @brief
     */
    void onDestroy() override {
        Game::onDestroy();
    }
    /**
     * @brief
     */
    virtual void toggleFullScreenWindow() override {
        Game::toggleFullScreenWindow();
    }
    /**
     * @brief
     */
    virtual void setWindowBounds(const RECT& rect) override {
        Game::setWindowBounds(rect);
    }
    /**
     * @brief
     */
    virtual void onSizeChanged(UINT width, UINT height, bool minimized) override {
        Game::onSizeChanged(width, height, minimized);
        mScene->onWindowSizeChanged(width, height);
    }
    /**
     * @brief
     */
    virtual void onWindowMoved(int x, int y) override {
        Game::onWindowMoved(x, y);
    }

private:
    std::unique_ptr<Scene> mScene;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    MainApp app(1280, 720, L"Game");
    return app.run(hInstance, nCmdShow);
}
