/**
 * @file TopLevelAccelerationStructure.h
 * @brief トップレベルのAS
 * @details ジオメトリのシーン情報定義
 */

#pragma once
#include <DirectXMath.h>
#include "DX/Raytracing/BottomLevelAccelerationStructure.h"
#include "DX/Raytracing/DXRDevice.h"
#include "DX/Resource/Buffer.h"

namespace Framework::DX {
    /**
     * @class TopLevelAccelerationStructure
     * @brief トップレベルAS
     */
    class TopLevelAccelerationStructure {
    public:
        /**
         * @brief TLASのインスタンスディスク
         */
        struct InstanceDesc {
            UINT instanceID; //!< ID
            UINT mask; //!< マスク
            UINT flags; //!< ジオメトリのフラグ
            UINT hitGroupIndex; //!< ヒットグループの計算に使用するインデックス
            BottomLevelAccelerationStructure* blas; //!< 対象となるジオメトリ
            DirectX::XMMATRIX transform; //!< ジオメトリのトランスフォーム
            /**
             * @brief コンストラクタ
             */
            InstanceDesc()
                : instanceID(0), mask(0), flags(0), hitGroupIndex(0), blas(nullptr), transform() {}
        };

    public:
        /**
         * @brief コンストラクタ
         */
        TopLevelAccelerationStructure();
        /**
         * @brief デストラクタ
         */
        ~TopLevelAccelerationStructure();
        /**
         * @brief インスタンスディスクを追加する
         */
        void add(const InstanceDesc& desc);
        /**
         * @brief　TLASを構築する
         * @param device デバイス
         * @param buildFlag TLASの構築フラグ
         */
        void build(const DXRDevice& device, DeviceResource* deviceResource,
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        /**
         * @brief ディスクをクリアする
         */
        void clear();
        /**
         * @brief バッファを取得する
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        const ShaderResourceView& getView() const {
            return mSRV;
        }

    private:
        /**
         * @brief 構築に必要な事前情報を構築する
         * @param device デバイス
         * @param buildFlag TLASの構築フラグ
         */
        void buildPrebuildInfo(
            const DXRDevice& device, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag);

    private:
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC mDesc; //!< ディスク
        std::vector<D3D12_RAYTRACING_INSTANCE_DESC> mInstanceDescs; //!< シーン情報ディスク
        Comptr<ID3D12Resource> mScratch; //!< スクラッチリソース
        Comptr<ID3D12Resource> mInstance; //!< 一時的なリソース
        Buffer mBuffer;
        //Comptr<ID3D12Resource> mBuffer; //!< TLASバッファ
        ShaderResourceView mSRV;
    };
} // namespace Framework::DX
