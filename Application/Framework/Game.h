#pragma once
#include <memory>
#include <Windows.h>
#include <string>
#include "Device/ISystemEventNotify.h"
#include "DX/DeviceResource.h"
#include "Input/InputManager.h"
#include "Window/Window.h"

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
        /**
        * @brief �A�v���P�[�V�����̎��s
        */
        int run(HINSTANCE hInstance, int nCmdShow);
        /**
        * @brief ������
        */
        virtual void onInit() override;
        /**
        * @brief �X�V
        */
        virtual void onUpdate() override;
        /**
        * @brief �`��J�n
        */
        void renderStart();
        /**
        * @brief �`��I��
        */
        void renderEnd();
        /**
        * @brief �j����
        */
        virtual void onDestroy() override;
        /**
        * @brief �t���X�N���[���؂�ւ�
        */
        virtual void toggleFullScreenWindow() override;
        /**
        * @brief �E�B���h�E�T�C�Y���X�V����
        */
        virtual void updateForSizeChange(UINT clientWidth, UINT cliendHeight) override;
        /**
        * @brief �E�B���h�E�̑傫�����Z�b�g����
        */
        virtual void setWindowBounds(const RECT & rect) override;
        /**
        * @brief �E�B���h�E�T�C�Y�̐؂�ւ��C�x���g
        */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override;
        /**
        * @brief �E�B���h�E�̈ړ�
        */
        virtual void onWindowMoved(int x, int y) override;
    protected:
        UINT mWidth; //!< �E�B���h�E��
        UINT mHeight; //!< �E�B���h�E����
        float mAspectRatio; //!< �A�X�y�N�g��
        RECT mWindowBounds; //!< �E�B���h�E�̋�`
        std::wstring mTitle; //!< �E�B���h�E�^�C�g��
        std::unique_ptr<Window::Window> mWindow; //!< �E�B���h�E
        std::unique_ptr<DX::DeviceResource> mDeviceResource; //!< �f�o�C�X���\�[�X
        std::unique_ptr<Input::InputManager> mInputManager; //!< ���͊Ǘ�
    };
} //Framework
