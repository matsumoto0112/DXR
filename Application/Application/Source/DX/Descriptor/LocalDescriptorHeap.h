#pragma once

namespace Framework::DX {
    /**
     * @class LocalDescriptorHeap
     * @brief ���[�J���̃f�B�X�N���v�^�q�[�v
     */
    class LocalDescriptorHeap {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        LocalDescriptorHeap() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~LocalDescriptorHeap() {}
        /**
         * @brief ������
         */
        void init(ID3D12Device* device, UINT descriptorNum, D3D12_DESCRIPTOR_HEAP_TYPE type);
        /**
         * @brief �A���P�[�g����
         * @param[out] cpuHandle �m�ۂ���CPU�n���h��
         * @param[out] gpuHandle �m�ۂ���GPU�n���h��
         * @return �A���P�[�g�ɐ���������true��Ԃ�
         */
        bool allocate(
            D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle);
        /**
         * @brief �q�[�v�̎擾
         */
        ID3D12DescriptorHeap* getHeap() const {
            return mDescriptorHeap.Get();
        }
        /**
         * @brief �f�B�X�N���v�^�̑傫����Ԃ�
         */
        UINT getDescriptorHeapSize() const {
            return mDescriptorHeapSize;
        }
        /**
         * @brief �f�B�X�N���v�^�̊m�ې���Ԃ�
         */
        UINT getDescriptorHeapNum() const {
            return mDescriptorHeapNum;
        }

    private:
        Comptr<ID3D12DescriptorHeap> mDescriptorHeap;
        UINT mCurrentAllocatedNum = 0;
        UINT mDescriptorHeapSize = 0;
        UINT mDescriptorHeapNum = 0;
    };
} // namespace Framework::DX