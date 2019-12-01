#include "BottomLevelAccelerationStructure.h"
#include "DX/Util/Helper.h"

namespace {
    inline constexpr DXGI_FORMAT toFormatFromIndexSize(UINT size) {
        switch (size) {
        case 2: return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
        case 4: return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
        default: return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        }
    }
} // namespace

namespace Framework::DX {

    BottomLevelAccelerationStructure::BottomLevelAccelerationStructure(const DXRDevice& device,
        const VertexBuffer& vertexBuffer, UINT vertexSize, const IndexBuffer& indexBuffer,
        UINT indexSize) {
        D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
        geometryDesc.Type
            = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
        geometryDesc.Triangles.IndexBuffer = indexBuffer.getResource()->GetGPUVirtualAddress();
        geometryDesc.Triangles.IndexCount
            = static_cast<UINT>(indexBuffer.getResource()->GetDesc().Width) / indexSize;
        geometryDesc.Triangles.IndexFormat = toFormatFromIndexSize(indexSize);
        geometryDesc.Triangles.Transform3x4 = 0;
        geometryDesc.Triangles.VertexBuffer.StartAddress
            = vertexBuffer.getResource()->GetGPUVirtualAddress();
        geometryDesc.Triangles.VertexBuffer.StrideInBytes = vertexSize;
        geometryDesc.Triangles.VertexCount
            = static_cast<UINT>(vertexBuffer.getResource()->GetDesc().Width) / vertexSize;
        geometryDesc.Triangles.VertexFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
        geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags
            = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::
                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC
        bottomLevelBuildDesc = {};
        D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& bottomLevelInputs
            = bottomLevelBuildDesc.Inputs;
        bottomLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
        bottomLevelInputs.Flags = buildFlags;
        bottomLevelInputs.NumDescs = 1;
        bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
        bottomLevelInputs.pGeometryDescs = &geometryDesc;

        //\’z‚É•K—v‚Èî•ñ‚ðŽæ“¾‚·‚é
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO
        bottomLevelPreInfo = {};
        device.getDXRDevice()->GetRaytracingAccelerationStructurePrebuildInfo(
            &bottomLevelInputs, &bottomLevelPreInfo);
        MY_THROW_IF_FALSE(bottomLevelPreInfo.ResultDataMaxSizeInBytes > 0);

        mScratch = createUAVBuffer(device.getDXRDevice(), bottomLevelPreInfo.ScratchDataSizeInBytes,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"ScratchResource");

        D3D12_RESOURCE_STATES initResourceState
            = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
        mBuffer = createUAVBuffer(device.getDXRDevice(),
            bottomLevelPreInfo.ResultDataMaxSizeInBytes, initResourceState, L"BottomLevelAS");

        bottomLevelBuildDesc.ScratchAccelerationStructureData = mScratch->GetGPUVirtualAddress();
        bottomLevelBuildDesc.DestAccelerationStructureData = mBuffer->GetGPUVirtualAddress();

        device.getDXRCommandList()->BuildRaytracingAccelerationStructure(
            &bottomLevelBuildDesc, 0, nullptr);
        device.getDXRCommandList()->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::UAV(mBuffer.Get()));
    }
    BottomLevelAccelerationStructure::~BottomLevelAccelerationStructure() {}
} // namespace Framework::DX
