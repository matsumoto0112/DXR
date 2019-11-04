#pragma once
#include <Windows.h>
#include <dxgi1_6.h>
#include "Device/ISystemEventNotify.h"

namespace Framework::Window {
    /**
    * @class Window
    * @brief �E�B���h�E�N���X
    */
    class Window {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Window(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance, Device::ISystemEventNotify* notify);
        /**
        * @brief �f�X�g���N�^
        */
        ~Window();
        /**
        * @brief �E�B���h�E��\������
        */
        void show(int nCmdShow);
        /**
        * @brief �E�B���h�E�n���h�����擾����
        */
        inline HWND getHWnd() const { return mHWnd; }
        /**
        * @brief �t���X�N���[�����H
        */
        inline bool isFullScreen()const { return mIsFullScreen; }

        void toggleFullScreenWindow(IDXGISwapChain* swapChain);

        void setWindowZOrderToTopMost(bool setToTopMost) const;
        UINT getWidth() const { return mWindowRect.right - mWindowRect.left; }
        UINT getHeight() const { return mWindowRect.bottom - mWindowRect.top; }
        RECT getWindowRect() const { return mWindowRect; }
    private:
        static constexpr UINT WINDOW_STYLE = WS_OVERLAPPEDWINDOW;
    private:
        HWND mHWnd; //!< �E�B���h�E�n���h��
        bool mIsFullScreen; //!< �t���X�N���[����
        RECT mWindowRect; //!< �E�B���h�E��`
    };

} //Framework::Window
