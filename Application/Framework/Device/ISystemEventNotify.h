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
        virtual void onInit() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onDestroy() = 0;
    };
} //Framework::Device
