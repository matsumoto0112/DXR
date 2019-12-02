#include "Game.h"
#include "Device/GameDevice.h"
#include "ImGui/ImGuiManager.h"
#include "Utility/Debug.h"
namespace Framework {
    //コンストラクタ
    Game::Game(UINT width, UINT height, const std::wstring& title)
        : mWidth(width), mHeight(height), mTitle(title) {}
    //デストラクタ
    Game::~Game() {}
    //実行
    int Game::run(HINSTANCE hInstance, int nCmdShow) {
        try {
            Device::GameDevice::getInstance()->init(mWidth, mHeight, mTitle, hInstance,
                DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, this,
                DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

            this->onInit();
            //初期化後にウィンドウを表示
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
        } catch (const std::exception& e) {
            OutputDebugStringA("エラー アプリケーションに問題が発生しました\n");
            OutputDebugStringA(e.what());
            OutputDebugStringA("/n");
            __debugbreak();
            this->onDestroy();
            return EXIT_FAILURE;
        }
    }

    //初期化
    void Game::onInit() {}
    //更新
    void Game::onUpdate() {}
    void Game::onRender() {}
    //終了時
    void Game::onDestroy() {
        Device::GameDevice::getInstance()->finalize();
    }
    void Game::onFrameEvent() {
        onUpdate();
        onRender();
    }

} // namespace Framework
