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
         * @brief
         */
        enum Flags {
            None,
            Global,
            Local,
        };

    public:
        /**
         * @brief
         */
        RootSignature();
        /**
         * @brief
         */
        ~RootSignature();
        void create(ID3D12Device* device, const CD3DX12_ROOT_SIGNATURE_DESC& desc);
        ID3D12RootSignature* getRootSignature() const {
            return mRootSignature.Get();
        }

    private:
        ComPtr<ID3D12RootSignature> mRootSignature;
    };
} // namespace Framework::DX
