#include "Scene.h"
#include "Utility/StringUtil.h"
#include "Utility/Debug.h"
#include "DX/Raytracing/Helper.h"


#include "CompiledShaders/Miss.hlsl.h"
#include "CompiledShaders/Normal.hlsl.h"
#include "CompiledShaders/RayGenShader.hlsl.h"

#include "Application/Assets/Shader/Raytracing/Util/Compat.h"

using namespace Framework::DX;
using namespace Framework::Utility;

namespace {
    static const std::wstring MISS_SHADER_NAME = L"Miss";
    static const std::wstring CLOSEST_HIT_NAME = L"Normal";
    static const std::wstring RAY_GEN_NAME = L"RayGenShader";
    static const std::wstring HIT_GROUP_NAME = L"HitGroup";

    static constexpr UINT TLAS_NUM = 3;

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
            CD3DX12_DESCRIPTOR_RANGE ranges[1];
            ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0); //レンダーターゲット

            CD3DX12_ROOT_PARAMETER rootParams[GlobalRootSignature::Slot::Count];
            rootParams[GlobalRootSignature::Slot::RenderTarget].InitAsDescriptorTable(1, &ranges[0]);
            rootParams[GlobalRootSignature::Slot::AccelerationStructure].InitAsShaderResourceView(0);
            rootParams[GlobalRootSignature::Slot::SceneConstant].InitAsConstantBufferView(0);

            CD3DX12_ROOT_SIGNATURE_DESC global(_countof(rootParams), rootParams);
            serializeAndCreateRootSignature(mDeviceResource->getDevice(),
                global, &mGlobalRootSignature);
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
        Buffer mIndexBuffer;
        Buffer mVertexBuffer;
        {
            //三角形のバッファ作成
            {
                std::vector<Index> indices = { 0,1,2 };
                createBuffer(mDeviceResource->getDevice(), &mIndexBuffer.resource, indices.data(), indices.size() * sizeof(indices[0]), L"IndexBuffer");
                std::vector<Vertex> vertices =
                {
                    { XMFLOAT3(0,0,0) },
                    { XMFLOAT3(1,-1,0) },
                    { XMFLOAT3(-1,-1,0) },
                };
                createBuffer(mDeviceResource->getDevice(), &mVertexBuffer.resource, vertices.data(), vertices.size() * sizeof(vertices[0]), L"VertexBuffer");
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

            //BLAS用のジオメトリディスク作成
            D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
            geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
            geometryDesc.Triangles.IndexBuffer = mIndexBuffer.resource->GetGPUVirtualAddress();
            geometryDesc.Triangles.IndexCount = static_cast<UINT>(mIndexBuffer.resource->GetDesc().Width) / sizeof(Index);
            geometryDesc.Triangles.IndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
            geometryDesc.Triangles.Transform3x4 = 0;
            geometryDesc.Triangles.VertexBuffer.StartAddress = mVertexBuffer.resource->GetGPUVirtualAddress();
            geometryDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(Vertex);
            geometryDesc.Triangles.VertexCount = static_cast<UINT>(mVertexBuffer.resource->GetDesc().Width) / sizeof(Vertex);
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

            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelPreInfo = {};
            dxrDevice->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &bottomLevelPreInfo);
            MY_THROW_IF_FALSE(bottomLevelPreInfo.ResultDataMaxSizeInBytes > 0);

            ComPtr<ID3D12Resource> scratchResource;
            allocateUAVBuffer(device, Framework::Math::MathUtil::mymax(topLevelPreInfo.ScratchDataSizeInBytes, bottomLevelPreInfo.ScratchDataSizeInBytes),
                &scratchResource, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, L"ScratchResource");

            {
                D3D12_RESOURCE_STATES initResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
                allocateUAVBuffer(device, bottomLevelPreInfo.ResultDataMaxSizeInBytes, &mBLASBuffer, initResourceState, L"BottomLevelAS");
                allocateUAVBuffer(device, topLevelPreInfo.ResultDataMaxSizeInBytes, &mTLASBuffer, initResourceState, L"TopLevelAS");
            }

            ComPtr<ID3D12Resource> instanceDescs;
            std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDesc(TLAS_NUM);
            for (UINT n = 0; n < TLAS_NUM; n++) {
                XMMATRIX transform = XMMatrixScaling(3, 3, 1) * XMMatrixTranslation((float)n * 3, 0, 0);
                instanceDesc[n].InstanceID = 0;
                instanceDesc[n].InstanceMask = 0xff;
                instanceDesc[n].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
                instanceDesc[n].InstanceContributionToHitGroupIndex = 0;
                instanceDesc[n].AccelerationStructure = mBLASBuffer->GetGPUVirtualAddress();
                XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc[n].Transform), transform);
            }
            createBuffer(device, &instanceDescs, &instanceDesc[0], instanceDesc.size() * sizeof(D3D12_RAYTRACING_INSTANCE_DESC), L"InstanceDescs");
            {
                bottomLevelBuildDesc.ScratchAccelerationStructureData = scratchResource->GetGPUVirtualAddress();
                bottomLevelBuildDesc.DestAccelerationStructureData = mBLASBuffer->GetGPUVirtualAddress();
            }
            {
                topLevelBuildDesc.DestAccelerationStructureData = mTLASBuffer->GetGPUVirtualAddress();
                topLevelBuildDesc.ScratchAccelerationStructureData = scratchResource->GetGPUVirtualAddress();
                topLevelInputs.InstanceDescs = instanceDescs->GetGPUVirtualAddress();
            }

            dxrCommandList->BuildRaytracingAccelerationStructure(&bottomLevelBuildDesc, 0, nullptr);
            commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::UAV(mBLASBuffer.Get()));
            dxrCommandList->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);

            mDeviceResource->executeCommandList();
            mDeviceResource->waitForGPU();
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
            UINT num = 1;
            UINT recordSize = shaderIDSize;
            ShaderTable table(device, num, recordSize, L"MissShaderTable");
            table.push_back(ShaderRecord(missShaderID, shaderIDSize));
            mMissTable = table.getResource();
        }
        //HitGroup
        {
            UINT num = 1;
            UINT recordSize = shaderIDSize;
            ShaderTable table(device, num, recordSize, L"HitGroupShaderTable");
            table.push_back(ShaderRecord(hitGroupShaderID, shaderIDSize));
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

    mSceneCB->cameraPosition = { 0,0,-10,1 };
    XMMATRIX view = XMMatrixLookAtLH({ 0,0,-10 }, { 0,0,0 }, { 0,1,0 });
    const float aspect = static_cast<float>(mWidth) / static_cast<float>(mHeight);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToDegrees(45.0f), aspect, 0.1f, 100.0f);
    XMMATRIX vp = view * proj;
    mSceneCB->projectionToWorld = XMMatrixInverse(nullptr, vp);
}

void Scene::render() {
    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature.Get());
    mSceneCB.copyStatingToGPU(frameIndex);

    ID3D12DescriptorHeap* heaps[] = { mDescriptorTable->getHeap() };
    commandList->SetDescriptorHeaps(1, heaps);
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::RenderTarget, mRaytracingOutput.gpuHandle);
    commandList->SetComputeRootConstantBufferView(GlobalRootSignature::Slot::SceneConstant, mSceneCB.gpuVirtualAddress());
    commandList->SetComputeRootShaderResourceView(GlobalRootSignature::Slot::AccelerationStructure, mTLASBuffer->GetGPUVirtualAddress());

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
