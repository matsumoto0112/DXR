/**
 * @file ShaderRecord.h
 * @brief �V�F�[�_�[���R�[�h�Ǘ�
 */

#pragma once

namespace Framework::DX {
    /**
     * @brief �V�F�[�_�[���R�[�h�N���X
     */
    class ShaderRecord {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param id �V�F�[�_�[�̐擪ID
         * @param size �V�F�[�_�[�̑傫��
         */
        ShaderRecord(void* id, UINT size) : shaderID(id, size) {}
        /**
         * @brief �R���X�g���N�^
         * @param id �V�F�[�_�[�̐擪ID
         * @param size �V�F�[�_�[�̑傫��
         * @param localRootArgument ���[�J���A�[�M�������g
         * @param localRootArgumentSize ���[�J���A�[�M�������g�̑傫��
         */
        ShaderRecord(void* id, UINT size, void* localRootArgument, UINT localRootArgumentSize)
            : shaderID(id, size), localRootArgument(localRootArgument, localRootArgumentSize) {}
        /**
         * @brief �f�[�^���R�s�[����
         * @param dest �R�s�[��
         */
        void copyTo(void* dest) const {
            uint8_t* byteDest = static_cast<uint8_t*>(dest);
            memcpy(byteDest, shaderID.ptr, shaderID.size);
            if (localRootArgument.ptr) {
                memcpy(byteDest + shaderID.size, localRootArgument.ptr, localRootArgument.size);
            }
        }

    private:
        /**
         * @struct PointerWithSize
         * @brief �f�[�^�̃|�C���^�Ƃ��̑傫���̃y�A
         */
        struct PointerWithSize {
            void* ptr; //!< �|�C���^
            UINT size; //!< �傫��
            PointerWithSize() : ptr(nullptr), size(0) {}
            PointerWithSize(void* ptr, UINT size) : ptr(ptr), size(size) {}
        };
        PointerWithSize shaderID; //!< �V�F�[�_�[ID
        PointerWithSize localRootArgument; //!< ���[�J���A�[�M�������g
    };
} // namespace Framework::DX
