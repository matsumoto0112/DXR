#include "DXRPipelineStateObject.h"
#include "DX/Shader/RootSignature.h"

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
    void DXRPipelineStateObject::bindLocalRootSignature(
        const RootSignature& localRootSignature, const std::wstring& targetShaderName) {
        CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT* local
            = mPipelineStateObjectDesc.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
        local->SetRootSignature(localRootSignature.getRootSignature());

        CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT* asso
            = mPipelineStateObjectDesc
                  .CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
        asso->SetSubobjectToAssociate(*local);
        asso->AddExport(targetShaderName.c_str());
    }
} // namespace Framework::DX
