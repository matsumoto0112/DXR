#pragma once
#include <Windows.h>

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
        virtual void onFrameEvent() = 0;
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
