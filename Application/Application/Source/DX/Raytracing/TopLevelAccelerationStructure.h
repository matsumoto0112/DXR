/**
 * @file TopLevelAccelerationStructure.h
 * @brief トップレベルのAS
 * @details ジオメトリのシーン情報定義
 */

#pragma once
#include <DirectXMath.h>
#include "DX/Raytracing/BottomLevelAccelerationStructure.h"
#include "DX/Raytracing/DXRDevice.h"

namespace Framework::DX {
    /**
     * @class TopLevelAccelerationStructure
     * @brief トップレベルAS
     */
    class TopLevelAccelerationStructure {
    public:
        struct InstanceDesc {
            UINT instanceID;
            UINT mask;
            UINT flags;
            UINT hitGroupIndex;
            BottomLevelAccelerationStructure* blas;
            DirectX::XMMATRIX transform;

            InstanceDesc()
                : instanceID(0), mask(0), flags(0), hitGroupIndex(0), blas(nullptr), transform() {}
        };

    public:
        /**
         * @brief
         */
        TopLevelAccelerationStructure();
        /**
         * @brief
         */
        ~TopLevelAccelerationStructure();

        void add(const InstanceDesc& desc);
        void build(
            const DXRDevice& device, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag);
        void clear();

        ID3D12Resource* getBuffer() const {
            return mBuffer.Get();
        }

    private:
        void buildPrebuildInfo(
            const DXRDevice& device, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag);

    private:
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC mDesc;
        std::vector<D3D12_RAYTRACING_INSTANCE_DESC> mInstanceDescs;
        ComPtr<ID3D12Resource> mScratch;
        ComPtr<ID3D12Resource> mInstance;
        ComPtr<ID3D12Resource> mBuffer;
    };
} // namespace Framework::DX
