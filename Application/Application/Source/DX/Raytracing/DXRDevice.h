/**
 * @file DXRDevice.h
 * @brief DXR用デバイス管理
 */

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
        DXRDevice();
        /**
         * @brief デストラクタ
         */
        ~DXRDevice();
        /**
         * @brief デバイスのリセット
         */
        void reset();
        /**
         * @brief デバイスの生成
         */
        void create(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
        /**
         * @brief デバイスの取得
         */
        ID3D12Device5* getDXRDevice() const {
            return mDXRDevice.Get();
        }
        /**
         * @brief コマンドリストの取得
         */
        ID3D12GraphicsCommandList5* getDXRCommandList() const {
            return mDXRCommandList.Get();
        }

    private:
        Comptr<ID3D12Device5> mDXRDevice; //!< DXRに対応したデバイス
        Comptr<ID3D12GraphicsCommandList5> mDXRCommandList; //!< DXRに対応したコマンドリスト
    };
} // namespace Framework::DX
