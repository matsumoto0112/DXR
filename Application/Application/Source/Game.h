#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include "DX/DeviceResource.h"
#include "Device/ISystemEventNotify.h"
#include "ImGui/ImGuiManager.h"
#include "Input/InputManager.h"
#include "Window/Window.h"

namespace Framework {
    /**
     * @brief
     */
    class Game : public Device::ISystemEventNotify {
    public:
        /**
         * @brief コンストラクタ
         */
        Game(UINT width, UINT height, const std::wstring& title);
        /**
         * @brief デストラクタ
         */
        virtual ~Game();
        /**
         * @brief アプリケーションの実行
         */
        int run(HINSTANCE hInstance, int nCmdShow);
        /**
         * @brief 初期化
         */
        virtual void onInit();
        /**
         * @brief 更新
         */
        virtual void onUpdate();
        /**
         * @brief 描画
         */
        virtual void onRender();
        /**
         * @brief 破棄時
         */
        virtual void onDestroy();

        virtual void onFrameEvent() override;
        /**
         * @brief フルスクリーン切り替え
         */
        virtual void toggleFullScreenWindow() override {}
        /**
         * @brief ウィンドウの大きさをセットする
         */
        virtual void setWindowBounds(const RECT&) override {}
        /**
         * @brief ウィンドウサイズの切り替えイベント
         */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override {
            mWidth = width;
            mHeight = height;
        }
        /**
         * @brief ウィンドウの移動
         */
        virtual void onWindowMoved(int x, int y) override {}

    protected:
        UINT mWidth; //!< ウィンドウ幅
        UINT mHeight; //!< ウィンドウ高さ
        std::wstring mTitle; //!< ウィンドウタイトル
    };
} // namespace Framework
