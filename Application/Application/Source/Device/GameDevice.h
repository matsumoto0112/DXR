/**
 * @file GameDevice.h
 * @brief �Q�[���f�o�C�X�Ǘ�
 */

#pragma once
#include <d3d12.h>
#include "DX/DeviceResource.h"
#include "Device/ISystemEventNotify.h"
#include "ImGui/ImGuiManager.h"
#include "Input/InputManager.h"
#include "Utility/Singleton.h"
#include "Window/Window.h"

namespace Framework::Device {
    /**
     * @class GameDevice
     * @brief �Q�[���f�o�C�X�Ǘ�
     */
    class GameDevice : public Utility::Singleton<GameDevice>, public ISystemEventNotify {
    public:
        /**
         * @brief ����������
         * @param width �E�B���h�E��
         * @param height �E�B���h�E����
         * @param title �E�B���h�E�^�C�g��
         * @param hInstance �C���X�^���g�n���h��
         * @param rtvFormat �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
         * @param dsvFormat �f�v�X�E�X�e���V���t�H�[�}�b�g
         * @param notify �V�X�e���C�x���g�̒ʒm��
         * @param flags �f�o�C�X�t���O
         */
        void init(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance,
            DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, ISystemEventNotify* notify, UINT flags);
        /**
         * @brief �A�v���P�[�V�����̎��s
         * @param nCmdShow �\���X�^�C��
         */
        void run(int nCmdShow);
        /**
         * @brief �I������
         */
        void finalize();
        /**
         * @brief �E�B���h�E�̎擾
         */
        Window::Window* getWindow() const { return mWindow.get(); }
        /**
         * @brief �f�o�C�X���\�[�X�̎擾
         */
        DX::DeviceResource* getDeviceResource() const { return mDeviceResource.get(); }
        /**
         * @brief ���̓}�l�[�W���̎擾
         */
        Input::InputManager* getInputManager() const { return mInputManager.get(); }
        /**
         * @brief �t���[���C�x���g����
         */
        virtual void onFrameEvent() override;
        /**
         * @brief �t���X�N���[�����[�h�̐؂�ւ�
         */
        virtual void toggleFullScreenWindow() override;
        /**
         * @brief �E�B���h�E�̋��E��ݒ肷��
         */
        virtual void setWindowBounds(const RECT& rect) override;
        /**
         * @brief �E�B���h�E�T�C�Y�̕ύX�C�x���g
         */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override;
        /**
         * @brief �E�B���h�E�̈ړ��C�x���g
         */
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
        UniquePtr<Input::InputManager> mInputManager; //!< ���̓}�l�[�W��
        UniquePtr<ImGuiManager> mImGuiManager; //!< ImGui�Ǘ�
        ISystemEventNotify* mNotify; //!< �V�X�e���C�x���g�̒ʒm��
    private:
        /**
         * @brief �t���[���J�n������
         */
        void beginFrame();
        /**
         * @brief �t���[���I��������
         */
        void endFrame();
    };
} // namespace Framework::Device
