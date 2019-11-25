#include "DXRDevice.h"

namespace Framework::DX {
    //コンストラクタ
    DXRDevice::DXRDevice() {}
    //デストラクタ
    DXRDevice::~DXRDevice() { reset(); }
    //デバイスのリセット
    void DXRDevice::reset() {
        mDXRDevice.Reset();
        mDXRCommandList.Reset();
    }
    //デバイスの生成
    void DXRDevice::create(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) {
        MY_THROW_IF_FAILED(device->QueryInterface(IID_PPV_ARGS(&mDXRDevice)));
        MY_THROW_IF_FAILED(commandList->QueryInterface(IID_PPV_ARGS(&mDXRCommandList)));
        mDXRDevice->SetName(L"DXRDevice");
        mDXRCommandList->SetName(L"DXRCommandList");
    }
} // namespace Framework::DX
