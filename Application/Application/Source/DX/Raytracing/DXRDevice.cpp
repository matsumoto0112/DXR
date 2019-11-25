#include "DXRDevice.h"

namespace Framework::DX {
    //�R���X�g���N�^
    DXRDevice::DXRDevice() {}
    //�f�X�g���N�^
    DXRDevice::~DXRDevice() { reset(); }
    //�f�o�C�X�̃��Z�b�g
    void DXRDevice::reset() {
        mDXRDevice.Reset();
        mDXRCommandList.Reset();
    }
    //�f�o�C�X�̐���
    void DXRDevice::create(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) {
        MY_THROW_IF_FAILED(device->QueryInterface(IID_PPV_ARGS(&mDXRDevice)));
        MY_THROW_IF_FAILED(commandList->QueryInterface(IID_PPV_ARGS(&mDXRCommandList)));
        mDXRDevice->SetName(L"DXRDevice");
        mDXRCommandList->SetName(L"DXRCommandList");
    }
} // namespace Framework::DX
