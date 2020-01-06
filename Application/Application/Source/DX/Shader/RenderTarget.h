#pragma once
#include "DX/Shader/RenderTargetView.h"
#include "Utility/Color4.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RenderTarget
     * @brief レンダーターゲット
     */
    class RenderTarget {
    public:
        /**
         * @brief コンストラクタ
         */
        RenderTarget() {}
        /**
         * @brief デストラクタ
         */
        ~RenderTarget() {}
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, UINT width, UINT height, DXGI_FORMAT format,
            const Utility::Color4& clearColor, const std::wstring& name);
        /**
         * @brief 状態を遷移させる
         */
        void transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES nextState);
        /**
         * @brief レンダーターゲットのクリア
         */
        void clear(ID3D12GraphicsCommandList* commandList, const Utility::Color4& color);
        /**
         * @brief リソースから初期化をする
         */
        void initAsResource(DeviceResource* device, Comptr<ID3D12Resource> resource);
        /**
         * @brief バッファを取得する
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief ビューの取得
         */
        const RenderTargetView& getView() const {
            return mView;
        }

    private:
        Buffer mBuffer;
        RenderTargetView mView;
    };
} // namespace Framework::DX