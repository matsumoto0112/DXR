#include "Scene.h"
#include <DirectXMath.h>
#include <numeric>
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Raytracing/Shader/ShaderTable.h"
#include "DX/Util/Helper.h"
#include "ImGui/ImGuiManager.h"
#include "Math/Quaternion.h"
#include "Model.h"
#include "Utility/Debug.h"
#include "Utility/IO/GLBLoader.h"
#include "Utility/IO/TextureLoader.h"
#include "Utility/Path.h"
#include "Utility/StringUtil.h"

#include "CompiledShaders/ClosestHit_Normal.hlsl.h"
#include "CompiledShaders/ClosestHit_Plane.hlsl.h"
#include "CompiledShaders/ClosestHit_Sphere.hlsl.h"
#include "CompiledShaders/Miss.hlsl.h"
#include "CompiledShaders/RayGenShader.hlsl.h"
#include "CompiledShaders/Shadow.hlsl.h"

#include "Assets/Shader/Raytracing/Util/HitGroupCompat.h"
#include "Assets/Shader/Raytracing/Util/MissCompat.h"

using namespace Framework::DX;
using namespace Framework::Desc;
using namespace Framework::Utility;
using namespace Framework::Math;
using namespace DirectX;

namespace {
    //シェーダーのローカル情報を設定するためのキー
    namespace ShaderKey {
        enum MyEnum {
            RayGenShader,
            MissShader,
            MissShadowShader,
            HitGroup_UFO,
            HitGroup_Floor,
            HitGroup_Sphere,
        };
    }

    struct ExportShaderInfo {
        std::wstring name;
        ShaderType type;
        const void* shader;
        UINT shaderSize;
    };

    static const std::vector<ExportShaderInfo> EXPORT_SHADER_LIST{
        { L"RayGenShader", ShaderType::RayGeneration, g_pRayGenShader, _countof(g_pRayGenShader) },
        { L"Miss", ShaderType::Miss, g_pMiss, _countof(g_pMiss) },
        { L"Shadow", ShaderType::Miss, g_pShadow, _countof(g_pShadow) },
        { L"ClosestHit_Normal", ShaderType::HitGroup, g_pClosestHit_Normal,
            _countof(g_pClosestHit_Normal) },
        { L"ClosestHit_Plane", ShaderType::HitGroup, g_pClosestHit_Plane,
            _countof(g_pClosestHit_Plane) },
        { L"ClosestHit_Sphere", ShaderType::HitGroup, g_pClosestHit_Sphere,
            _countof(g_pClosestHit_Sphere) },
    };

    struct HitGroup {
        UINT shaderKey;
        std::wstring hitGroupName;
        std::wstring closestHitNames;
    };

    static const std::vector<HitGroup> HIT_GROUP_LIST = {
        { ShaderKey::HitGroup_UFO, L"HitGroup_UFO", L"ClosestHit_Normal" },
        { ShaderKey::HitGroup_Floor, L"HitGroup_Floor", L"ClosestHit_Plane" },
        { ShaderKey::HitGroup_Sphere, L"HitGroup_Sphere", L"ClosestHit_Sphere" },
    };

    struct ShaderTableInfo {
        std::wstring name;
        ShaderKey::MyEnum key;
        ShaderType type;
    };
    static const std::vector<ShaderTableInfo> SHADER_TABLE_INFO = {
        { L"RayGenShader", ShaderKey::RayGenShader, ShaderType::RayGeneration },
        { L"Miss", ShaderKey::MissShader, ShaderType::Miss },
        { L"Shadow", ShaderKey::MissShadowShader, ShaderType::Miss },
        { L"HitGroup_UFO", ShaderKey::HitGroup_UFO, ShaderType::HitGroup },
        { L"HitGroup_Floor", ShaderKey::HitGroup_Floor, ShaderType::HitGroup },
        { L"HitGroup_Sphere", ShaderKey::HitGroup_Sphere, ShaderType::HitGroup },
    };

    struct LoadModelInfo {
        std::wstring name;
        UINT shaderKey;
    };

    static const std::unordered_map<ModelType::Enum, LoadModelInfo> MODEL_NAMES = {
        { ModelType::UFO, { L"UFO.glb", ShaderKey::HitGroup_UFO } },
        { ModelType::Sphere, { L"sphere.glb", ShaderKey::HitGroup_Sphere } },
        { ModelType::Floor, { L"floor.glb", ShaderKey::HitGroup_Floor } },
    };

    static constexpr UINT UFO_COUNT = 1;
    static constexpr UINT FLOOR_COUNT = 1;
    static int SPHERE_COUNT = 100;

    std::unordered_map<ModelType::Enum, Model> mLoadedModels;

    struct Object {
        Model* model;
        UINT hitGroupIndex;
        BottomLevelAccelerationStructure* blas;
        Vec3 position;
        Quaternion rotation;
        Vec3 scale;
    };
    Object mFloor;
    std::vector<Object> mSpheres;
} // namespace

Scene::Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager,
    UINT width, UINT height)
    : mDeviceResource(device),
      mInputManager(inputManager),
      mDXRDevice(),
      mWidth(width),
      mHeight(height) {

    mCameraPosition = Vec3(0, 0, -10);
    mLightPosition = Vec3(0, 100, -100);
    mLightDiffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    mLightAmbient = Color4(0.1f, 0.1f, 0.1f, 1.0f);

    mTextures.resize(TEXTURE_NUM);
}
Scene::~Scene() {}

void Scene::create() {
    createDeviceDependentResources();
    createWindowDependentResources();
    {
        ID3D12Device* device = mDeviceResource->getDevice();
        UINT backBufferCount = mDeviceResource->getBackBufferCount();
        mSceneCB.init(mDeviceResource, L"SceneConstantBuffer");
        mSceneCB.createCBV(mDeviceResource, true);
    }
}

void Scene::reset() {
    mDXRDevice.reset();
    mGpuTimer.reset();
    mGpuTimer.releaseDevice();
}

void Scene::update() {
    mDeviceResource->getRaytracingDescriptorManager()->mHeap.resetGlobal();
    mTime.update();

#pragma region IMGUI_REGION
    ImGui::Begin("Status");
    ImGui::Text("FPS:%0.3f", mTime.getFPS());
    ImGui::End();

    ImGui::Begin("Parameter");
    {
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
        if (ImGui::TreeNode("Camera")) {
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
            if (ImGui::TreeNode("Position")) {
                ImGui::DragFloat("X", &mCameraPosition.x, 1.0f);
                ImGui::DragFloat("Y", &mCameraPosition.y, 1.0f);
                ImGui::DragFloat("Z", &mCameraPosition.z, 1.0f);
                ImGui ::TreePop();
            }
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
            if (ImGui::TreeNode("Rotation")) {
                ImGui::DragFloat("X", &mCameraRotation.x, 0.1f);
                ImGui::DragFloat("Y", &mCameraRotation.y, 0.1f);
                ImGui::DragFloat("Z", &mCameraRotation.z, 0.1f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
        if (ImGui::TreeNode("LightPosition")) {
            ImGui::DragFloat("X", &mLightPosition.x, 1.0f);
            ImGui::DragFloat("Y", &mLightPosition.y, 1.0f);
            ImGui::DragFloat("Z", &mLightPosition.z, 1.0f);
            ImGui::TreePop();
        }
        ImGui::DragInt("Count", &SPHERE_COUNT);
    }
    ImGui::End();
#pragma endregion
#pragma region CONSTANT_BUFFER_UPDATE
    mSceneCB->cameraPosition = Vec4(mCameraPosition, 1.0f);
    const float aspect = static_cast<float>(mWidth) / static_cast<float>(mHeight);

    Mat4 view = Mat4::createRotation(mCameraRotation) * Mat4::createTranslate(mCameraPosition);
    view = view.inverse();
    Mat4 proj = Mat4::createProjection(Deg(45.0f), aspect, 0.1f, 100.0f);
    Mat4 vp = view * proj;
    mSceneCB->projectionToWorld = vp.inverse();
    mSceneCB->lightPosition = Vec4(mLightPosition, 1.0f);
    mSceneCB->lightDiffuse = mLightDiffuse;
    mSceneCB->lightAmbient = mLightAmbient;
    mSceneCB->globalTime = static_cast<float>(mTime.getTime());
#pragma endregion
}

void Scene::render() {
    ID3D12Device* device = mDeviceResource->getDevice();
    ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice.getDXRCommandList();

    mTLASBuffer->clear();
    UINT offset = 0;
    TopLevelAccelerationStructure::InstanceDesc instanceDesc = {};
    instanceDesc.instanceID = 0;
    instanceDesc.mask = 0xff;
    instanceDesc.flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;

    auto createTransform = [](Object& obj) {
        return XMMatrixScaling(obj.scale.x, obj.scale.y, obj.scale.z)
            * XMMatrixRotationQuaternion(
                { obj.rotation.x, obj.rotation.y, obj.rotation.z, obj.rotation.w })
            * XMMatrixTranslation(obj.position.x, obj.position.y, obj.position.z);
    };

    auto addTLAS = [&](Object& obj) {
        XMMATRIX transform = createTransform(obj);
        instanceDesc.hitGroupIndex = obj.hitGroupIndex;
        instanceDesc.blas = obj.blas;
        instanceDesc.transform = transform;
        mTLASBuffer->add(instanceDesc);
    };

    addTLAS(mFloor);
    for (auto&& obj : mSpheres) { addTLAS(obj); }

    mTLASBuffer->build(mDXRDevice, mDeviceResource,
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE);

    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature->getRootSignature());
    mSceneCB.updateStaging();

    ID3D12DescriptorHeap* heaps[]
        = { mDeviceResource->getRaytracingDescriptorManager()->mHeap.mDescriptorHeap.Get() };
    commandList->SetDescriptorHeaps(_countof(heaps), heaps);

    DescriptorSet globalSet;
    globalSet.setCbvHandle(0, mSceneCB.getView().getInfo().cpuHandle);
    globalSet.setSrvHandle(0, mTLASBuffer->getView().getInfo().cpuHandle);
    globalSet.setSrvHandle(1, mResourceIndexBufferSRV.getInfo().cpuHandle);
    globalSet.setSrvHandle(2, mResourceVertexBufferSRV.getInfo().cpuHandle);
    globalSet.setUavHandle(0, mRaytracingOutputUAV.getInfo().cpuHandle);

    mDeviceResource->getRaytracingDescriptorManager()->copyAndSetComputeDescriptorTable(
        mDeviceResource, commandList, globalSet);
    mDXRStateObject->doRaytracing(mWidth, mHeight);

    D3D12_RESOURCE_BARRIER preCopyBarriers[2];
    preCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(mDeviceResource->getRenderTarget(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
    preCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.getResource(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->ResourceBarrier(_countof(preCopyBarriers), preCopyBarriers);

    commandList->CopyResource(mDeviceResource->getRenderTarget(), mRaytracingOutput.getResource());

    D3D12_RESOURCE_BARRIER postCopyBarriers[2];
    postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(mDeviceResource->getRenderTarget(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
    postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.getResource(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    commandList->ResourceBarrier(_countof(postCopyBarriers), postCopyBarriers);
}

void Scene::onWindowSizeChanged(UINT width, UINT height) {
    mWidth = width;
    mHeight = height;

    releaseWindowDependentResources();
    createWindowDependentResources();
}

void Scene::createDeviceDependentResources() {
    mDXRDevice.create(mDeviceResource->getDevice(), mDeviceResource->getCommandList());
    {
        mGpuTimer.storeDevice(mDeviceResource->getDevice(), mDeviceResource->getCommandQueue(),
            mDeviceResource->getBackBufferCount());
    }
    //グローバルルートシグネチャ
    {
        CD3DX12_DESCRIPTOR_RANGE ranges[GlobalRootSignature::Slot::Count];
        ranges[GlobalRootSignature::Slot::Cbv].Init(
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
            GLOBAL_CONSTANT_BUFFER_VIEW_RESGISTER_NUM, GLOBAL_CONSTANT_BUFFER_VIEW_RESGISTER_START);
        ranges[GlobalRootSignature::Slot::Srv].Init(
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
            GLOBAL_SHADER_RESOURCE_VIEW_REGISTER_NUM, GLOBAL_SHADER_RESOURCE_VIEW_REGISTER_START);
        ranges[GlobalRootSignature::Slot::Uav].Init(
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
            GLOBAL_UNORDERED_ACCESS_VIEW_REGISTER_NUM, GLOBAL_UNORDERED_ACCESS_VIEW_REGISTER_START);

        std::vector<CD3DX12_ROOT_PARAMETER> rootParams(GlobalRootSignature::Slot::Count);
        rootParams[GlobalRootSignature::Slot::Cbv].InitAsDescriptorTable(
            1, &ranges[GlobalRootSignature::Slot::Cbv]);
        rootParams[GlobalRootSignature::Slot::Srv].InitAsDescriptorTable(
            1, &ranges[GlobalRootSignature::Slot::Srv]);
        rootParams[GlobalRootSignature::Slot::Uav].InitAsDescriptorTable(
            1, &ranges[GlobalRootSignature::Slot::Uav]);

        std::vector<CD3DX12_STATIC_SAMPLER_DESC> sampler(1);
        sampler[0] = CD3DX12_STATIC_SAMPLER_DESC(
            0, D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

        RootSignatureDesc desc{ rootParams, sampler, RootSignature::Flags::None };
        mGlobalRootSignature = std::make_unique<RootSignature>(mDeviceResource->getDevice(), desc);
    }
    //ミスシェーダー
    {
        std::vector<CD3DX12_ROOT_PARAMETER> params(1);
        params[0].InitAsConstants(
            Framework::Math::MathUtil::alignPow2(sizeof(MissConstant), sizeof(UINT32)), 1);

        RootSignatureDesc desc{ params, {}, RootSignature::Flags::Local };
        mMissLocalRootSignature
            = std::make_unique<RootSignature>(mDeviceResource->getDevice(), desc);
    }
    //ヒットグループシェーダー
    {
        CD3DX12_DESCRIPTOR_RANGE range[5];
        range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
            LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START + 0);
        range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
            LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START + 1);
        range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
            LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START + 2);
        range[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
            LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START + 3);
        range[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
            LOCAL_SHADER_RESOURCE_VIEW_REGISTER_START + 4);

        std::vector<CD3DX12_ROOT_PARAMETER> params(6);
        UINT contSize
            = Framework::Math::MathUtil::alignPow2(sizeof(HitGroupConstant), sizeof(UINT32));

        params[0].InitAsDescriptorTable(1, &range[0]);
        params[1].InitAsDescriptorTable(1, &range[1]);
        params[2].InitAsDescriptorTable(1, &range[2]);
        params[3].InitAsDescriptorTable(1, &range[3]);
        params[4].InitAsDescriptorTable(1, &range[4]);

        params[5].InitAsConstants(contSize, 1);
        RootSignatureDesc desc{ params, {}, RootSignature::Flags::Local };
        mHitGroupLocalRootSignature
            = std::make_unique<RootSignature>(mDeviceResource->getDevice(), desc);
    }
    {
        ID3D12Device* device = mDeviceResource->getDevice();
        ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
        ID3D12CommandAllocator* allocator = mDeviceResource->getCommandAllocator();
        ID3D12Device5* dxrDevice = mDXRDevice.getDXRDevice();
        ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice.getDXRCommandList();

        std::vector<Framework::DX::Vertex> resourceVertices;
        std::vector<Index> resourceIndices;

        auto path = Framework::Utility::ExePath::getInstance()->exe();
        path = path.remove_filename();
        auto modelPath = path / "Resources" / "Model";
        auto texPath = path / "Resources" / "Texture";

        UINT vertexOffset = 0;
        UINT indexOffset = 0;

        auto loadModel = [&](const std::filesystem::path& filepath, UINT modelID) {
            Model model;
            model.init(mDeviceResource, commandList, filepath, modelID);
            model.mVertexOffset = vertexOffset;
            model.mIndexOffset = indexOffset;
            vertexOffset += model.mVertices.size();
            indexOffset += model.mIndices.size();
            return model;
        };

        mLoadedModels[ModelType::UFO] = loadModel(modelPath / MODEL_NAMES.at(ModelType::UFO).name,
            MODEL_NAMES.at(ModelType::UFO).shaderKey);
        mLoadedModels[ModelType::Floor]
            = loadModel(modelPath / MODEL_NAMES.at(ModelType::Floor).name,
                MODEL_NAMES.at(ModelType::Floor).shaderKey);
        mLoadedModels[ModelType::Sphere]
            = loadModel(modelPath / MODEL_NAMES.at(ModelType::Sphere).name,
                MODEL_NAMES.at(ModelType::Sphere).shaderKey);
        for (auto&& model : mLoadedModels) {
            resourceIndices.insert(
                resourceIndices.end(), model.second.mIndices.begin(), model.second.mIndices.end());
            resourceVertices.insert(resourceVertices.end(), model.second.mVertices.begin(),
                model.second.mVertices.end());
        }

        mResourcesIndexBuffer.init(mDeviceResource, resourceIndices,
            D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"ResourceIndex");
        mResourceIndexBufferSRV = mResourcesIndexBuffer.createSRV(mDeviceResource, true);

        mResourcesVertexBuffer.init(mDeviceResource, resourceVertices, L"ResourceVertex");
        mResourceVertexBufferSRV.initAsBuffer(
            mDeviceResource, mResourcesVertexBuffer.getBuffer(), true);

        for (auto&& model : mLoadedModels) {
            mBLASBuffers[model.first] = std::make_unique<BottomLevelAccelerationStructure>();
            mBLASBuffers[model.first]->init(mDXRDevice, model.second.mVertexBuffer,
                static_cast<UINT>(sizeof(Vertex)), model.second.mIndexBuffer,
                static_cast<UINT>(sizeof(Index)));
        }

        mTLASBuffer = std::make_unique<TopLevelAccelerationStructure>();

        mDeviceResource->executeCommandList();
        mDeviceResource->waitForGPU();

        mFloor.model = &mLoadedModels[ModelType::Floor];
        mFloor.hitGroupIndex = LocalRootSignature::HitGroupIndex::Floor;
        mFloor.blas = mBLASBuffers[ModelType::Floor].get();
        mFloor.position = Vec3(0, -10, 0);
        mFloor.rotation = Quaternion::IDENTITY;
        mFloor.scale = Vec3(500, 1, 500);

        UINT root = static_cast<UINT>(Framework::Math::MathUtil::sqrt(SPHERE_COUNT));
        for (UINT i = 0; i < SPHERE_COUNT; i++) {
            Object sphere;
            sphere.model = &mLoadedModels[ModelType::Sphere];
            sphere.hitGroupIndex = LocalRootSignature::HitGroupIndex::Sphere;
            sphere.blas = mBLASBuffers[ModelType::Sphere].get();
            sphere.position = Vec3((i % root) * 20.0f, 0, (i / root) * 20.0f);
            sphere.rotation = Quaternion::IDENTITY;
            sphere.scale = Vec3(1, 1, 1);
            mSpheres.emplace_back(sphere);
        }
    }

    {

        mDXRStateObject = std::make_unique<DXRPipelineStateObject>(&mDXRDevice);

        for (auto&& exportShader : EXPORT_SHADER_LIST) {
            mDXRStateObject->exportShader(
                exportShader.shader, exportShader.shaderSize, exportShader.name);
        }

        for (auto&& hitGroup : HIT_GROUP_LIST) {
            mDXRStateObject->bindHitGroup({ hitGroup.hitGroupName,
                D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, hitGroup.closestHitNames });
        }

        for (auto&& table : SHADER_TABLE_INFO) {
            mDXRStateObject->associateShaderInfoWithKey(table.key, table.type, table.name);
            switch (table.type) {
            case ShaderType::RayGeneration: break;
            case ShaderType::Miss:
                mDXRStateObject->bindLocalRootSignature(*mMissLocalRootSignature, table.name);
                break;
            case ShaderType::HitGroup:
                mDXRStateObject->bindLocalRootSignature(*mHitGroupLocalRootSignature, table.name);
                break;
            default: break;
            }
        }

        //その他シェーダーの設定
        UINT payloadSize
            = Framework::Math::MathUtil::mymax<UINT>({ sizeof(RayPayload), sizeof(ShadowPayload) });
        UINT attrSize = sizeof(float) * 2;
        UINT maxRecursionDepth = MAX_RAY_RECURSION_DEPTH;
        mDXRStateObject->setConfig(payloadSize, attrSize, maxRecursionDepth);

        mDXRStateObject->bindLocalRootSignature(*mMissLocalRootSignature, L"Miss");

        mDXRStateObject->bindGlobalRootSignature(*mGlobalRootSignature);

        mDXRStateObject->createPipeline();
        mDXRStateObject->prebuild();

        mDXRStateObject->setShaderTableConfig(
            ShaderType::RayGeneration, 1, 0, L"RayGenShaderTable");
        mDXRStateObject->appendShaderTable(ShaderKey::RayGenShader);
        {
            struct RootArgument {
                MissConstant cb;
            } rootArgument;
            rootArgument.cb.back = Color(188.0f / 255.0f, 226.0f / 255.0f, 232.0f / 255.0f, 1.0f);

            mDXRStateObject->setShaderTableConfig(
                ShaderType::Miss, 2, sizeof(RootArgument), L"MissShaderTable");
            mDXRStateObject->appendShaderTable(ShaderKey::MissShader, &rootArgument);
            mDXRStateObject->appendShaderTable(ShaderKey::MissShadowShader);
        }
        {
            struct RootArgument {
                D3D12_GPU_DESCRIPTOR_HANDLE albedo;
                D3D12_GPU_DESCRIPTOR_HANDLE normal;
                D3D12_GPU_DESCRIPTOR_HANDLE metallicRoughness;
                D3D12_GPU_DESCRIPTOR_HANDLE emissive;
                D3D12_GPU_DESCRIPTOR_HANDLE occlusion;
                HitGroupConstant cb;
            };

            mDXRStateObject->setShaderTableConfig(
                ShaderType::HitGroup, 4, sizeof(RootArgument), L"HitGroupShaderTable");

            auto setRootArgument = [&](const Model& model) {
                RootArgument arg;
                arg.cb.indexOffset = model.mIndexOffset;
                arg.cb.vertexOffset = model.mVertexOffset;
                arg.albedo = model.mAlbedo.getView().getInfo().gpuHandle;
                arg.normal = model.mNormalMap.getView().getInfo().gpuHandle;
                arg.metallicRoughness = model.mMetallicRoughness.getView().getInfo().gpuHandle;
                arg.emissive = model.mEmissiveMap.getView().getInfo().gpuHandle;
                arg.occlusion = model.mOcclusionMap.getView().getInfo().gpuHandle;
                return arg;
            };

            for (auto&& model : mLoadedModels) {
                mDXRStateObject->appendShaderTable(
                    model.second.mModelID, &setRootArgument(model.second));
            }

            mDXRStateObject->buildShaderTable();
        }
    }
}

void Scene::releaseDeviceDependentResources() {}

void Scene::createWindowDependentResources() {
    ID3D12Device* device = mDeviceResource->getDevice();
    DXGI_FORMAT backBufferFormat = mDeviceResource->getBackBufferFormat();

    TextureDesc desc = {};
    desc.width = mWidth;
    desc.height = mHeight;
    desc.name = L"RaytracingOutput";
    desc.flags = TextureFlags::UnorderedAccess;
    desc.format = backBufferFormat;

    mRaytracingOutput.init(mDeviceResource, desc);
    mRaytracingOutputUAV.initAsTexture2D(mDeviceResource, mRaytracingOutput, true);
}

void Scene::releaseWindowDependentResources() {}
