#include "Game.h"
#include "ImGui/ImGuiManager.h"
#include "Utility/Debug.h"
#include "Window/Procedure/CreateProc.h"
#include "Window/Procedure/DestroyProc.h"
#include "Window/Procedure/PaintProc.h"
#include "Window/Procedure/Procedures.h"
#include "Window/Procedure/SysKeyDown.h"
#include "Window/Procedure/SizeChanged.h"
#include "Window/Procedure/ImGuiProc.h"
#include "Window/Procedure/WindowMoved.h"

namespace Framework {
    //�R���X�g���N�^
    Game::Game(UINT width, UINT height, const std::wstring& title)
        :mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr) {
        Window::Procedures::addProc(new Window::CreateProc());
        Window::Procedures::addProc(new Window::DestroyProc());
        Window::Procedures::addProc(new Window::PaintProc());
        Window::Procedures::addProc(new Window::SysKeyDown());
        Window::Procedures::addProc(new Window::SizeChanged());
        Window::Procedures::addProc(new Window::WindowMoved());
        Window::Procedures::addProc(new Window::ImGuiProc());
    }
    //�f�X�g���N�^
    Game::~Game() { }
    //���s
    int Game::run(HINSTANCE hInstance, int nCmdShow) {
        try {
            mWindow = std::make_unique<Window::Window>(mWidth, mHeight, mTitle, hInstance, this);

            this->onInit();
            //��������ɃE�B���h�E��\��
            mWindow->show(nCmdShow);

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
        }
        catch (const std::exception& e) {
            MY_DEBUG_LOG("�G���[ �A�v���P�[�V�����ɖ�肪�������܂���\n%s\n", e.what());

            this->onDestroy();
            return EXIT_FAILURE;
        }

    }

    //������
    void Game::onInit() {
        mDeviceResource = std::make_unique<DX::DeviceResource>(
            DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
            DX::DeviceResource::REQUIRE_TEARING_SUPPORT);

        mDeviceResource->setWindow(mWindow.get());
        mDeviceResource->initializeDXGIAdapter();

        mDeviceResource->createDeviceResources();
        mDeviceResource->createWindowDependentResources();

        ImGuiManager::getInstance()->init(mWindow->getHWnd(), mDeviceResource->getDevice(), mDeviceResource->getBackBufferFormat());
    }
    //�X�V
    void Game::onUpdate() { }
    void Game::renderStart() {
        mDeviceResource->prepare();
        Framework::ImGuiManager::getInstance()->beginFrame();

        //�����_�[�^�[�Q�b�g�̃N���A
        ID3D12GraphicsCommandList* list = mDeviceResource->getCommandList();
        D3D12_CPU_DESCRIPTOR_HANDLE rtv[] = { mDeviceResource->getRenderTargetView() };

        list->OMSetRenderTargets(1, rtv, FALSE, &mDeviceResource->getDepthStencilView());
        static float color[4] = { 0.0f,0.0f,0.0f,0.0f };
        list->ClearRenderTargetView(mDeviceResource->getRenderTargetView(), color, 0, nullptr);
    }
    void Game::renderEnd() {
        Framework::ImGuiManager::getInstance()->endFrame(mDeviceResource->getCommandList());

        mDeviceResource->present();
    }
    //�I����
    void Game::onDestroy() {
        mDeviceResource->waitForGPU();
    }
    //�t���X�N���[���؂�ւ�
    void Game::toggleFullScreenWindow() {
        if (!mDeviceResource->isTearingSupported())return;
        mWindow->toggleFullScreenWindow(mDeviceResource->getSwapChain());
    }
    //�E�B���h�E�T�C�Y�̕ύX
    void Game::updateForSizeChange(UINT clientWidth, UINT clientHeight) {
        mWidth = clientWidth;
        mHeight = clientHeight;
        mAspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);
    }
    //�E�B���h�E�̋�`�̃Z�b�g
    void Game::setWindowBounds(const RECT& rect) {
        mWindowBounds = rect;
    }
    //�E�B���h�E�T�C�Y�̕ύX�C�x���g
    void Game::onSizeChanged(UINT width, UINT height, bool minimized) {
        mDeviceResource->windowSizeChanged(width, height, minimized);
        updateForSizeChange(width, height);
    }
    //�E�B���h�E�̈ړ�
    void Game::onWindowMoved(int, int) { }

} //Framework
