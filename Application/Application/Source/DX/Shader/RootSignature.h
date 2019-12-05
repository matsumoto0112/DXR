/**
 * @file RootSignature.h
 * @brief ���[�g�V�O�l�`���N���X
 */

#pragma once

namespace Framework::DX {
    struct RootSignatureDesc;
    /**
     * @class RootSignature
     * @brief ���[�g�V�O�l�`��
     */
    class RootSignature {
    public:
        /**
         * @brief ���[�g�V�O�l�`���̃t���O
         */
        enum class Flags {
            None,
            Local,
            Global,
        };

    public:
        /**
         * @brief �R���X�g���N�^
         */
        RootSignature(ID3D12Device* device, const RootSignatureDesc& desc);
        /**
         * @brief �f�X�g���N�^
         */
        ~RootSignature();
        /**
         * @brief ���[�g�V�O�l�`���̎擾
         */
        ID3D12RootSignature* getRootSignature() const {
            return mRootSignature.Get();
        }

    private:
        Comptr<ID3D12RootSignature> mRootSignature; //!< ���[�g�V�O�l�`��
    };

    /**
     * @brief ���[�g�V�O�l�`���f�B�X�N
     */
    struct RootSignatureDesc {
        std::vector<CD3DX12_ROOT_PARAMETER> params;
        std::vector<CD3DX12_STATIC_SAMPLER_DESC> samplers;
        RootSignature::Flags flag;
    };

} // namespace Framework::DX
