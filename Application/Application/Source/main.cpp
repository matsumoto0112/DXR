#include "Framework/Game.h"
#include "Device/GameDevice.h"
#include "Utility/Debug.h"
#include "Scene.h"

/**
* @class MainApp
* @brief メインアプリケーション
*/
class MainApp : public Framework::Game {
public:
    /**
    * @brief コンストラクタ
    */
    MainApp(UINT width, UINT height, const std::wstring& title)
        :Game(width, height, title) { }
    /**
    * @brief デストラクタ
    */
    ~MainApp() { }
    void onInit() override {
        Game::onInit();

        mScene = std::make_unique<Scene>(Framework::Device::GameDevice::getInstance()->getDeviceResource(),
            Framework::Device::GameDevice::getInstance()->getInputManager(), mWidth, mHeight);
        mScene->create();
    }
    void onUpdate() override {
        Game::onUpdate();
        mScene->update();
    }
    void onRender() override {
        auto deviceResource = Framework::Device::GameDevice::getInstance()->getDeviceResource();
        //レンダーターゲットのクリア
        ID3D12GraphicsCommandList* list = deviceResource->getCommandList();
        D3D12_CPU_DESCRIPTOR_HANDLE rtv[] = { deviceResource->getRenderTargetView() };

        list->OMSetRenderTargets(1, rtv, FALSE, &deviceResource->getDepthStencilView());
        static float color[4] = { 0,0,0,0 };
        list->ClearRenderTargetView(deviceResource->getRenderTargetView(), color, 0, nullptr);

        mScene->render();
    }
    void onDestroy()override {
        Game::onDestroy();
    }
private:
    std::unique_ptr<Scene> mScene;
};

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    MainApp app(1280, 720, L"Game");
    return app.run(hInstance, nCmdShow);
}
