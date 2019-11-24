/**
 * @file GameDevice.h
 * @brief ゲームデバイス管理
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
     * @brief ゲームデバイス管理
     */
    class GameDevice : public Utility::Singleton<GameDevice>, public ISystemEventNotify {
    public:
        /**
         * @brief 初期化処理
         * @param width ウィンドウ幅
         * @param height ウィンドウ高さ
         * @param title ウィンドウタイトル
         * @param hInstance インスタントハンドル
         * @param rtvFormat レンダーターゲットのフォーマット
         * @param dsvFormat デプス・ステンシルフォーマット
         * @param notify システムイベントの通知先
         * @param flags デバイスフラグ
         */
        void init(UINT width, UINT height, const std::wstring& title, HINSTANCE hInstance,
            DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, ISystemEventNotify* notify, UINT flags);
        /**
         * @brief アプリケーションの実行
         * @param nCmdShow 表示スタイル
         */
        void run(int nCmdShow);
        /**
         * @brief 終了処理
         */
        void finalize();
        /**
         * @brief ウィンドウの取得
         */
        Window::Window* getWindow() const { return mWindow.get(); }
        /**
         * @brief デバイスリソースの取得
         */
        DX::DeviceResource* getDeviceResource() const { return mDeviceResource.get(); }
        /**
         * @brief 入力マネージャの取得
         */
        Input::InputManager* getInputManager() const { return mInputManager.get(); }
        /**
         * @brief フレームイベント処理
         */
        virtual void onFrameEvent() override;
        /**
         * @brief フルスクリーンモードの切り替え
         */
        virtual void toggleFullScreenWindow() override;
        /**
         * @brief ウィンドウの境界を設定する
         */
        virtual void setWindowBounds(const RECT& rect) override;
        /**
         * @brief ウィンドウサイズの変更イベント
         */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) override;
        /**
         * @brief ウィンドウの移動イベント
         */
        virtual void onWindowMoved(int x, int y) override;

    protected:
        /**
         * @brief コンストラクタ
         */
        GameDevice();
        /**
         * @brief デストラクタ
         */
        ~GameDevice();

    private:
        UniquePtr<Window::Window> mWindow; //!< ウィンドウ
        UniquePtr<DX::DeviceResource> mDeviceResource; //!< デバイスリソース
        UniquePtr<Input::InputManager> mInputManager; //!< 入力マネージャ
        UniquePtr<ImGuiManager> mImGuiManager; //!< ImGui管理
        ISystemEventNotify* mNotify; //!< システムイベントの通知先
    private:
        /**
         * @brief フレーム開始時処理
         */
        void beginFrame();
        /**
         * @brief フレーム終了時処理
         */
        void endFrame();
    };
} // namespace Framework::Device
