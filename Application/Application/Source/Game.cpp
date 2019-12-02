#include "Game.h"
#include "Device/GameDevice.h"
#include "ImGui/ImGuiManager.h"
#include "Utility/Debug.h"
namespace Framework {
    //�R���X�g���N�^
    Game::Game(UINT width, UINT height, const std::wstring& title)
        : mWidth(width), mHeight(height), mTitle(title) {}
    //�f�X�g���N�^
    Game::~Game() {}
    //���s
    int Game::run(HINSTANCE hInstance, int nCmdShow) {
        try {
            Device::GameDevice::getInstance()->init(mWidth, mHeight, mTitle, hInstance,
                DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, this,
                DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

            this->onInit();
            //��������ɃE�B���h�E��\��
            Device::GameDevice::getInstance()->run(nCmdShow);

            MSG msg = {};
            //���C�����[�v
            while (msg.message != WM_QUIT) {
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            this->onDestroy();

            return static_cast<char>(msg.wParam);
        } catch (const std::exception& e) {
            OutputDebugStringA("�G���[ �A�v���P�[�V�����ɖ�肪�������܂���\n");
            OutputDebugStringA(e.what());
            OutputDebugStringA("/n");
            __debugbreak();
            this->onDestroy();
            return EXIT_FAILURE;
        }
    }

    //������
    void Game::onInit() {}
    //�X�V
    void Game::onUpdate() {}
    void Game::onRender() {}
    //�I����
    void Game::onDestroy() {
        Device::GameDevice::getInstance()->finalize();
    }
    void Game::onFrameEvent() {
        onUpdate();
        onRender();
    }

} // namespace Framework
