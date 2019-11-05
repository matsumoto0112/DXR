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
        DXRDevice(DeviceResource* deviceResource);
        /**
        * @brief �f�X�g���N�^
        */
        ~DXRDevice();
        /**
        * @brief �f�o�C�X�̃��Z�b�g
        */
        void reset();
        /**
        * @brief �f�o�C�X�̍Đ���
        */
        void recreate();
    private:
        DeviceResource* mDeviceResource;
        ComPtr<ID3D12Device5> mDXRDevice;
        ComPtr<ID3D12GraphicsCommandList5> mDXRCommandList;
    };
} //Framework::DX
