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
    };
} //Framework::Device
