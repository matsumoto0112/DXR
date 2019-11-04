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
        * @brief コンストラクタ
        */
        Game(UINT width, UINT height, const std::wstring& title);
        /**
        * @brief デストラクタ
        */
        virtual ~Game();
        int run(HINSTANCE hInstance, int nCmdShow);
        virtual void onInit() override;
        virtual void onUpdate() override;
        virtual void onRender() override;
        virtual void onDestroy() override;
        virtual void toggleFullScreenWindow() override;
        virtual void updateForSizeChange(UINT clientWidth, UINT cliendHeight) override;
        virtual void setWindowBounds(const RECT & rect) override;
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override;
        virtual void onWindowMoved(int x, int y) override;
    protected:
        UINT mWidth;
        UINT mHeight;
        float mAspectRatio;
        RECT mWindowBounds;
        std::wstring mTitle;
        std::unique_ptr<Window::Window> mWindow;
        std::unique_ptr<DX::DeviceResource> mDeviceResource;
    };
} //Framework
