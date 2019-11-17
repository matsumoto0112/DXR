#pragma once
#include <d3d12.h>
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @class IBuffer
    * @brief ���\�[�X�o�b�t�@���N���X
    */
    class IBuffer {
    public:
        /**
        * @brief �f�X�g���N�^
        */
        virtual ~IBuffer() = default;
        /**
        * @brief ���\�[�X�̎擾
        */
        ID3D12Resource* getResource() const { return mResource.Get(); }
        /**
        * @brief CPU�n���h���̃Z�b�g
        */
        void setCPUHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle) { mCPUHandle = handle; }
        /**
        * @brief CPU�n���h���̎擾
        */
        D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle()const { return mCPUHandle; };
        /**
        * @brief GPU�n���h���̃Z�b�g
        */
        void setGPUHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle) { mGPUHandle = handle; }
        /**
        * @brief GPU�n���h���̎擾
        */
        D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle() const { return mGPUHandle; }
    //protected:
        ComPtr<ID3D12Resource> mResource; //!< ���\�[�X
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle; //!< CPU�n���h��
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle; //!< GPU�n���h��
    };
} //Framework::DX
