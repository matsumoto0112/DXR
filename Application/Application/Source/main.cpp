#include "Framework/Game.h"
#include "Utility/Debug.h"

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
        MY_DEBUG_LOG("OnInit\n");
    }
    void onUpdate() override {
        MY_DEBUG_LOG("OnUpdate\n");
    }
    void onRender() override {
        MY_DEBUG_LOG("Render\n");
   }
    void onDestroy()override {
        MY_DEBUG_LOG("OnDestroy\n");
    }
private:

};

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    MainApp app(1280, 720, L"Game");
    return app.run(hInstance, nCmdShow);
}
