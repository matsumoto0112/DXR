#pragma once
#include "DX/Shader/DepthStencilView.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class DepthStencil
     * @brief デプス・ステンシル
     */
    class DepthStencil {
    public:
        /**
         * @brief コンストラクタ
         */
        DepthStencil() {}
        /**
         * @brief デストラクタ
         */
        ~DepthStencil() {}
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, UINT width, UINT height, DXGI_FORMAT format,
            const std::wstring& name);
        /**
         * @brief 状態を遷移させる
         */
        void transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState);
        /**
         * @brief ビューのクリア処理
         */
        void clear(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief バッファを取得する
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief ビューの取得
         */
        const DepthStencilView& getView() const {
            return mDSV;
        }

    private:
        Buffer mBuffer;
        DepthStencilView mDSV;
    };
} // namespace Framework::DX
