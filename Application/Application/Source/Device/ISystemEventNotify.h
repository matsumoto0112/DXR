/**
 * @file ISystemEventNotify.h
 * @brief システムイベント処理通知
 */

#pragma once
#include <Windows.h>

namespace Framework::Device {
    /**
     * @brief システムイベント処理数値
     */
    class ISystemEventNotify {
    public:
        /**
         * @brief デストラクタ
         */
        virtual ~ISystemEventNotify() = default;
        /**
         * @brief フレームイベント
         */
        virtual void onFrameEvent() = 0;
        /**
         * @brief フルスクリーンモード切替
         */
        virtual void toggleFullScreenWindow() = 0;
        /**
         * @brief ウィンドウの境界設定
         */
        virtual void setWindowBounds(const RECT& rect) = 0;
        /**
         * @brief ウィンドウサイズの変更
         */
        virtual void onSizeChanged(UINT width, UINT height, bool minimized) = 0;
        /**
         * @brief ウィンドウの移動イベント
         */
        virtual void onWindowMoved(int x, int y) = 0;
    };
} // namespace Framework::Device
