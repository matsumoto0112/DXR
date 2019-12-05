#include "DXRPipelineStateObject.h"
#include "DX/Shader/RootSignature.h"

namespace {
    inline void debugPrintDesc(const D3D12_STATE_OBJECT_DESC* desc) {
        std::wstringstream wstr;
        wstr << L"\n";
        wstr << L"--------------------------------------------------------------------\n";
        wstr << L"| D3D12 State Object 0x" << static_cast<const void*>(desc) << L": ";
        if (desc->Type == D3D12_STATE_OBJECT_TYPE_COLLECTION) wstr << L"Collection\n";
        if (desc->Type == D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE)
            wstr << L"Raytracing Pipeline\n";

        auto ExportTree = [](UINT depth, UINT numExports, const D3D12_EXPORT_DESC* exports) {
            std::wostringstream woss;
            for (UINT i = 0; i < numExports; i++) {
                woss << L"|";
                if (depth > 0) {
                    for (UINT j = 0; j < 2 * depth - 1; j++) woss << L" ";
                }
                woss << L" [" << i << L"]: ";
                if (exports[i].ExportToRename) woss << exports[i].ExportToRename << L" --> ";
                woss << exports[i].Name << L"\n";
            }
            return woss.str();
        };

        for (UINT i = 0; i < desc->NumSubobjects; i++) {
            wstr << L"| [" << i << L"]: ";
            switch (desc->pSubobjects[i].Type) {
            case D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE:
                wstr << L"Global Root Signature 0x" << desc->pSubobjects[i].pDesc << L"\n";
                break;
            case D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE:
                wstr << L"Local Root Signature 0x" << desc->pSubobjects[i].pDesc << L"\n";
                break;
            case D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK:
                wstr << L"Node Mask: 0x" << std::hex << std::setfill(L'0') << std::setw(8)
                     << *static_cast<const UINT*>(desc->pSubobjects[i].pDesc) << std::setw(0)
                     << std::dec << L"\n";
                break;
            case D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY: {
                wstr << L"DXIL Library 0x";
                auto lib = static_cast<const D3D12_DXIL_LIBRARY_DESC*>(desc->pSubobjects[i].pDesc);
                wstr << lib->DXILLibrary.pShaderBytecode << L", " << lib->DXILLibrary.BytecodeLength
                     << L" bytes\n";
                wstr << ExportTree(1, lib->NumExports, lib->pExports);
                break;
            }
            case D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION: {
                wstr << L"Existing Library 0x";
                auto collection = static_cast<const D3D12_EXISTING_COLLECTION_DESC*>(
                    desc->pSubobjects[i].pDesc);
                wstr << collection->pExistingCollection << L"\n";
                wstr << ExportTree(1, collection->NumExports, collection->pExports);
                break;
            }
            case D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION: {
                wstr << L"Subobject to Exports Association (Subobject [";
                auto association = static_cast<const D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION*>(
                    desc->pSubobjects[i].pDesc);
                UINT index
                    = static_cast<UINT>(association->pSubobjectToAssociate - desc->pSubobjects);
                wstr << index << L"])\n";
                for (UINT j = 0; j < association->NumExports; j++) {
                    wstr << L"|  [" << j << L"]: " << association->pExports[j] << L"\n";
                }
                break;
            }
            case D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION: {
                wstr << L"DXIL Subobjects to Exports Association (";
                auto association = static_cast<const D3D12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION*>(
                    desc->pSubobjects[i].pDesc);
                wstr << association->SubobjectToAssociate << L")\n";
                for (UINT j = 0; j < association->NumExports; j++) {
                    wstr << L"|  [" << j << L"]: " << association->pExports[j] << L"\n";
                }
                break;
            }
            case D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG: {
                wstr << L"Raytracing Shader Config\n";
                auto config = static_cast<const D3D12_RAYTRACING_SHADER_CONFIG*>(
                    desc->pSubobjects[i].pDesc);
                wstr << L"|  [0]: Max Payload Size: " << config->MaxPayloadSizeInBytes
                     << L" bytes\n";
                wstr << L"|  [1]: Max Attribute Size: " << config->MaxAttributeSizeInBytes
                     << L" bytes\n";
                break;
            }
            case D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG: {
                wstr << L"Raytracing Pipeline Config\n";
                auto config = static_cast<const D3D12_RAYTRACING_PIPELINE_CONFIG*>(
                    desc->pSubobjects[i].pDesc);
                wstr << L"|  [0]: Max Recursion Depth: " << config->MaxTraceRecursionDepth << L"\n";
                break;
            }
            case D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP: {
                wstr << L"Hit Group (";
                auto hitGroup
                    = static_cast<const D3D12_HIT_GROUP_DESC*>(desc->pSubobjects[i].pDesc);
                wstr << (hitGroup->HitGroupExport ? hitGroup->HitGroupExport : L"[none]") << L")\n";
                wstr << L"|  [0]: Any Hit Import: "
                     << (hitGroup->AnyHitShaderImport ? hitGroup->AnyHitShaderImport : L"[none]")
                     << L"\n";
                wstr << L"|  [1]: Closest Hit Import: "
                     << (hitGroup->ClosestHitShaderImport ? hitGroup->ClosestHitShaderImport
                                                          : L"[none]")
                     << L"\n";
                wstr << L"|  [2]: Intersection Import: "
                     << (hitGroup->IntersectionShaderImport ? hitGroup->IntersectionShaderImport
                                                            : L"[none]")
                     << L"\n";
                break;
            }
            }
            wstr << L"|--------------------------------------------------------------------\n";
        }
        wstr << L"\n";
        OutputDebugStringW(wstr.str().c_str());
    }
} // namespace

namespace Framework::DX {
    //コンストラクタ
    DXRPipelineStateObject::DXRPipelineStateObject(DXRDevice* device)
        : mDevice(device),
          mPipelineStateObjectDesc{
              D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE
          } {}
    //デストラクタ
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
    void DXRPipelineStateObject::setConfig(
        UINT payloadSize, UINT attributeSize, UINT maxRecursionDepth) {
        CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT* shaderConfig
            = mPipelineStateObjectDesc
                  .CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
        shaderConfig->Config(payloadSize, attributeSize);

        CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT* pipelineConfig
            = mPipelineStateObjectDesc
                  .CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
        pipelineConfig->Config(maxRecursionDepth);
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
    void DXRPipelineStateObject::bindGlobalRootSignature(const RootSignature& rootSignature) {
        CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT* global
            = mPipelineStateObjectDesc.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
        global->SetRootSignature(rootSignature.getRootSignature());
    }
    void DXRPipelineStateObject::createPipeline() {
        debugPrintDesc(mPipelineStateObjectDesc);
        MY_THROW_IF_FAILED(mDevice->getDXRDevice()->CreateStateObject(
            mPipelineStateObjectDesc, IID_PPV_ARGS(&mPipelineStateObject)));
    }
    void DXRPipelineStateObject::associateShaderInfoWithKey(
        int key, ShaderType type, const std::wstring& name) {
        mShaderDatas.emplace(key, ShaderData{ type, name, nullptr });
    }

    void DXRPipelineStateObject::prebuild() {
        MY_THROW_IF_FALSE_LOG(mPipelineStateObject, "パイプラインが存在しません");

        Comptr<ID3D12StateObjectProperties> stateObjectProp;
        MY_THROW_IF_FAILED(mPipelineStateObject->QueryInterface(IID_PPV_ARGS(&stateObjectProp)));
        for (auto&& data : mShaderDatas) {
            data.second.id = stateObjectProp->GetShaderIdentifier(data.second.name.c_str());
        }
    }

    void DXRPipelineStateObject::setShaderTableConfig(
        ShaderType type, UINT num, UINT appendSize, const std::wstring& name) {
        UINT shaderIDSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + appendSize;
        mShaderTables[type] = std::make_unique<ShaderTable>(
            mDevice->getDXRDevice(), num, shaderIDSize, name.c_str());
    }
    void DXRPipelineStateObject::appendShaderTable(int key, void* rootArgument) {
        MY_ASSERTION(Utility::isExist(mShaderDatas, key), "キーが存在しません");

        constexpr UINT shaderIDSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
        ShaderType type = mShaderDatas[key].type;
        MY_ASSERTION(Utility::isExist(mShaderTables, type),
            "keyの属するシェーダータイプに対するsetShaderTableConfigがすでに呼ばれていることを確認"
            "してください");

        const UINT rootArgumentSize = mShaderTables[type]->getShaderRecordSize() - shaderIDSize;
        if (rootArgumentSize > 0) {
            mShaderTables[type]->push_back(
                ShaderRecord(mShaderDatas[key].id, shaderIDSize, rootArgument, rootArgumentSize));
        } else {
            mShaderTables[type]->push_back(ShaderRecord(mShaderDatas[key].id, shaderIDSize));
        }
    }
    void DXRPipelineStateObject::buildShaderTable() {
        auto loadResource = [&](ShaderType type) {
            mShaderResources[type].resource = mShaderTables[type]->getResource();
            mShaderResources[type].stride = mShaderTables[type]->getShaderRecordSize();
        };
        loadResource(ShaderType::RayGeneration);
        loadResource(ShaderType::Miss);
        loadResource(ShaderType::HitGroup);

        //不要になったのでクリアする
        mShaderTables.clear();
    }
    void DXRPipelineStateObject::doRaytracing(UINT width, UINT height) {
        D3D12_DISPATCH_RAYS_DESC dispatchDesc = {};
        dispatchDesc.RayGenerationShaderRecord.StartAddress
            = mShaderResources[ShaderType::RayGeneration].resource->GetGPUVirtualAddress();
        dispatchDesc.RayGenerationShaderRecord.SizeInBytes
            = mShaderResources[ShaderType::RayGeneration].resource->GetDesc().Width;

        dispatchDesc.MissShaderTable.StartAddress
            = mShaderResources[ShaderType::Miss].resource->GetGPUVirtualAddress();
        dispatchDesc.MissShaderTable.SizeInBytes
            = mShaderResources[ShaderType::Miss].resource->GetDesc().Width;
        dispatchDesc.MissShaderTable.StrideInBytes = mShaderResources[ShaderType::Miss].stride;

        dispatchDesc.HitGroupTable.StartAddress
            = mShaderResources[ShaderType::HitGroup].resource->GetGPUVirtualAddress();
        dispatchDesc.HitGroupTable.SizeInBytes
            = mShaderResources[ShaderType::HitGroup].resource->GetDesc().Width;
        dispatchDesc.HitGroupTable.StrideInBytes = mShaderResources[ShaderType::HitGroup].stride;

        dispatchDesc.Width = width;
        dispatchDesc.Height = height;
        dispatchDesc.Depth = 1;

        mDevice->getDXRCommandList()->SetPipelineState1(mPipelineStateObject.Get());
        mDevice->getDXRCommandList()->DispatchRays(&dispatchDesc);
    }
} // namespace Framework::DX
