#pragma once
#include <memory>
#include <Windows.h>
#include <string>
#include "Device/ISystemEventNotify.h"
#include "Window/Window.h"
#include "DX/DeviceResource.h"

namespace Framework {
    /**
    * @class Game
    * @brief discription
    */
    class Game :public Device::ISystemEventNotify {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Game(UINT width, UINT height, const std::wstring& title);
        /**
        * @brief �f�X�g���N�^
        */
        virtual ~Game();
        int run(HINSTANCE hInstance, int nCmdShow);
        virtual void onInit() override;
        virtual void onUpdate() override;
        virtual void onRender() override;
        virtual void onDestroy() override;
    protected:
        UINT mWidth;
        UINT mHeight;
        std::wstring mTitle;
        std::unique_ptr<Window::Window> mWindow;
        std::unique_ptr<DX::DeviceResource> mDeviceResource;
    };
} //Framework
