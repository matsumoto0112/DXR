/**
 * @file RootSignature.h
 * @brief
 */

#pragma once

namespace Framework::DX {
    /**
     * @class RootSignature
     * @brief ルートシグネチャ
     */
    class RootSignature {
    public:
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
        void create(ID3D12Device* device, Flags flag,
            const std::vector<CD3DX12_ROOT_PARAMETER>& parameters,
            const std::vector<CD3DX12_STATIC_SAMPLER_DESC>& samplers = {});
        ID3D12RootSignature* getRootSignature() const { return mRootSignature.Get(); }

    private:
        ComPtr<ID3D12RootSignature> mRootSignature;
    };
} // namespace Framework::DX
