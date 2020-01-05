#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/ConstantBufferView.h"

namespace Framework::DX {
    /**
     * @class ConstantBuffer
     * @brief �R���X�^���g�o�b�t�@�Ǘ��N���X
     */
    template <class T>
    class ConstantBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        ConstantBuffer() {}
        /**
         * @brief �R�s�[�R���X�g���N�^
         */
        ConstantBuffer(const ConstantBuffer& other);
        /**
         * @brief �f�X�g���N�^
         */
        ~ConstantBuffer();
        /**
         * @brief �R�s�[
         */
        ConstantBuffer& operator=(const ConstantBuffer& other);
        /**
         * @brief ������
         */
        void init(DeviceResource* device, const std::wstring& name);
        /**
         * @brief �V�F�[�_�[���\�[�X�r���[���쐬����
         * @param device �f�o�C�X
         */
        void createCBV(DeviceResource* device, DescriptorHeapFlag flag);
        /**
         * @brief �X�e�[�W���O���e�ւ̃A�N�Z�X���Z�q
         */
        T* operator->() {
            return &mStaging;
        }
        /**
         * @brief ���݂̃X�e�[�W���O���擾����
         */
        T& getStaging() {
            return mStaging;
        }
        /**
         * @brief �r���[�̎擾
         */
        const ConstantBufferView& getView() const {
            return mCBV;
        }
        /**
         * @brief �X�e�[�W���O���e���X�V����
         */
        void updateStaging();

    private:
        T mStaging;
        Buffer mBuffer;
        ConstantBufferView mCBV;
        void* mMapped = nullptr;
    };
    template <class T>
    inline ConstantBuffer<T>::ConstantBuffer(const ConstantBuffer& other) {
        *this = other;
    }
    template <class T>
    inline ConstantBuffer<T>::~ConstantBuffer() {
        if (mMapped) {
            mBuffer.unmap();
            mMapped = nullptr;
        }
    }
    template <class T>
    inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(const ConstantBuffer& other) {
        if (this == &other) return *this;
        this->mCBV = other.mCBV;
        this->mStaging = other.mStaging;
        this->mBuffer = other.mBuffer;

        //����̃}�b�v���g���Ɖ���ς݂ɃA�N�Z�X����̂�
        //�����Ń}�b�v����������
        this->mMapped = this->mBuffer.map();
        return *this;
    }
    //������
    template <class T>
    inline void ConstantBuffer<T>::init(DeviceResource* device, const std::wstring& name) {
        mBuffer.init(device, Buffer::Usage::ConstantBuffer, sizeof(T), 0, name);

        mMapped = mBuffer.map();
    }
    template <class T>
    inline void ConstantBuffer<T>::createCBV(DeviceResource* device, DescriptorHeapFlag flag) {
        mCBV.init(device, mBuffer, flag);
    }
    //�X�e�[�W���O�̍X�V
    template <class T>
    inline void ConstantBuffer<T>::updateStaging() {
        memcpy(mMapped, &mStaging, sizeof(T));
    }
} // namespace Framework::DX
