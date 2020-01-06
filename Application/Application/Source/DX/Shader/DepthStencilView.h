#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class DepthStencilView
     * @brief デプス・ステンシルビュー
     */
    class DepthStencilView {
    public:
        /**
         * @brief コンストラクタ
         */
        DepthStencilView() {}
        /**
         * @brief デストラクタ
         */
        ~DepthStencilView() {}
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, const Buffer& buffer, DXGI_FORMAT format);
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