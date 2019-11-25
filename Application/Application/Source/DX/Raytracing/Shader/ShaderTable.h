/**
 * @file ShaderTable.h
 * @brief �V�F�[�_�[�e�[�u��
 */

#pragma once
#include "DX/Raytracing/Shader/ShaderRecord.h"
#include "DX/Util/GPUUploadBuffer.h"

namespace Framework::DX {
    /**
     * @brief �V�F�[�_�[�e�[�u���N���X
     */
    class ShaderTable : public GPUUploadBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param device �f�o�C�X
         * @param shaderRecordNum �����N����V�F�[�_�[��
         * @param shaderRecordSize ���R�[�h��̑傫��
         * @param name �V�F�[�_�[��
         */
        ShaderTable(
            ID3D12Device* device, UINT shaderRecordNum, UINT shaderRecordSize, LPCWSTR name) {
            mRecordSize = Math::MathUtil::alignPow2(
                shaderRecordSize, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);
            mShaderRecords.reserve(shaderRecordNum);
            UINT bufferSize = shaderRecordNum * mRecordSize;
            allocate(device, bufferSize, name);
            mMapped = getMapCPUWriteOnly();
        }
        /**
         * @brief ���R�[�h��ǉ�����
         */
        void push_back(const ShaderRecord& record) {
            MY_THROW_IF_FALSE(mShaderRecords.size() < mShaderRecords.capacity());
            mShaderRecords.emplace_back(record);
            record.copyTo(mMapped);
            mMapped += mRecordSize;
        }
        /**
         * @brief �V�F�[�_�[���R�[�h�̑傫�����擾����
         */
        UINT getShaderRecordSize() const { return mRecordSize; }

    private:
        uint8_t* mMapped; //!< �������ݗ̈�
        UINT mRecordSize; //!< ���R�[�h�̑傫��
        std::vector<ShaderRecord> mShaderRecords; //!< �V�F�[�_�[���R�[�h
    };
} // namespace Framework::DX
