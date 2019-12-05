/**
 * @file DXRDevice.h
 * @brief DXR�p�f�o�C�X�Ǘ�
 */

#pragma once
#include "DX/DeviceResource.h"

namespace Framework::DX {
    /**
     * @class DXRDevice
     * @brief DXR�p�f�o�C�X
     */
    class DXRDevice {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        DXRDevice();
        /**
         * @brief �f�X�g���N�^
         */
        ~DXRDevice();
        /**
         * @brief �f�o�C�X�̃��Z�b�g
         */
        void reset();
        /**
         * @brief �f�o�C�X�̐���
         */
        void create(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
        /**
         * @brief �f�o�C�X�̎擾
         */
        ID3D12Device5* getDXRDevice() const {
            return mDXRDevice.Get();
        }
        /**
         * @brief �R�}���h���X�g�̎擾
         */
        ID3D12GraphicsCommandList5* getDXRCommandList() const {
            return mDXRCommandList.Get();
        }

    private:
        Comptr<ID3D12Device5> mDXRDevice; //!< DXR�ɑΉ������f�o�C�X
        Comptr<ID3D12GraphicsCommandList5> mDXRCommandList; //!< DXR�ɑΉ������R�}���h���X�g
    };
} // namespace Framework::DX
