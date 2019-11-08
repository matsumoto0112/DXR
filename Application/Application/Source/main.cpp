#include "Framework/Game.h"
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

        mScene = std::make_unique<Scene>(mDeviceResource.get(), mWidth, mHeight);
        mScene->create();
    }
    void onUpdate() override {
        Game::onUpdate();
        mScene->update();
    }
    void onRender() override {
        //Game::renderStart();

        mScene->render();
        //Game::renderEnd();
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
