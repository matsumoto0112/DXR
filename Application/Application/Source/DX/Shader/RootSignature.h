/**
 * @file RootSignature.h
 * @brief ルートシグネチャクラス
 */

#pragma once

namespace Framework::DX {
    struct RootSignatureDesc;
    /**
     * @class RootSignature
     * @brief ルートシグネチャ
     */
    class RootSignature {
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
        RootSignature(ID3D12Device* device, const RootSignatureDesc& desc);
        /**
         * @brief デストラクタ
         */
        ~RootSignature();
        /**
         * @brief ルートシグネチャの取得
         */
        ID3D12RootSignature* getRootSignature() const {
            return mRootSignature.Get();
        }

    private:
        Comptr<ID3D12RootSignature> mRootSignature; //!< ルートシグネチャ
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
