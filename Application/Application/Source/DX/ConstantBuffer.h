/**
 * @file ConstantBuffer.h
 * @brief �R���X�^���g�o�b�t�@
 */

#pragma once
#include "DX/Util/GPUUploadBuffer.h"

namespace Framework::DX {
    /**
     * @class ConstantBuffer
     * @tparam T �g�p����\����
     * @brief �R���X�^���g�o�b�t�@
     */
    template <class T>
    class ConstantBuffer : public GPUUploadBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param device �f�o�C�X
         * @param numInstances ���C���X�^���X��
         * @param name ���\�[�X��
         */
        ConstantBuffer(ID3D12Device* device, UINT numInstances, const std::wstring& name);
        /**
         * @brief �f�X�g���N�^
         */
        ~ConstantBuffer();
        /**
         * @brief �X�e�[�W���O�̓��e��GPU�ɃR�s�[����
         * @param instanceIndex �R�s�[����C���f�b�N�X
         */
        void copyStatingToGPU(UINT instanceIndex = 0);
        /**
         * @brief �C���X�^���X�����擾����
         */
        UINT numInstances() const {
            return mNumInstances;
        }
        /**
         * @brief GPU�A�h���X���擾����
         * @param instanceIndex �擾����C���f�b�N�X
         */
        D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress(UINT instanceIndex = 0);
        /**
         * @brief �X�e�[�W���O�̓��e�ɃA�N�Z�X����
         */
        inline T& staging() {
            return mStaging;
        }

    private:
        uint8_t* mMappedConstantData; //!< �������̏������ݔ͈�
        UINT mAlignedInstanceSize; //!< �A���C�������g���ꂽ�o�b�t�@�T�C�Y
        UINT mNumInstances; //!< �C���X�^���X��
        T mStaging; //!< �R���X�^���g�o�b�t�@�̒l
    };

    //�R���X�g���N�^
    template <class T>
    inline ConstantBuffer<T>::ConstantBuffer(
        ID3D12Device* device, UINT numInstances, const std::wstring& name)
        : mMappedConstantData(nullptr),
          mAlignedInstanceSize(
              Math::MathUtil::alignPow2(sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)),
          mNumInstances(numInstances) {
        UINT bufferSize = mNumInstances * mAlignedInstanceSize;
        allocate(device, bufferSize, name.c_str());
        mMappedConstantData = getMapCPUWriteOnly();
    }
    //�f�X�g���N�^
    template <class T>
    inline ConstantBuffer<T>::~ConstantBuffer() {}
    //GPU�Ƀf�[�^���R�s�[����
    template <class T>
    inline void ConstantBuffer<T>::copyStatingToGPU(UINT instanceIndex) {
        MY_ASSERTION(instanceIndex < mNumInstances, "�C���f�b�N�X���͈͊O�ł�");
        memcpy(mMappedConstantData + instanceIndex * mAlignedInstanceSize, &mStaging, sizeof(T));
    }
    //GPU�A�h���X�̎擾
    template <class T>
    inline D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer<T>::gpuVirtualAddress(UINT instanceIndex) {
        MY_ASSERTION(instanceIndex < mNumInstances, "�C���f�b�N�X���͈͊O�ł�");
        return mResource->GetGPUVirtualAddress() + instanceIndex * mAlignedInstanceSize;
    }

} // namespace Framework::DX
