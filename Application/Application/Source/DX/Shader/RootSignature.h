/**
 * @file RootSignature.h
 * @brief ���[�g�V�O�l�`���N���X
 */

#pragma once

namespace Framework::DX {
    class DeviceResource;
    struct RootSignatureDesc;
    /**
     * @class RootSignature
     * @brief ���[�g�V�O�l�`��
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
        RootSignature();
        /**
         * @brief �f�X�g���N�^
         */
        ~RootSignature();
        /**
         * @brief ������
         */
        void init(DeviceResource* device, const RootSignatureDesc& desc);
        /**
         * @brief ������
         * @param device �f�o�C�X
         * @param samplers �g�p����T���v���[
         */
        void init(DeviceResource* device, const std::vector<CD3DX12_STATIC_SAMPLER_DESC>& samplers);
        /**
         * @brief �R�}���h���X�g�ɃZ�b�g����
         */
        void setGraphicsCommandList(ID3D12GraphicsCommandList* commandList);

        /**
         * @brief ���[�g�V�O�l�`���̎擾
         */
        ID3D12RootSignature* getRootSignature() const {
            return mRootSignature.Get();
        }
        /**
         * @brief ���[�g�V�O�l�`���̓��̓C���f�b�N�X���擾����
         */
        const RootSignatureInputIndex& getInputIndex() const {
            return mInputIndex;
        }

    private:
        Comptr<ID3D12RootSignature> mRootSignature; //!< ���[�g�V�O�l�`��
        RootSignatureInputIndex mInputIndex;
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
