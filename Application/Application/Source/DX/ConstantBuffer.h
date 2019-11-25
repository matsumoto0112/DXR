/**
 * @file ConstantBuffer.h
 * @brief �R���X�^���g�o�b�t�@
 */

#pragma once
#include "DX/Util/GPUUploadBuffer.h"
#include "Utility/Debug.h"

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
         */
        ConstantBuffer();
        /**
         * @brief �f�X�g���N�^
         */
        ~ConstantBuffer();
        /**
         * @brief �o�b�t�@���쐬����
         * @param device �f�o�C�X
         * @param numInstances ���C���X�^���X��
         * @param name ���\�[�X��
         */
        void create(ID3D12Device* device, UINT numInstances, LPCWSTR name);
        /**
         * @brief �X�e�[�W���O�̓��e��GPU�ɃR�s�[����
         * @param instanceIndex �R�s�[����C���f�b�N�X
         */
        void copyStatingToGPU(UINT instanceIndex = 0);
        /**
         * @brief �C���X�^���X�����擾����
         */
        UINT numInstances() const { return mNumInstances; }
        /**
         * @brief GPU�A�h���X���擾����
         * @param instanceIndex �擾����C���f�b�N�X
         */
        D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress(UINT instanceIndex = 0);
        /**
         * @brief �A���[���Z�q
         * @details �X�e�[�W���O�̓��e�ɃA�N�Z�X���₷�����邽��
         */
        T* operator->() { return &mStaging; }

    private:
        uint8_t* mMappedConstantData; //!< �������̏������ݔ͈�
        UINT mAlignedInstanceSize; //!< �A���C�������g���ꂽ�o�b�t�@�T�C�Y
        UINT mNumInstances; //!< �C���X�^���X��
        T mStaging; //!< �R���X�^���g�o�b�t�@�̒l
    };

    //�R���X�g���N�^
    template <class T>
    inline ConstantBuffer<T>::ConstantBuffer()
        : mMappedConstantData(nullptr), mAlignedInstanceSize(0), mNumInstances(0) {}
    //�f�X�g���N�^
    template <class T>
    inline ConstantBuffer<T>::~ConstantBuffer() {}
    //�o�b�t�@�̍쐬
    template <class T>
    inline void ConstantBuffer<T>::create(ID3D12Device* device, UINT numInstances, LPCWSTR name) {
        mNumInstances = numInstances;
        mAlignedInstanceSize
            = Math::MathUtil::alignPow2(sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
        UINT bufferSize = mNumInstances * mAlignedInstanceSize;
        allocate(device, bufferSize, name);
        mMappedConstantData = getMapCPUWriteOnly();
    }
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
