#pragma once

namespace Framework::Device {
    /**
    * @class ISystemEventNotify
    * @brief システムのイベント通知インターフェース
    */
    class ISystemEventNotify {
    public:
        /**
        * @brief デストラクタ
        */
        virtual ~ISystemEventNotify() = default;
        /**
        * @brief 初期化
        */
        virtual void onInit() = 0;
        /**
        * @brief 更新
        */
        virtual void onUpdate() = 0;
        /**
        * @brief 描画
        */
        virtual void onRender() = 0;
        /**
        * @brief 破棄
        */
        virtual void onDestroy() = 0;
        /**
        * @brief フルスクリーンモードの切り替え
        */
        virtual void toggleFullScreenWindow() = 0;
        virtual void updateForSizeChange(UINT clientWidth, UINT cliendHeight) = 0;
        virtual void setWindowBounds(const RECT& rect) = 0;
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) = 0;
        virtual void onWindowMoved(int x, int y) = 0;
    };
} //Framework::Device
