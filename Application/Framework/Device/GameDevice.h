#pragma once
#include <d3d12.h>
#include "Device/ISystemEventNotify.h"
#include "DX/DeviceResource.h"
#include "ImGui/ImGuiManager.h"
#include "Input/InputManager.h"
#include "Utility/Singleton.h"
#include "Window/Window.h"

namespace Framework::Device {
    /**
    * @class GameDevice
    * @brief discription
    */
    class GameDevice : public Utility::Singleton<GameDevice>, public ISystemEventNotify {
    public:
        void init(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance,
            DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, ISystemEventNotify* notify, UINT flags);
        void run(int nCmdShow);
        void finalize();
        Window::Window* getWindow() const { return mWindow.get(); }
        DX::DeviceResource* getDeviceResource() const { return mDeviceResource.get(); }
        Input::InputManager* getInputManager() const { return mInputManager.get(); }
        virtual void onFrameEvent() override;
        virtual void toggleFullScreenWindow() override;
        virtual void updateForSizeChange(UINT clientWidth, UINT clientHeight) override;
        virtual void setWindowBounds(const RECT& rect) override;
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override;
        virtual void onWindowMoved(int x, int y) override;
    protected:
        /**
        * @brief �R���X�g���N�^
        */
        GameDevice();
        /**
        * @brief �f�X�g���N�^
        */
        ~GameDevice();
    private:
        UniquePtr<Window::Window> mWindow; //!< �E�B���h�E
        UniquePtr<DX::DeviceResource> mDeviceResource; //!< �f�o�C�X���\�[�X
        UniquePtr<Input::InputManager> mInputManager; //!< ���͊Ǘ�
        UniquePtr<ImGuiManager> mImGuiManager;
        ISystemEventNotify* mNotify;

        void beginFrame();
        void endFrame();
    };
} //Framework::Device
