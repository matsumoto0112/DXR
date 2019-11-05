#pragma once
#include "DX/DeviceResource.h"

namespace Framework::DX {
    /**
    * @class DXRDevice
    * @brief DXR用デバイス
    */
    class DXRDevice {
    public:
        /**
        * @brief コンストラクタ
        */
        DXRDevice(DeviceResource* deviceResource);
        /**
        * @brief デストラクタ
        */
        ~DXRDevice();
        /**
        * @brief デバイスのリセット
        */
        void reset();
        /**
        * @brief デバイスの再生成
        */
        void recreate();
        ID3D12Device5* getDXRDevice() const { return mDXRDevice.Get(); }
        ID3D12GraphicsCommandList5* getDXRCommandList() const { return mDXRCommandList.Get(); }
    private:
        DeviceResource* mDeviceResource;
        ComPtr<ID3D12Device5> mDXRDevice;
        ComPtr<ID3D12GraphicsCommandList5> mDXRCommandList;
    };
} //Framework::DX
