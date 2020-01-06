#pragma once
#include "DX/Shader/RenderTarget.h"
#include "DX/Resource/ShaderResourceView.h"

namespace Framework::DX {
    enum class RenderTargetState {
        RenderTarget,
        ShaderResource,
    };
    /**
     * @class RenderTargetTexture
     * @brief テクスチャとして利用できるレンダーターゲット
     */
    class RenderTargetTexture {
    public:
        /**
         * @brief コンストラクタ
         */
        RenderTargetTexture() {}
        /**
         * @brief デストラクタ
         */
        ~RenderTargetTexture() {}
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
         * @brief モードを切り替える
         */
        void transition(ID3D12GraphicsCommandList* commandList, RenderTargetState nextMode);
        /**
         * @brief レンダーターゲットのクリア
         */
        void clear(ID3D12GraphicsCommandList* commandList, const Utility::Color4& clearColor);
        /**
         * @brief レンダーターゲットビューを取得する
         */
        const RenderTargetView& getRenderTargetView() const {
            return mRenderTarget.getView();
        }
        /**
         * @brief シェーダーリソースビューを取得する
         */
        const ShaderResourceView& getShaderResourceView() const {
            return mSRV;
        }

    private:
        RenderTarget mRenderTarget;
        ShaderResourceView mSRV;
    };
} // namespace Framework::DX