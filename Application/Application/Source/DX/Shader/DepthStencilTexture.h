#pragma once
#include "DX/Shader/DepthStencil.h"
#include "DX/Resource/ShaderResourceView.h"

namespace Framework::DX {
    /**
     * @brief バッファの状態定義
     */
    enum class DepthStencilState {
        DepthStencil,
        ShaderResource,
    };
    /**
     * @class DepthStencilTexture
     * @brief テクスチャとして利用できるデプス・ステンシルバッファ
     */
    class DepthStencilTexture {
    public:
    public:
        /**
         * @brief コンストラクタ
         */
        DepthStencilTexture() {}
        /**
         * @brief デストラクタ
         */
        ~DepthStencilTexture() {}
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
         * @brief 状態を遷移させる
         */
        void transition(ID3D12GraphicsCommandList* commandList, DepthStencilState nextMode);
        /**
         * @brief デプス・ステンシルのクリア
         */
        void clear(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief デプス・ステンシルビューの取得
         */
        const DepthStencilView& getDepthStencilView() const {
            return mDepthStencil.getView();
        }
        /**
         * @brief シェーダーリソースビューの取得
         */
        const ShaderResourceView& getShaderResourceView() const {
            return mSRV;
        }

    private:
        DepthStencil mDepthStencil;
        ShaderResourceView mSRV;
    };
} // namespace Framework::DX