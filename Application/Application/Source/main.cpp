#include "Framework/Game.h"
#include "Utility/Debug.h"
#include "Scene.h"

/**
* @class MainApp
* @brief ���C���A�v���P�[�V����
*/
class MainApp : public Framework::Game {
public:
    /**
    * @brief �R���X�g���N�^
    */
    MainApp(UINT width, UINT height, const std::wstring& title)
        :Game(width, height, title) { }
    /**
    * @brief �f�X�g���N�^
    */
    ~MainApp() { }
    void onInit() override {
        Game::onInit();
        mDebugWindow = std::make_unique<Framework::ImGUI::Window>("Debug");
        mFPSText = std::make_shared<Framework::ImGUI::Text>("FPS:");
        mDebugWindow->addItem(mFPSText);

        mScene = std::make_unique<Scene>(mDeviceResource.get());
        mScene->create();
    }
    void onUpdate() override {
        Game::onUpdate();
    }
    void onRender() override {
        Game::renderStart();

        mDebugWindow->draw();

        Game::renderEnd();
    }
    void onDestroy()override {
        Game::onDestroy();
    }
private:
    std::unique_ptr<Framework::ImGUI::Window> mDebugWindow;
    std::shared_ptr<Framework::ImGUI::Text> mFPSText;
    std::unique_ptr<Scene> mScene;
};

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    MainApp app(1280, 720, L"Game");
    return app.run(hInstance, nCmdShow);
}
