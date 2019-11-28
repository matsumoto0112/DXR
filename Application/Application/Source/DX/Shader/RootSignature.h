/**
 * @file RootSignature.h
 * @brief ���[�g�V�O�l�`���N���X
 */

#pragma once

namespace Framework::DX {
    /**
     * @class RootSignature
     * @brief ���[�g�V�O�l�`��
     */
    class RootSignature {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        RootSignature(ID3D12Device* device, const CD3DX12_ROOT_SIGNATURE_DESC& desc);
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
        ComPtr<ID3D12RootSignature> mRootSignature; //!< ���[�g�V�O�l�`��
    };
} // namespace Framework::DX
