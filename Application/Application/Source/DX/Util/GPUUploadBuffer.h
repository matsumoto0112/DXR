/**
 * @file GPUUploadBuffer.h
 * @brief GPU�ɃA�b�v���[�h�\�ȃo�b�t�@
 */

#pragma once
#include <d3d12.h>
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
     * @class GPUUploadBuffer
     * @brief GPU�ɃA�b�v���[�h����o�b�t�@
     */
    class GPUUploadBuffer {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        GPUUploadBuffer();
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~GPUUploadBuffer();
        /**
         * @brief �������̃A���P�[�g����
         * @device �f�o�C�X
         * @bufferSize �o�b�t�@�̑傫��
         * @param name ���\�[�X��
         */
        void allocate(ID3D12Device* device, UINT bufferSize, LPCWSTR name);
        /**
         * @brief CPU�������ݗp�̃�������Ԃ�
         */
        uint8_t* getMapCPUWriteOnly();
        /**
         * @brief ���\�[�X���擾����
         */
        ComPtr<ID3D12Resource> getResource() const { return mResource; }

    protected:
        ComPtr<ID3D12Resource> mResource; //!< ���\�[�X
    };
} // namespace Framework::DX
