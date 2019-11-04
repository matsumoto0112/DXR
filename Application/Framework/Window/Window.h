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
        /**
        * @brief �t���X�N���[�����Ɣ�t���X�N���[����؂�ւ���
        * @param swapChain �X���b�v�`�F�C��
        */
        void toggleFullScreenWindow(IDXGISwapChain* swapChain);
        /**
        * @brief �E�B���h�E��Z�I�[�_�[�𑀍삷��
        * @param setToTopMost �őO�ʂɏo����
        */
        void setWindowZOrderToTopMost(bool setToTopMost) const;
        /**
        * @brief �E�B���h�E�̕����擾����
        */
        UINT getWidth() const { return mWindowRect.right - mWindowRect.left; }
        /**
        * @brief �E�B���h�E�̍������擾����
        */
        UINT getHeight() const { return mWindowRect.bottom - mWindowRect.top; }
        /**
        * @brief �E�B���h�E��`���擾����
        */
        RECT getWindowRect() const { return mWindowRect; }
    private:
        static constexpr UINT WINDOW_STYLE = WS_OVERLAPPEDWINDOW; //!< �E�B���h�E�̃X�^�C��
    private:
        HWND mHWnd; //!< �E�B���h�E�n���h��
        bool mIsFullScreen; //!< �t���X�N���[����
        RECT mWindowRect; //!< �E�B���h�E��`
    };

} //Framework::Window
