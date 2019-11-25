/**
 * @file InputManager.h
 * @brief ゲームにおける入力全般を管理する
 */

#pragma once
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace Framework::Input {
    /**
     * @class InputManager
     * @brief 入力を管理する
     */
    class InputManager {
    public:
        /**
         * @brief コンストラクタ
         * @param hWnd ウィンドウハンドル
         */
        InputManager(HWND hWnd);
        /**
         * @brief デストラクタ
         */
        ~InputManager();
        /**
         * @brief 入力情報を更新する
         */
        void update();
        /**
         * @brief キーボードを取得する
         */
        inline Keyboard* getKeyboard() const { return mKeyboard.get(); }
        /**
         * @brief マウスを取得する
         */
        inline Mouse* getMouse() const { return mMouse.get(); }

    private:
        UniquePtr<Keyboard> mKeyboard; //!< キーボード
        UniquePtr<Mouse> mMouse; //!< マウス
    };
} // namespace Framework::Input
