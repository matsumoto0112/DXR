#pragma once
#include <d3d12.h>
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @enum HeapType
    * @brief �q�[�v�̎�ޒ�`
    */
    enum class HeapType {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
    };

    /**
    * @enum HeapFlag
    * @brief �q�[�v�̃t���O��`
    */
    enum class HeapFlag {
        None,
        ShaderVisible,
    };

    /**
    * @class DescriptorTable
    * @brief �f�B�X�N���v�^�q�[�v�e�[�u��
    */
    class DescriptorTable {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        DescriptorTable();
        /**
        * @brief �f�X�g���N�^
        */
        ~DescriptorTable();
        /**
        * @brief �q�[�v�̍쐬
        * @param device �f�o�C�X
        * @param heapType �q�[�v�̎��
        * @param heapFlag �q�[�v�t���O
        * @param descriptorNum ���蓖�Ă�q�[�v�̐�
        * @param name �q�[�v��
        */
        void create(ID3D12Device* device, HeapType heapType, HeapFlag heapFlag, UINT descriptorNum, LPCWSTR name = nullptr);
        /**
        * @brief �q�[�v�̃��Z�b�g
        */
        void reset();
        /**
        * @brief �f�B�X�N���v�^�q�[�v�̎擾
        */
        inline ID3D12DescriptorHeap* getHeap() const { return mHeap.Get(); }
        CD3DX12_CPU_DESCRIPTOR_HANDLE getCPUHandle(UINT index);
        /**
        * @brief
        */
        CD3DX12_GPU_DESCRIPTOR_HANDLE getGPUHandle(UINT index);
    private:
        ComPtr<ID3D12DescriptorHeap> mHeap; //!< �f�B�X�N���v�^�q�[�v
        UINT mDescriptorSize; //!< �f�B�X�N���v�^�̃T�C�Y
    };
} //Framework::DX
