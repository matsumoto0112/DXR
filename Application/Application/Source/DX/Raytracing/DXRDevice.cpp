#include "DXRDevice.h"
#include "Utility/Debug.h"

namespace Framework::DX {
    //コンストラクタ
    DXRDevice::DXRDevice(DeviceResource* deviceResource)
        :mDeviceResource(deviceResource) { }
    //デストラクタ
    DXRDevice::~DXRDevice() {
        reset();
    }
    //デバイスのリセット
    void DXRDevice::reset() {
        mDXRDevice.Reset();
        mDXRCommandList.Reset();
    }
    //デバイスの再生成
    void DXRDevice::recreate() {
        ID3D12Device* device = mDeviceResource->getDevice();
        ID3D12CommandList* commandList = mDeviceResource->getCommandList();

        MY_THROW_IF_FAILED(device->QueryInterface(IID_PPV_ARGS(&mDXRDevice)));
        MY_THROW_IF_FAILED(commandList->QueryInterface(IID_PPV_ARGS(&mDXRCommandList)));
        mDXRDevice->SetName(L"DXRDevice");
        mDXRCommandList->SetName(L"DXRCommandList");
    }
} //Framework::DX
