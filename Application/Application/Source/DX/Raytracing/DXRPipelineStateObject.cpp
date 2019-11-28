#include "DXRPipelineStateObject.h"

namespace Framework::DX {

    DXRPipelineStateObject::DXRPipelineStateObject()
        : mPipelineStateObjectDesc{
              D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE
          } {}
    DXRPipelineStateObject::~DXRPipelineStateObject() {}
    void DXRPipelineStateObject::bindHitGroup(const HitGroupDesc& desc) {
        CD3DX12_HIT_GROUP_SUBOBJECT* hitGroup
            = mPipelineStateObjectDesc.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
        if (!desc.closestHitName.empty())
            hitGroup->SetClosestHitShaderImport(desc.closestHitName.c_str());
        if (!desc.anyHitName.empty()) hitGroup->SetAnyHitShaderImport(desc.anyHitName.c_str());
        if (!desc.intersectionName.empty())
            hitGroup->SetIntersectionShaderImport(desc.intersectionName.c_str());
        hitGroup->SetHitGroupExport(desc.name.c_str());
        hitGroup->SetHitGroupType(desc.type);
    }
    void DXRPipelineStateObject::setConfig(UINT payloadSize, UINT attributeSize) {
        CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT* config
            = mPipelineStateObjectDesc
                  .CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
        config->Config(payloadSize, attributeSize);
    }
} // namespace Framework::DX
