#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RenderTargetView
     * @brief レンダーターゲットビュー
     */
    class RenderTargetView {
    public:
        /**
         * @brief コンストラクタ
         */
        RenderTargetView() {}
        /**
         * @brief デストラクタ
         */
        ~RenderTargetView() {}
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, const Buffer& buffer);
        /**
         * @brief ディスクリプタ情報の取得
         */
        const DescriptorInfo& getInfo() const {
            return mInfo;
        }

    private:
        DescriptorInfo mInfo;
    };
} // namespace Framework::DX