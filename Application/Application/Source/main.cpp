#include "Framework/Game.h"
#include "Utility/Debug.h"
#include "Framework/ImGui/ImGuiManager.h"

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
    }
    void onUpdate() override {
        Game::onUpdate();
    }
    void onRender() override {
        Game::onRender();

        mDeviceResource->prepare();
        Framework::ImGuiManager::getInstance()->beginFrame();
        ID3D12GraphicsCommandList* list = mDeviceResource->getCommandList();
        D3D12_CPU_DESCRIPTOR_HANDLE rtv[] = { mDeviceResource->getRenderTargetView() };
        list->OMSetRenderTargets(1, rtv, FALSE, nullptr);
        static float color[4] = { 0.0f,0.0f,0.0f,0.0f };
        list->ClearRenderTargetView(mDeviceResource->getRenderTargetView(), color, 0, nullptr);

        ImGui::Begin("TEST WINDOW");
        ImGui::Text("Test");
        ImGui::SliderFloat4("COLOR", color, 0.0f, 1.0f);
        ImGui::End();

        Framework::ImGuiManager::getInstance()->endFrame(mDeviceResource->getCommandList());

        mDeviceResource->present();
    }
    void onDestroy()override {
        Game::onDestroy();
    }
private:
};

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    MainApp app(1280, 720, L"Game");
    return app.run(hInstance, nCmdShow);
}
