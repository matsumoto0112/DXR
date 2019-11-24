#include "DXRDevice.h"
#include "Utility/Debug.h"

namespace Framework::DX {
    //�R���X�g���N�^
    DXRDevice::DXRDevice(DeviceResource* deviceResource)
        :mDeviceResource(deviceResource) { }
    //�f�X�g���N�^
    DXRDevice::~DXRDevice() {
        reset();
    }
    //�f�o�C�X�̃��Z�b�g
    void DXRDevice::reset() {
        mDXRDevice.Reset();
        mDXRCommandList.Reset();
    }
    //�f�o�C�X�̍Đ���
    void DXRDevice::recreate() {
        ID3D12Device* device = mDeviceResource->getDevice();
        ID3D12CommandList* commandList = mDeviceResource->getCommandList();

        MY_THROW_IF_FAILED(device->QueryInterface(IID_PPV_ARGS(&mDXRDevice)));
        MY_THROW_IF_FAILED(commandList->QueryInterface(IID_PPV_ARGS(&mDXRCommandList)));
        mDXRDevice->SetName(L"DXRDevice");
        mDXRCommandList->SetName(L"DXRCommandList");
    }
} //Framework::DX
