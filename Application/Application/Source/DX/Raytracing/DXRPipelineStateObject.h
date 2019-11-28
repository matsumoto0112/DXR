/**
 * @file DXRPipelineStateObject.h
 * @brief ���C�g���[�V���O�p�p�C�v���C��
 */

#pragma once

namespace Framework::DX {
    class RootSignature;
    /**
     * @class DXRPipelineStateObject
     * @brief ���C�g���[�V���O�p�p�C�v���C��
     */
    class DXRPipelineStateObject {
    public:
        /**
         * @struct HitGroupDesc
         * @brief �q�b�g�O���[�v�f�B�X�N
         */
        struct HitGroupDesc {
            std::wstring name; //!< �q�b�g�O���[�v��
            D3D12_HIT_GROUP_TYPE type; //!< �q�b�g�O���[�v�̎��
            std::wstring closestHitName; //!< closesthit�V�F�[�_�[��
            std::wstring anyHitName; //!< anyhit�V�F�[�_�[��
            std::wstring intersectionName; //!< intersection�V�F�[�_�[��
            /**
             * @brief �R���X�g���N�^
             */
            HitGroupDesc(const std::wstring& name, D3D12_HIT_GROUP_TYPE type,
                const std::wstring& closestHitName = L"", const std::wstring& anyHitName = L"",
                const std::wstring& intersectionName = L"")
                : name(name),
                  type(type),
                  closestHitName(closestHitName),
                  anyHitName(anyHitName),
                  intersectionName(intersectionName) {}
        };

    public:
        /**
         * @brief �R���X�g���N�^
         */
        DXRPipelineStateObject();
        /**
         * @brief �f�X�g���N�^
         */
        ~DXRPipelineStateObject();
        /**
         * @brief �V�F�[�_�[�t�@�C������V�F�[�_�[�����o��
         * @param shaderCode �V�F�[�_�[�R�[�h
         * @param byteLength �V�F�[�_�[�R�[�h�̒���
         * @param names ���o���V�F�[�_�[�̃G���g���[�|�C���g��
         */
        template <class... T>
        void exportShader(void* shaderCode, size_t byteLength, const T&... names);
        /**
         * @brief �q�b�g�O���[�v�����т���
         */
        void bindHitGroup(const HitGroupDesc& desc);
        /**
         * @brief ���C�g���[�V���O�̐ݒ���s��
         */
        void setConfig(UINT payloadSize, UINT attributeSize, UINT maxRecursionDepth);
        /**
         * @brief ���[�J�����[�g�V�O�l�`���̃o�C���h������
         * @param localRootSignature ���[�J�����[�g�V�O�l�`��
         * @param targetShaderName �ΏۂƂȂ�V�F�[�_�[��
         */
        void bindLocalRootSignature(
            const RootSignature& localRootSignature, const std::wstring& targetShaderName);
        /**
         * @brief �O���[�o�����[�g�V�O�l�`���̃o�C���h������
         */
        void bindGlobalRootSignature(const RootSignature& rootSignature);
        /**
         * @brief �p�C�v���C���̍쐬
         */
        void create(ID3D12Device5* device);
        /**
         * @brief �p�C�v���C���̎擾
         */
        ID3D12StateObject* getStateObject() const {
            return mPipelineStateObject.Get();
        }

    private:
        CD3DX12_STATE_OBJECT_DESC mPipelineStateObjectDesc; //!< �p�C�v���C���f�B�X�N
        ComPtr<ID3D12StateObject> mPipelineStateObject; //!< �p�C�v���C���I�u�W�F�N�g
    };

    //�V�F�[�_�[�̓ǂݍ���
    template <class... T>
    inline void DXRPipelineStateObject::exportShader(
        void* shaderCode, size_t byteLength, const T&... names) {
        CD3DX12_DXIL_LIBRARY_SUBOBJECT* lib
            = mPipelineStateObjectDesc.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
        lib->SetDXILLibrary(&CD3DX12_SHADER_BYTECODE(shaderCode, byteLength));

        for (auto&& name : std::initializer_list<std::wstring>{ names... }) {
            lib->DefineExport(name.c_str());
        }
    }
} // namespace Framework::DX
