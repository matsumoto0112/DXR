/**
 * @file RootSignature.h
 * @brief ルートシグネチャクラス
 */

#pragma once

namespace Framework::DX {
    /**
     * @class RootSignature
     * @brief ルートシグネチャ
     */
    class RootSignature {
    public:
        /**
         * @brief コンストラクタ
         */
        RootSignature(ID3D12Device* device, const CD3DX12_ROOT_SIGNATURE_DESC& desc);
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
        ComPtr<ID3D12RootSignature> mRootSignature; //!< ルートシグネチャ
    };
} // namespace Framework::DX
