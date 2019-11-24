#pragma once
#include <memory>
#include <Windows.h>
#include <string>
#include "Device/ISystemEventNotify.h"
#include "DX/DeviceResource.h"
#include "Input/InputManager.h"
#include "Window/Window.h"
#include "ImGui/ImGuiManager.h"

namespace Framework {
    /**
    * @class Game
    * @brief discription
    */
    class Game : public Device::ISystemEventNotify {
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
        virtual void onInit();
        /**
        * @brief �X�V
        */
        virtual void onUpdate();
        /**
        * @brief �`��
        */
        virtual void onRender();
        /**
        * @brief �j����
        */
        virtual void onDestroy();

        virtual void onFrameEvent() override;
        /**
        * @brief �t���X�N���[���؂�ւ�
        */
        virtual void toggleFullScreenWindow() override { }
        /**
        * @brief �E�B���h�E�T�C�Y���X�V����
        */
        virtual void updateForSizeChange(UINT clientWidth, UINT clientHeight) override {
            mWidth = clientWidth;
            mHeight = clientHeight;
        }
        /**
        * @brief �E�B���h�E�̑傫�����Z�b�g����
        */
        virtual void setWindowBounds(const RECT&) override { }
        /**
        * @brief �E�B���h�E�T�C�Y�̐؂�ւ��C�x���g
        */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override {
            mWidth = width;
            mHeight = height;
        }
        /**
        * @brief �E�B���h�E�̈ړ�
        */
        virtual void onWindowMoved(int x, int y) override { }
    protected:
        UINT mWidth; //!< �E�B���h�E��
        UINT mHeight; //!< �E�B���h�E����
        std::wstring mTitle; //!< �E�B���h�E�^�C�g��
    };
} //Framework
