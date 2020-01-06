/**
 * @file RootSignature.h
 * @brief ルートシグネチャクラス
 */

#pragma once

namespace Framework::DX {
    class DeviceResource;
    struct RootSignatureDesc;
    /**
     * @class RootSignature
     * @brief ルートシグネチャ
     */
    class RootSignature {
    public:
        struct RootSignatureInputIndex {
            UINT constantBuffer;
            UINT shaderResource;
            UINT unorderedAccess;
            UINT sampler;
        };

    public:
        /**
         * @brief ルートシグネチャのフラグ
         */
        enum class Flags {
            None,
            Local,
            Global,
        };

    public:
        /**
         * @brief コンストラクタ
         */
        RootSignature();
        /**
         * @brief デストラクタ
         */
        ~RootSignature();
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, const RootSignatureDesc& desc);
        /**
         * @brief 初期化
         * @param device デバイス
         * @param samplers 使用するサンプラー
         */
        void init(DeviceResource* device, const std::vector<CD3DX12_STATIC_SAMPLER_DESC>& samplers);
        /**
         * @brief コマンドリストにセットする
         */
        void setGraphicsCommandList(ID3D12GraphicsCommandList* commandList);

        /**
         * @brief ルートシグネチャの取得
         */
        ID3D12RootSignature* getRootSignature() const {
            return mRootSignature.Get();
        }
        /**
         * @brief ルートシグネチャの入力インデックスを取得する
         */
        const RootSignatureInputIndex& getInputIndex() const {
            return mInputIndex;
        }

    private:
        Comptr<ID3D12RootSignature> mRootSignature; //!< ルートシグネチャ
        RootSignatureInputIndex mInputIndex;
    };

    /**
     * @brief ルートシグネチャディスク
     */
    struct RootSignatureDesc {
        std::vector<CD3DX12_ROOT_PARAMETER> params;
        std::vector<CD3DX12_STATIC_SAMPLER_DESC> samplers;
        RootSignature::Flags flag;
    };

} // namespace Framework::DX
