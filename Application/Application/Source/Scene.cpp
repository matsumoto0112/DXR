#include "Scene.h"
#include "DX/Raytracing/Helper.h"
#include "Utility/Debug.h"
#include "Utility/IO/GLBLoader.h"
#include "Utility/Path.h"
#include "Utility/StringUtil.h"

#include "CompiledShaders/Miss.hlsl.h"
#include "CompiledShaders/Normal.hlsl.h"
#include "CompiledShaders/RayGenShader.hlsl.h"

#include "Application/Assets/Shader/Raytracing/Util/MissCompat.h"
#include "Application/Assets/Shader/Raytracing/Util/HitGroupCompat.h"

using namespace Framework::DX;
using namespace Framework::Utility;

namespace {
    static const std::wstring MISS_SHADER_NAME = L"Miss";
    static const std::wstring CLOSEST_HIT_NAME = L"Normal";
    static const std::wstring RAY_GEN_NAME = L"RayGenShader";
    static const std::wstring HIT_GROUP_NAME = L"HitGroup";

    static const std::unordered_map<BottomLevelASType::MyEnum, std::string> MODEL_NAMES =
    {
        {BottomLevelASType::WaterTower , "sphere.glb" },
    };

    static constexpr UINT TRIANGLE_COUNT = 1;
    static constexpr UINT QUAD_COUNT = 1;
    static constexpr UINT TLAS_NUM = TRIANGLE_COUNT + QUAD_COUNT;

    auto createBuffer = [](ID3D12Device* device, ID3D12Resource** resource, void* data, UINT64 size, LPCWSTR name = nullptr) {
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
        MY_THROW_IF_FAILED(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(resource)));
        MY_THROW_IF_FAILED((*resource)->SetName(name));
        void* mapped;
        MY_THROW_IF_FAILED((*resource)->Map(0, nullptr, &mapped));
        memcpy(mapped, data, size);
        (*resource)->Unmap(0, nullptr);
    };

    template <class T>
    inline std::vector<T> toLinearList(const std::vector<std::vector<T>>& list) {
        std::vector<T> res;
        for (size_t i = 0; i < list.size(); i++) {
            res.insert(res.end(), list[i].begin(), list[i].end());
        }
        return res;
    }

    inline std::vector<Vertex> toLinearVertices(
        const std::vector<std::vector<Framework::Math::Vector3>>& positions,
        const std::vector<std::vector<Framework::Math::Vector3>>& normals = {},
        const std::vector<std::vector<Framework::Math::Vector2>>& uvs = {}) {
        std::vector<Vertex> res;
        for (size_t i = 0; i < positions.size(); i++) {
            for (size_t j = 0; j < positions[i].size(); j++) {
                Vertex v;
                v.position = positions[i][j];
                v.normal = normals.empty() ? Vec3(0, 0, 0) : normals[i][j];
                v.uv = uvs.empty() ? Vec2(0, 0) : uvs[i][j];
                res.emplace_back(v);
            }
        }
        return res;
    }
}

Scene::Scene(Framework::DX::DeviceResource* device, UINT width, UINT height)
    :mDeviceResource(device),
    mDXRDevice(std::make_unique<DXRDevice>(mDeviceResource)),
    mWidth(width),
    mHeight(height) {
    mDebugWindow = std::make_unique<Framework::ImGUI::Window>("Debug");
    mFPSText = std::make_shared<Framework::ImGUI::Text>("FPS:");
    mDebugWindow->addItem(mFPSText);
    mDescriptorTable = std::make_unique<DescriptorTable>();
    mCameraPosition = Vec3(0, 0, -10);
    mLightPosition = Vec3(0, 100, -100);
    mLightDiffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    mLightAmbient = Color4(0.3f, 0.3f, 0.3f, 1.0f);

#define CAMERA_POSITION_PARAMS(name,type,min,max) {\
        std::shared_ptr<Framework::ImGUI::FloatField> field = \
            std::make_shared<Framework::ImGUI::FloatField>(name,type); \
        field ->setCallBack([&](float val){type = val; }); \
        field ->setMinValue(min); \
        field->setMaxValue(max); \
        mDebugWindow->addItem(field); \
    }
#define CAMERA_ROTATION_PARAMS(name,type,min,max) {\
        std::shared_ptr<Framework::ImGUI::FloatField> field = \
            std::make_shared<Framework::ImGUI::FloatField>(name,type); \
        field ->setCallBack([&](float val){type = (float)Rad(Deg(val)); }); \
        field ->setMinValue(min); \
        field->setMaxValue(max); \
        mDebugWindow->addItem(field); \
    }
    mDebugWindow->addItem(std::make_shared<Framework::ImGUI::Text>("Camera"));
    mDebugWindow->addItem(std::make_shared<Framework::ImGUI::Text>("Position"));
    CAMERA_POSITION_PARAMS("X", mCameraPosition.x, -100.0f, 100.0f);
    CAMERA_POSITION_PARAMS("Y", mCameraPosition.y, -100.0f, 100.0f);
    CAMERA_POSITION_PARAMS("Z", mCameraPosition.z, -100.0f, 100.0f);
    mDebugWindow->addItem(std::make_shared<Framework::ImGUI::Text>("Rotation"));
    CAMERA_ROTATION_PARAMS("RX", mCameraRotation.x, 0.0f, 360.0f);
    CAMERA_ROTATION_PARAMS("RY", mCameraRotation.y, 0.0f, 360.0f);
    CAMERA_ROTATION_PARAMS("RZ", mCameraRotation.z, 0.0f, 360.0f);
    mDebugWindow->addItem(std::make_shared<Framework::ImGUI::Text>("LightPosition"));
    CAMERA_POSITION_PARAMS("LX", mLightPosition.x, -100.0f, 100.0f);
    CAMERA_POSITION_PARAMS("LY", mLightPosition.y, -100.0f, 100.0f);
    CAMERA_POSITION_PARAMS("LZ", mLightPosition.z, -100.0f, 100.0f);
}

Scene::~Scene() { }

void Scene::create() {
    //最初にDXR用インターフェースを作成する
    mDXRDevice->recreate();
    //おまけ的なものを先に作る
    {
        mGpuTimer.storeDevice(mDeviceResource->getDevice(), mDeviceResource->getCommandQueue(), mDeviceResource->getBackBufferCount());
    }
    //ルートシグネチャを作成する
    {
        auto serializeAndCreateRootSignature = [&](ID3D12Device* device,
            CD3DX12_ROOT_SIGNATURE_DESC& desc, ComPtr<ID3D12RootSignature>* rootSig) {
                ComPtr<ID3DBlob> blob, error;
                MY_THROW_IF_FAILED_LOG(D3D12SerializeRootSignature(&desc,
                    D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1,
                    &blob, &error),
                    error ? toString(static_cast<wchar_t*>(error->GetBufferPointer())) : "");
                MY_THROW_IF_FAILED(device->CreateRootSignature(
                    1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&(*rootSig))));
        };
        //グローバルルートシグネチャをまずは作る
        {
            CD3DX12_DESCRIPTOR_RANGE ranges[3];
            ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0); //レンダーターゲット
            ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); //インデックスバッファ
            ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); //頂点バッファ

            CD3DX12_ROOT_PARAMETER rootParams[GlobalRootSignature::Slot::Count];
            rootParams[GlobalRootSignature::Slot::RenderTarget].InitAsDescriptorTable(1, &ranges[0]);
            rootParams[GlobalRootSignature::Slot::AccelerationStructure].InitAsShaderResourceView(0);
            rootParams[GlobalRootSignature::Slot::SceneConstant].InitAsConstantBufferView(0);
            rootParams[GlobalRootSignature::Slot::IndexBuffer].InitAsDescriptorTable(1, &ranges[1]);
            rootParams[GlobalRootSignature::Slot::VertexBuffer].InitAsDescriptorTable(1, &ranges[2]);

            CD3DX12_ROOT_SIGNATURE_DESC global(_countof(rootParams), rootParams);
            serializeAndCreateRootSignature(mDeviceResource->getDevice(),
                global, &mGlobalRootSignature);
        }
        //ローカルルートシグネチャを作る
        {
            //Missシェーダー用ローカルルートシグネチャ
            {
                CD3DX12_ROOT_PARAMETER rootParams[LocalRootSignature::Miss::Count];
                rootParams[0].InitAsConstants(align(sizeof(MissConstant), 32), 1);

                CD3DX12_ROOT_SIGNATURE_DESC local(_countof(rootParams), rootParams);
                local.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
                serializeAndCreateRootSignature(mDeviceResource->getDevice(),
                    local, &mMissLocalRootSignature);
            }
            //HitGroupシェーダー用ローカルルートシグネチャ
            {
                CD3DX12_ROOT_PARAMETER params[LocalRootSignature::HitGroup::Count];
                params[0].InitAsConstants(align(sizeof(HitGroupConstant), 32), 1);

                CD3DX12_ROOT_SIGNATURE_DESC local(_countof(params), params);
                local.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
                serializeAndCreateRootSignature(mDeviceResource->getDevice(),
                    local, &mHitGroupLocalRootSignature[LocalRootSignature::HitGroup::Normal]);
            }
        }
    }
    //パイプライン作成
    {
        CD3DX12_STATE_OBJECT_DESC pipeline{ D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
        //すべてのシェーダーを読み込んであげる
        {
            {
                CD3DX12_DXIL_LIBRARY_SUBOBJECT* lib = pipeline.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
                lib->SetDXILLibrary(&CD3DX12_SHADER_BYTECODE((void*)g_pMiss, _countof(g_pMiss)));
                lib->DefineExport(MISS_SHADER_NAME.c_str());
            }
            {
                CD3DX12_DXIL_LIBRARY_SUBOBJECT* lib = pipeline.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
                lib->SetDXILLibrary(&CD3DX12_SHADER_BYTECODE((void*)g_pNormal, _countof(g_pNormal)));
                lib->DefineExport(CLOSEST_HIT_NAME.c_str());
            }
            {
                CD3DX12_DXIL_LIBRARY_SUBOBJECT* lib = pipeline.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
                lib->SetDXILLibrary(&CD3DX12_SHADER_BYTECODE((void*)g_pRayGenShader, _countof(g_pRayGenShader)));
                lib->DefineExport(RAY_GEN_NAME.c_str());
            }
        }
        //HitGroupをまとめる
        {
            CD3DX12_HIT_GROUP_SUBOBJECT* hitGroup = pipeline.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
            hitGroup->SetClosestHitShaderImport(CLOSEST_HIT_NAME.c_str());
            hitGroup->SetHitGroupExport(HIT_GROUP_NAME.c_str());
            hitGroup->SetHitGroupType(D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES);
        }
        //シェーダーコンフィグ
        {
            CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT* config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
            UINT payloadSize = Framework::Math::MathUtil::mymax({ UINT(sizeof(RayPayload)) });
            UINT attrSize = sizeof(float) * 2;
            config->Config(payloadSize, attrSize);
        }
        //ローカルルートシグネチャの設定
        {
            //Missシェーダー
            {
                CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT* local = pipeline.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
                local->SetRootSignature(mMissLocalRootSignature.Get());

                CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT* asso = pipeline.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
                asso->SetSubobjectToAssociate(*local);
                asso->AddExport(MISS_SHADER_NAME.c_str());
            }
            //HitGroupシェーダー
            {
                CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT* local = pipeline.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
                local->SetRootSignature(mHitGroupLocalRootSignature[LocalRootSignature::HitGroup::Normal].Get());

                CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT* asso = pipeline.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
                asso->SetSubobjectToAssociate(*local);
                asso->AddExport(HIT_GROUP_NAME.c_str());
            }
        }
        //グローバルルートシグネチャの設定
        {
            CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT* global = pipeline.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
            global->SetRootSignature(mGlobalRootSignature.Get());
        }
        //パイプラインコンフィグ
        {
            CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT* config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
            config->Config(MAX_RECURSION_NUM);
        }

        PrintStateObjectDesc(pipeline);
        MY_THROW_IF_FAILED(mDXRDevice->getDXRDevice()->CreateStateObject(pipeline, IID_PPV_ARGS(&mDXRStateObject)));
    }
    //ヒープの確保
    {
        mDescriptorTable->create(mDeviceResource->getDevice(), HeapType::CBV_SRV_UAV, HeapFlag::ShaderVisible, 10000, L"DescriptorTable");
    }
    //ジオメトリを生成する
    {
        //BLAS構築用のバッファ
        std::array<Buffer, BottomLevelASType::Count> mIndexBuffer;
        std::array<Buffer, BottomLevelASType::Count> mVertexBuffer;

        std::vector<Vertex> resourceVertices;
        std::vector<Index> resourceIndices;
        {
            auto path = Framework::Utility::ExePath::getInstance()->exe();
            path = path.remove_filename();
            auto resourcePath = Framework::Utility::toString(path.append("Resources\\Model\\").c_str());
           //三角形のバッファ作成
            {
                Framework::Utility::GLBLoader loader(resourcePath + MODEL_NAMES.at(BottomLevelASType::WaterTower));
                auto indices = toLinearList(loader.getIndicesPerSubMeshes());
                auto vertices = toLinearVertices(loader.getPositionsPerSubMeshes(), loader.getNormalsPerSubMeshes(), loader.getUVsPerSubMeshes());
                createBuffer(mDeviceResource->getDevice(), &mIndexBuffer[BottomLevelASType::WaterTower].resource, indices.data(), indices.size() * sizeof(indices[0]), L"IndexBuffer");
                createBuffer(mDeviceResource->getDevice(), &mVertexBuffer[BottomLevelASType::WaterTower].resource, vertices.data(), vertices.size() * sizeof(vertices[0]), L"VertexBuffer");

                resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
                resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
            }
            //四角形のバッファ作成
            {
                std::vector<Index> indices = { 0,1,2,0,2,3 };
                createBuffer(mDeviceResource->getDevice(), &mIndexBuffer[BottomLevelASType::Quad].resource, indices.data(), indices.size() * sizeof(indices[0]), L"IndexBuffer");
                std::vector<Vertex> vertices =
                {
                    { Vec3(-1,1,0) },
                    { Vec3(1,1,0) },
                    { Vec3(1,-1,0) },
                    { Vec3(-1,-1,0) },
                };
                createBuffer(mDeviceResource->getDevice(), &mVertexBuffer[BottomLevelASType::Quad].resource, vertices.data(), vertices.size() * sizeof(vertices[0]), L"VertexBuffer");

                resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
                resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
            }
        }
        //BLAS・TLASの構築
        {
            auto allocateUAVBuffer = [](ID3D12Device* device, UINT64 bufferSize, ID3D12Resource** resource, D3D12_RESOURCE_STATES initResourceState, LPCWSTR name = nullptr) {
                auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
                auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
                MY_THROW_IF_FAILED(device->CreateCommittedResource(
                    &heapProp,
                    D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                    &bufferDesc,
                    initResourceState,
                    nullptr,
                    IID_PPV_ARGS(resource)));
                (*resource)->SetName(name);
            };

            ID3D12Device* device = mDeviceResource->getDevice();
            ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
            ID3D12CommandAllocator* allocator = mDeviceResource->getCommandAllocator();
            ID3D12Device5* dxrDevice = mDXRDevice->getDXRDevice();
            ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice->getDXRCommandList();

            commandList->Reset(allocator, nullptr);

            //BLAS
            {

                //BLAS用のジオメトリディスク作成
                for (int i = 0; i < BottomLevelASType::Count; i++) {
                    D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
                    geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
                    geometryDesc.Triangles.IndexBuffer = mIndexBuffer[i].resource->GetGPUVirtualAddress();
                    geometryDesc.Triangles.IndexCount = static_cast<UINT>(mIndexBuffer[i].resource->GetDesc().Width) / sizeof(Index);
                    geometryDesc.Triangles.IndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
                    geometryDesc.Triangles.Transform3x4 = 0;
                    geometryDesc.Triangles.VertexBuffer.StartAddress = mVertexBuffer[i].resource->GetGPUVirtualAddress();
                    geometryDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(Vertex);
                    geometryDesc.Triangles.VertexCount = static_cast<UINT>(mVertexBuffer[i].resource->GetDesc().Width) / sizeof(Vertex);
                    geometryDesc.Triangles.VertexFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
                    geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

                    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

                    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC bottomLevelBuildDesc = {};
                    auto& bottomLevelInputs = bottomLevelBuildDesc.Inputs;
                    bottomLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
                    bottomLevelInputs.Flags = buildFlags;
                    bottomLevelInputs.NumDescs = 1;
                    bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
                    bottomLevelInputs.pGeometryDescs = &geometryDesc;

                    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelPreInfo = {};
                    dxrDevice->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &bottomLevelPreInfo);
                    MY_THROW_IF_FALSE(bottomLevelPreInfo.ResultDataMaxSizeInBytes > 0);

                    allocateUAVBuffer(device, bottomLevelPreInfo.ScratchDataSizeInBytes,
                        &mBLASBuffers[i].scratch, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"ScratchResource");

                    D3D12_RESOURCE_STATES initResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
                    allocateUAVBuffer(device, bottomLevelPreInfo.ResultDataMaxSizeInBytes, &mBLASBuffers[i].buffer, initResourceState, L"BottomLevelAS");

                    bottomLevelBuildDesc.ScratchAccelerationStructureData = mBLASBuffers[i].scratch->GetGPUVirtualAddress();
                    bottomLevelBuildDesc.DestAccelerationStructureData = mBLASBuffers[i].buffer->GetGPUVirtualAddress();

                    dxrCommandList->BuildRaytracingAccelerationStructure(&bottomLevelBuildDesc, 0, nullptr);
                    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::UAV(mBLASBuffers[i].buffer.Get()));
                }
            }
            //TLAS
            {
                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

                D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC topLevelBuildDesc = {};
                auto& topLevelInputs = topLevelBuildDesc.Inputs;
                topLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
                topLevelInputs.Flags = buildFlags;
                topLevelInputs.NumDescs = TLAS_NUM;
                topLevelInputs.pGeometryDescs = nullptr;
                topLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO topLevelPreInfo = {};
                mDXRDevice->getDXRDevice()->GetRaytracingAccelerationStructurePrebuildInfo(&topLevelInputs, &topLevelPreInfo);
                MY_THROW_IF_FALSE(topLevelPreInfo.ResultDataMaxSizeInBytes > 0);

                allocateUAVBuffer(device, topLevelPreInfo.ScratchDataSizeInBytes,
                    &mTLASBuffer.scratch, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"ScratchResource");

                D3D12_RESOURCE_STATES initResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
                allocateUAVBuffer(device, topLevelPreInfo.ResultDataMaxSizeInBytes, &mTLASBuffer.buffer, initResourceState, L"TopLevelAS");
                ComPtr<ID3D12Resource> instanceDescs;
                std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDesc(TLAS_NUM);
                UINT offset = 0;
                for (UINT n = 0; n < TRIANGLE_COUNT; n++) {
                    XMMATRIX transform = XMMatrixScaling(1, 1, 1) *
                        XMMatrixRotationRollPitchYaw(0, 0, 0) *
                        XMMatrixTranslation((float)n * 5, 0, 0);
                    instanceDesc[n + offset].InstanceID = 0;
                    instanceDesc[n + offset].InstanceMask = 0xff;
                    instanceDesc[n + offset].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
                    instanceDesc[n + offset].InstanceContributionToHitGroupIndex = 0;
                    instanceDesc[n + offset].AccelerationStructure = mBLASBuffers[BottomLevelASType::WaterTower].buffer->GetGPUVirtualAddress();
                    XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc[n + offset].Transform), transform);
                }
                offset = TRIANGLE_COUNT;
                for (UINT n = 0; n < QUAD_COUNT; n++) {
                    XMMATRIX transform = XMMatrixScaling(1, 1, 1) * XMMatrixTranslation((float)n * 5, 3, 0);
                    instanceDesc[n + offset].InstanceID = 0;
                    instanceDesc[n + offset].InstanceMask = 0xff;
                    instanceDesc[n + offset].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
                    instanceDesc[n + offset].InstanceContributionToHitGroupIndex = 0;
                    instanceDesc[n + offset].AccelerationStructure = mBLASBuffers[BottomLevelASType::Quad].buffer->GetGPUVirtualAddress();
                    XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc[n + offset].Transform), transform);
                }

                createBuffer(device, &instanceDescs, &instanceDesc[0], instanceDesc.size() * sizeof(D3D12_RAYTRACING_INSTANCE_DESC), L"InstanceDescs");
                topLevelBuildDesc.DestAccelerationStructureData = mTLASBuffer.buffer->GetGPUVirtualAddress();
                topLevelBuildDesc.ScratchAccelerationStructureData = mTLASBuffer.scratch->GetGPUVirtualAddress();
                topLevelInputs.InstanceDescs = instanceDescs->GetGPUVirtualAddress();

                dxrCommandList->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);
                mDeviceResource->executeCommandList();
                mDeviceResource->waitForGPU();
            }

            //バッファのシェーダーリソースビュー作成
            createBuffer(device, &mResourcesIndexBuffer.resource, resourceIndices.data(), resourceIndices.size() * sizeof(resourceIndices[0]), L"ResourceIndex");
            createBuffer(device, &mResourcesVertexBuffer.resource, resourceVertices.data(), resourceVertices.size() * sizeof(resourceVertices[0]), L"ResourceVertex");

            //インデックスバッファのリソースビュー作成
            {
                UINT indexBufferSize = (UINT)resourceIndices.size() * sizeof(resourceIndices[0]) / sizeof(float);
                D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srvDesc.Buffer.NumElements = indexBufferSize;
                srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
                srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
                srvDesc.Buffer.StructureByteStride = 0;
                mResourcesIndexBuffer.cpuHandle = mDescriptorTable->getCPUHandle(DescriptorIndex::IndexBuffer);
                mResourcesIndexBuffer.gpuHandle = mDescriptorTable->getGPUHandle(DescriptorIndex::IndexBuffer);
                device->CreateShaderResourceView(mResourcesIndexBuffer.resource.Get(), &srvDesc, mResourcesIndexBuffer.cpuHandle);
            }
            //頂点バッファのリソースビュー作成
            {
                UINT vertexBufferSize = (UINT)resourceVertices.size();
                D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srvDesc.Buffer.NumElements = vertexBufferSize;
                srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
                srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
                srvDesc.Buffer.StructureByteStride = sizeof(resourceVertices[0]);
                mResourcesVertexBuffer.cpuHandle = mDescriptorTable->getCPUHandle(DescriptorIndex::VertexBuffer);
                mResourcesVertexBuffer.gpuHandle = mDescriptorTable->getGPUHandle(DescriptorIndex::VertexBuffer);
                device->CreateShaderResourceView(mResourcesVertexBuffer.resource.Get(), &srvDesc, mResourcesVertexBuffer.cpuHandle);
            }
        }
    }
    //シェーダーテーブルを構築する
    {
        ID3D12Device* device = mDeviceResource->getDevice();
        ComPtr<ID3D12StateObjectProperties> stateObjectProp;
        MY_THROW_IF_FAILED(mDXRStateObject.As(&stateObjectProp));
        void* rayGenShaderID = stateObjectProp->GetShaderIdentifier(RAY_GEN_NAME.c_str());
        void* missShaderID = stateObjectProp->GetShaderIdentifier(MISS_SHADER_NAME.c_str());
        void* hitGroupShaderID = stateObjectProp->GetShaderIdentifier(HIT_GROUP_NAME.c_str());
        UINT shaderIDSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
        //RayGenShader
        {
            UINT num = 1;
            UINT recordSize = shaderIDSize;
            ShaderTable table(device, num, recordSize, L"RayGenShaderTable");
            table.push_back(ShaderRecord(rayGenShaderID, shaderIDSize));
            mRayGenTable = table.getResource();
        }
        //MissShader
        {
            struct RootArgument {
                MissConstant cb;
            } rootArgument;
            rootArgument.cb.back = Color(1, 1, 1, 1);
            UINT num = 1;
            UINT recordSize = shaderIDSize + sizeof(RootArgument);
            ShaderTable table(device, num, recordSize, L"MissShaderTable");
            table.push_back(ShaderRecord(missShaderID, shaderIDSize, &rootArgument, sizeof(RootArgument)));
            mMissTable = table.getResource();
        }
        //HitGroup
        {
            struct RootArgument {
                HitGroupConstant cb;
            } rootArguments;
            rootArguments.cb.color = Color4(1, 1, 0, 1);
            UINT num = 1;
            UINT recordSize = shaderIDSize + sizeof(RootArgument);
            ShaderTable table(device, num, recordSize, L"HitGroupShaderTable");
            table.push_back(ShaderRecord(hitGroupShaderID, shaderIDSize, &rootArguments, sizeof(rootArguments)));
            mHitGroupTable = table.getResource();
        }
    }
    //出力先のレンダーターゲット作成
    {
        ID3D12Device* device = mDeviceResource->getDevice();
        DXGI_FORMAT backBufferFormat = mDeviceResource->getBackBufferFormat();

        auto uavResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(backBufferFormat,
            mWidth, mHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

        auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
        MY_THROW_IF_FAILED(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &uavResourceDesc,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr,
            IID_PPV_ARGS(&mRaytracingOutput.resource)));
        mRaytracingOutput.resource->SetName(L"RaytracingOutput");

        mRaytracingOutput.cpuHandle = mDescriptorTable->getCPUHandle(DescriptorIndex::RaytracingOutput);
        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
        device->CreateUnorderedAccessView(mRaytracingOutput.resource.Get(), nullptr, &uavDesc, mRaytracingOutput.cpuHandle);
        mRaytracingOutput.gpuHandle = mDescriptorTable->getGPUHandle(DescriptorIndex::RaytracingOutput);
    }
    //コンスタントバッファの初期化
    {
        ID3D12Device* device = mDeviceResource->getDevice();
        UINT backBufferCount = mDeviceResource->getBackBufferCount();
        mSceneCB.create(device, backBufferCount, L"SceneConstantBuffer");
    }
}

void Scene::reset() {
    mDXRDevice->reset();
    mGpuTimer.reset();
    mGpuTimer.releaseDevice();
}

void Scene::update() {
    mTime.update();
    mFPSText->setText(format("FPS:%0.3f", mTime.getFPS()));

    mSceneCB->cameraPosition = Vec4(mCameraPosition, 1.0f);
    const float aspect = static_cast<float>(mWidth) / static_cast<float>(mHeight);

    Mat4 view =
        Mat4::createRotation(mCameraRotation) *
        Mat4::createTranslate(mCameraPosition);
    view = view.inverse();
    Mat4 proj =
        Mat4::createProjection(Deg(45.0f), aspect, 0.1f, 100.0f);
    Mat4 vp = view * proj;
    mSceneCB->projectionToWorld = vp.inverse();
    mSceneCB->lightPosition = Vec4(mLightPosition, 1.0f);
    mSceneCB->lightDiffuse = mLightDiffuse;
    mSceneCB->lightAmbient = mLightAmbient;
}

void Scene::render() {
    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature.Get());
    mSceneCB.copyStatingToGPU(frameIndex);

    ID3D12DescriptorHeap* heaps[] = { mDescriptorTable->getHeap() };
    commandList->SetDescriptorHeaps(1, heaps);
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::RenderTarget, mRaytracingOutput.gpuHandle);
    commandList->SetComputeRootShaderResourceView(GlobalRootSignature::Slot::AccelerationStructure, mTLASBuffer.buffer->GetGPUVirtualAddress());
    commandList->SetComputeRootConstantBufferView(GlobalRootSignature::Slot::SceneConstant, mSceneCB.gpuVirtualAddress());
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::IndexBuffer, mResourcesIndexBuffer.gpuHandle);
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::VertexBuffer, mResourcesVertexBuffer.gpuHandle);

    D3D12_DISPATCH_RAYS_DESC dispatchDesc = {};
    dispatchDesc.RayGenerationShaderRecord.StartAddress = mRayGenTable->GetGPUVirtualAddress();
    dispatchDesc.RayGenerationShaderRecord.SizeInBytes = mRayGenTable->GetDesc().Width;

    dispatchDesc.MissShaderTable.StartAddress = mMissTable->GetGPUVirtualAddress();
    dispatchDesc.MissShaderTable.SizeInBytes = mMissTable->GetDesc().Width;
    dispatchDesc.MissShaderTable.StrideInBytes = dispatchDesc.MissShaderTable.SizeInBytes;

    dispatchDesc.HitGroupTable.StartAddress = mHitGroupTable->GetGPUVirtualAddress();
    dispatchDesc.HitGroupTable.SizeInBytes = mHitGroupTable->GetDesc().Width;
    dispatchDesc.HitGroupTable.StrideInBytes = dispatchDesc.HitGroupTable.SizeInBytes;

    dispatchDesc.Width = mWidth;
    dispatchDesc.Height = mHeight;
    dispatchDesc.Depth = 1;

    mDXRDevice->getDXRCommandList()->SetPipelineState1(mDXRStateObject.Get());
    mDXRDevice->getDXRCommandList()->DispatchRays(&dispatchDesc);

    D3D12_RESOURCE_BARRIER preCopyBarriers[2];
    //バックバッファのバリア
    preCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(mDeviceResource->getRenderTarget(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
    //レイトレのレンダーターゲット
    preCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.resource.Get(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->ResourceBarrier(_countof(preCopyBarriers), preCopyBarriers);

    commandList->CopyResource(mDeviceResource->getRenderTarget(), mRaytracingOutput.resource.Get());

    D3D12_RESOURCE_BARRIER postCopyBarriers[2];
    postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(mDeviceResource->getRenderTarget(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
    postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.resource.Get(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    commandList->ResourceBarrier(_countof(postCopyBarriers), postCopyBarriers);

    mDebugWindow->draw();
}
