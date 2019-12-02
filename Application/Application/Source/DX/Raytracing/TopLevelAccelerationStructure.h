/**
 * @file TopLevelAccelerationStructure.h
 * @brief �g�b�v���x����AS
 * @details �W�I���g���̃V�[������`
 */

#pragma once
#include <DirectXMath.h>
#include "DX/Raytracing/BottomLevelAccelerationStructure.h"
#include "DX/Raytracing/DXRDevice.h"

namespace Framework::DX {
    /**
     * @class TopLevelAccelerationStructure
     * @brief �g�b�v���x��AS
     */
    class TopLevelAccelerationStructure {
    public:
        /**
         * @brief TLAS�̃C���X�^���X�f�B�X�N
         */
        struct InstanceDesc {
            UINT instanceID; //!< ID
            UINT mask; //!< �}�X�N
            UINT flags; //!< �W�I���g���̃t���O
            UINT hitGroupIndex; //!< �q�b�g�O���[�v�̌v�Z�Ɏg�p����C���f�b�N�X
            BottomLevelAccelerationStructure* blas; //!< �ΏۂƂȂ�W�I���g��
            DirectX::XMMATRIX transform; //!< �W�I���g���̃g�����X�t�H�[��
            /**
             * @brief �R���X�g���N�^
             */
            InstanceDesc()
                : instanceID(0), mask(0), flags(0), hitGroupIndex(0), blas(nullptr), transform() {}
        };

    public:
        /**
         * @brief �R���X�g���N�^
         */
        TopLevelAccelerationStructure();
        /**
         * @brief �f�X�g���N�^
         */
        ~TopLevelAccelerationStructure();
        /**
         * @brief �C���X�^���X�f�B�X�N��ǉ�����
         */
        void add(const InstanceDesc& desc);
        /**
         * @brief�@TLAS���\�z����
         * @param device �f�o�C�X
         * @param buildFlag TLAS�̍\�z�t���O
         */
        void build(
            const DXRDevice& device, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag);
        /**
         * @brief �f�B�X�N���N���A����
         */
        void clear();
        /**
         * @brief �o�b�t�@���擾����
         */
        ID3D12Resource* getBuffer() const {
            return mBuffer.Get();
        }

    private:
        /**
         * @brief �\�z�ɕK�v�Ȏ��O�����\�z����
         * @param device �f�o�C�X
         * @param buildFlag TLAS�̍\�z�t���O
         */
        void buildPrebuildInfo(
            const DXRDevice& device, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag);

    private:
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC mDesc; //!< �f�B�X�N
        std::vector<D3D12_RAYTRACING_INSTANCE_DESC> mInstanceDescs; //!< �V�[�����f�B�X�N
        ComPtr<ID3D12Resource> mScratch; //!< �X�N���b�`���\�[�X
        ComPtr<ID3D12Resource> mInstance; //!< �ꎞ�I�ȃ��\�[�X
        ComPtr<ID3D12Resource> mBuffer; //!< TLAS�o�b�t�@
    };
} // namespace Framework::DX
