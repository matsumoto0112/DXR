#include "TopLevelAccelerationStructure.h"
#include "DX/Util/Helper.h"

using namespace DirectX;

namespace {
    inline Comptr<ID3D12Resource> createBuffer(
        ID3D12Device* device, void* data, UINT size, const std::wstring& name) {
        Comptr<ID3D12Resource> result = Framework::DX::createUploadBuffer(device, size, name);
        Framework::DX::writeToResource(result.Get(), data, size);
        return result;
    }
} // namespace

namespace Framework::DX {
    TopLevelAccelerationStructure::TopLevelAccelerationStructure() {}

    TopLevelAccelerationStructure::~TopLevelAccelerationStructure() {}

    void TopLevelAccelerationStructure::add(const InstanceDesc& desc) {
        MY_THROW_IF_FALSE(desc.blas);

        D3D12_RAYTRACING_INSTANCE_DESC instanceDesc = {};
        instanceDesc.InstanceID = desc.instanceID;
        instanceDesc.InstanceMask = desc.mask;
        instanceDesc.Flags = desc.flags;
        instanceDesc.InstanceContributionToHitGroupIndex = desc.hitGroupIndex;
        instanceDesc.AccelerationStructure = desc.blas->getBuffer()->GetGPUVirtualAddress();
        XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc.Transform), desc.transform);

        mInstanceDescs.emplace_back(instanceDesc);
    }

    void TopLevelAccelerationStructure::build(const DXRDevice& device,
        DeviceResource* deviceResource,
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag) {
        const UINT size
            = static_cast<UINT>(mInstanceDescs.size() * sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
        //前のディスクの状態から更新する必要があるなら更新する
        if (!(mDesc.Inputs.NumDescs == mInstanceDescs.size() && mDesc.Inputs.Flags == buildFlag)) {
            buildPrebuildInfo(device, buildFlag);
            mInstance = createUploadBuffer(device.getDXRDevice(), size, L"InstanceDesc");
            mDesc.Inputs.InstanceDescs = mInstance->GetGPUVirtualAddress();
        }
        writeToResource(mInstance.Get(), mInstanceDescs.data(), size);
        device.getDXRCommandList()->BuildRaytracingAccelerationStructure(&mDesc, 0, nullptr);
        mSRV.initAsRaytracingAccelerationStructure(
            deviceResource, mBuffer, DescriptorHeapType::RaytracingGlobal);
    }

    void TopLevelAccelerationStructure::clear() {
        mInstanceDescs.clear();
    }

    void TopLevelAccelerationStructure::buildPrebuildInfo(
        const DXRDevice& dxrDevice, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlag) {
        ID3D12Device5* device = dxrDevice.getDXRDevice();

        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& topLevelInputs = mDesc.Inputs;
        topLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
        topLevelInputs.Flags = buildFlag;
        topLevelInputs.NumDescs = static_cast<UINT>(mInstanceDescs.size());
        topLevelInputs.pGeometryDescs = nullptr;
        topLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO preInfo = {};
        device->GetRaytracingAccelerationStructurePrebuildInfo(&topLevelInputs, &preInfo);
        MY_THROW_IF_FALSE(preInfo.ResultDataMaxSizeInBytes > 0);

        mScratch = createUAVBuffer(device, preInfo.ScratchDataSizeInBytes,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"ScratchResource");

        Comptr<ID3D12Resource> res = createUAVBuffer(device, preInfo.ResultDataMaxSizeInBytes,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
            L"TopLevelAS");
        mBuffer.init(
            res, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);

        D3D12_RESOURCE_STATES initResourceState
            = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;

        mDesc.DestAccelerationStructureData = mBuffer.getResource()->GetGPUVirtualAddress();
        mDesc.ScratchAccelerationStructureData = mScratch->GetGPUVirtualAddress();
    }

} // namespace Framework::DX
