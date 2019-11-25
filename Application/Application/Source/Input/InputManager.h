/**
 * @file InputManager.h
 * @brief �Q�[���ɂ�������͑S�ʂ��Ǘ�����
 */

#pragma once
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace Framework::Input {
    /**
     * @class InputManager
     * @brief ���͂��Ǘ�����
     */
    class InputManager {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param hWnd �E�B���h�E�n���h��
         */
        InputManager(HWND hWnd);
        /**
         * @brief �f�X�g���N�^
         */
        ~InputManager();
        /**
         * @brief ���͏����X�V����
         */
        void update();
        /**
         * @brief �L�[�{�[�h���擾����
         */
        inline Keyboard* getKeyboard() const { return mKeyboard.get(); }
        /**
         * @brief �}�E�X���擾����
         */
        inline Mouse* getMouse() const { return mMouse.get(); }

    private:
        UniquePtr<Keyboard> mKeyboard; //!< �L�[�{�[�h
        UniquePtr<Mouse> mMouse; //!< �}�E�X
    };
} // namespace Framework::Input
