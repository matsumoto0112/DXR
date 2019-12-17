/**
 * @file DescriptorAllocator.h
 * @brief �f�B�X�N���v�^�̃A���P�[�g�Ǘ�
 */

#pragma once
#include "DX/Descriptor/DescriptorInfo.h"
#include "DX/Descriptor/LocalDescriptorHeap.h"

namespace Framework::DX {
    /**
     * @class DescriptorAllocator
     * @brief �f�B�X�N���v�^�̊m�ۊǗ�
     */
    class DescriptorAllocator {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        DescriptorAllocator() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~DescriptorAllocator() {}
        /**
         * @brief ������
         */
        void init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorNum);
        /**
         * @brief �f�B�X�N���v�^�̊m��
         */
        DescriptorInfo allocate(ID3D12Device* device);

    private:
        /**
         * @brief �q�[�v�̒ǉ�
         */
        void addHeap(ID3D12Device* device);

    private:
        std::vector<LocalDescriptorHeap> mStackHeaps; //!< �m�ۍς݂̃q�[�v
        D3D12_DESCRIPTOR_HEAP_TYPE mHeapType;
        UINT mHeapNum;
    };
} // namespace Framework::DX
