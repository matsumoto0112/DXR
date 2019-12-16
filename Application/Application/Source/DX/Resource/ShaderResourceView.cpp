#include "ShaderResourceView.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/DeviceResource.h"

namespace Framework::DX {
    void ShaderResourceView::initAsTexture2D(
        DeviceResource* device, const Buffer& buffer, DXGI_FORMAT format, bool isGlobal) {
        //シェーダーリソースビューを作成する
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = format;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.PlaneSlice = 0;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

        createShaderResourceView(device, buffer.getResource(), srvDesc, isGlobal);
    }
    void ShaderResourceView::initAsBuffer(
        DeviceResource* device, const Buffer& buffer, bool isGlobal) {
        UINT bufferNum = buffer.getSize() / buffer.getStride();

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        srvDesc.Buffer.NumElements = bufferNum;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
        srvDesc.Buffer.StructureByteStride = buffer.getStride();
        createShaderResourceView(device, buffer.getResource(), srvDesc, isGlobal);
    }
    void ShaderResourceView::initAsRawBuffer(
        DeviceResource* device, const Buffer& buffer, UINT numElements, bool isGlobal) {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        srvDesc.Buffer.NumElements = numElements;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
        srvDesc.Buffer.StructureByteStride = 0;
        createShaderResourceView(device, buffer.getResource(), srvDesc, isGlobal);
    }
    void ShaderResourceView::initAsRaytracingAccelerationStructure(
        DeviceResource* device, const Buffer& buffer, bool isGlobal) {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension
            = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
        srvDesc.RaytracingAccelerationStructure.Location
            = buffer.getResource()->GetGPUVirtualAddress();
        createShaderResourceView(device, nullptr, srvDesc, isGlobal);
    }
    void ShaderResourceView::createShaderResourceView(DeviceResource* device,
        ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, bool isGlobal) {
        if (isGlobal) {
            mInfo = device->getRaytracingDescriptorManager()->getGlobalView()->allocate(
                device->getDevice());
        } else {
            mInfo = device->getRaytracingDescriptorManager()->getLocalView()->allocate(
                device->getDevice());
        }
        device->getDevice()->CreateShaderResourceView(resource, &desc, mInfo.cpuHandle);
    }
} // namespace Framework::DX
