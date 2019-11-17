#pragma once
#include <d3d12.h>
#include "Desc/DescriptorTableDesc.h"
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"

namespace Framework::DX {
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
        * @param desc �f�[�u���f�B�X�N
        */
        void create(ID3D12Device* device, const Desc::DescriptorTableDesc& desc);
        /**
        * @brief �q�[�v�̃��Z�b�g
        */
        void reset();
        /**
        * @brief �f�B�X�N���v�^�q�[�v�̎擾
        */
        inline ID3D12DescriptorHeap* getHeap() const { return mHeap.Get(); }
        /**
        * @brief CPU�n���h���̎擾
        */
        CD3DX12_CPU_DESCRIPTOR_HANDLE getCPUHandle(UINT index);
        /**
        * @brief GPU�n���h���̎擾
        */
        CD3DX12_GPU_DESCRIPTOR_HANDLE getGPUHandle(UINT index);
    private:
        ComPtr<ID3D12DescriptorHeap> mHeap; //!< �f�B�X�N���v�^�q�[�v
        UINT mDescriptorSize; //!< �f�B�X�N���v�^�̃T�C�Y
    };
} //Framework::DX
