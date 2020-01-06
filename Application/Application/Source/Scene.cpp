#include "Scene.h"
#include <DirectXMath.h>
#include <numeric>
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Raytracing/Shader/ShaderTable.h"
#include "DX/Shader/PipelineState.h"
#include "DX/Util/BlendDesc.h"
#include "DX/Util/Helper.h"
#include "DX/Util/RasterizerDesc.h"
#include "ImGui/ImGuiManager.h"
#include "Math/Quaternion.h"
#include "Model.h"
#include "Utility/Debug.h"
#include "Utility/IO/ByteReader.h"
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
            HitGroup_House,
            HitGroup_Tree,
            HitGroup_Crate,

            End
        };
        constexpr UINT HITGROUP_NUM = ShaderKey::End - ShaderKey::HitGroup_UFO;
    } // namespace ShaderKey

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
        { ShaderKey::HitGroup_House, L"HitGroup_House", L"ClosestHit_Normal" },
        { ShaderKey::HitGroup_Tree, L"HitGroup_Tree", L"ClosestHit_Normal" },
        { ShaderKey::HitGroup_Crate, L"HitGroup_Crate", L"ClosestHit_Normal" },
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
        { L"HitGroup_House", ShaderKey::HitGroup_House, ShaderType::HitGroup },
        { L"HitGroup_Tree", ShaderKey::HitGroup_Tree, ShaderType::HitGroup },
        { L"HitGroup_Crate", ShaderKey::HitGroup_Crate, ShaderType::HitGroup },
    };

    struct LoadModelInfo {
        std::wstring name;
        UINT shaderKey;
    };

    static const std::unordered_map<ModelType::Enum, LoadModelInfo> MODEL_NAMES = {
        { ModelType::UFO, { L"UFO.glb", ShaderKey::HitGroup_UFO } },
        { ModelType::Sphere, { L"sphere.glb", ShaderKey::HitGroup_Sphere } },
        { ModelType::Floor, { L"floor.glb", ShaderKey::HitGroup_Floor } },
        { ModelType::House, { L"house.glb", ShaderKey::HitGroup_House } },
        { ModelType::Tree, { L"tree.glb", ShaderKey::HitGroup_Tree } },
        { ModelType::Crate, { L"Crate.glb", ShaderKey::HitGroup_Crate } },
    };

    std::unordered_map<ModelType::Enum, Model> mLoadedModels;

    struct Object {
        Model* model;
        BottomLevelAccelerationStructure* blas;
        Vec3 position;
        Quaternion rotation;
        Vec3 scale;
    };
    Object mFloor;
    std::vector<Object> mSpheres;
    Object mHouse;
    std::vector<Object> mTree;
    Object mCrate;

    RootSignature mDefaultRootSignature;
    PipelineState mGrayScalePipelineState;
    VertexBuffer mQuadVertex;
    IndexBuffer mQuadIndex;
    DescriptorInfo mSamplerInfo;
} // namespace

Scene::Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager,
    UINT width, UINT height)
    : mDeviceResource(device),
      mInputManager(inputManager),
      mDXRDevice(),
      mWidth(width),
      mHeight(height) {}
Scene::~Scene() {}

void Scene::create() {
    createDeviceDependentResources();
    createWindowDependentResources();
    {
        mSceneCB.init(mDeviceResource, L"SceneConstantBuffer");
        mSceneCB.createCBV(mDeviceResource, DescriptorHeapType::RaytracingGlobal);
        mSceneCB->cameraPosition = Vec4(0, 50, -300, 1.0f);
        mSceneCB->lightPosition = Vec4(0, 100, -100, 0);
        mSceneCB->gammaRate = 1.0f;
        mCameraRotation = Vec3::ZERO;
        mLightAmbient = Color4(0.1f, 0.1f, 0.1f, 1.0f);
    }
    //ポストエフェクトの初期化
    {
        using namespace Framework::Desc;
        mDefaultRootSignature.init(mDeviceResource, std::vector<CD3DX12_STATIC_SAMPLER_DESC>());

        PipelineStateDesc desc(mDefaultRootSignature);
        std::filesystem::path shaderPath = ExePath::getInstance()->exe() / "cso";
        std::vector<BYTE> vs = ByteReader::read(shaderPath / "GrayScale_VS.cso");
        std::vector<BYTE> ps = ByteReader::read(shaderPath / "GrayScale_PS.cso");
        desc.vs = CD3DX12_SHADER_BYTECODE(vs.data(), vs.size());
        desc.ps = CD3DX12_SHADER_BYTECODE(ps.data(), ps.size());
        desc.blend = BlendDesc(BlendMode::Default);
        desc.depthStencil = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        desc.dsvFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
        desc.flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
        D3D12_INPUT_ELEMENT_DESC inputElements[] = {
            { "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };
        desc.inputLayout = { inputElements, _countof(inputElements) };
        desc.rasterizer = RasterizerDesc(CullMode::Back, FillMode::Solid);
        desc.sample.Count = 1;
        desc.renderTargetNum = 1;
        desc.rtvFormat[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.sampleMask = UINT_MAX;

        mGrayScalePipelineState.init(mDeviceResource, desc, L"GrayScale");

        struct QuadVertex {
            Vec3 pos;
            Vec2 uv;
        };
        std::vector<QuadVertex> vertices{
            { Vec3(-1, 1, 0), Vec2(0, 0) },
            { Vec3(1, 1, 0), Vec2(1, 0) },
            { Vec3(1, -1, 0), Vec2(1, 1) },
            { Vec3(-1, -1, 0), Vec2(0, 1) },
        };
        std::vector<Index> indices{ 0, 1, 2, 0, 2, 3 };
        mQuadVertex.init(mDeviceResource, vertices, L"QuadVertex");
        mQuadIndex.init(mDeviceResource, indices,
            D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"QuadIndex");

        D3D12_SAMPLER_DESC sampler = {};
        sampler.Filter = D3D12_FILTER::D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS_EQUAL;
        sampler.MinLOD = 0;
        sampler.MaxLOD = FLT_MAX;
        mSamplerInfo = mDeviceResource->getHeapManager()->allocate(DescriptorHeapType::Sampler);
        mDeviceResource->getDevice()->CreateSampler(&sampler, mSamplerInfo.cpuHandle);
    }
}
void Scene::reset() {
    mDXRDevice.reset();
    mGpuTimer.reset();
    mGpuTimer.releaseDevice();
}

void Scene::update() {
    mTime.update();

#pragma region IMGUI_REGION
    if (ImGui::Begin("Status")) {
        ImGui::Text("FPS:%0.3f", mTime.getFPS());
        ImGui::End();
    }

    ImGui::Begin("Parameter");
    {
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
        if (ImGui::TreeNode("Camera")) {
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
            if (ImGui::TreeNode("Position")) {
                ImGui::DragFloat("X", &mSceneCB->cameraPosition.x, 1.0f);
                ImGui::DragFloat("Y", &mSceneCB->cameraPosition.y, 1.0f);
                ImGui::DragFloat("Z", &mSceneCB->cameraPosition.z, 1.0f);
                ImGui ::TreePop();
            }
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
            if (ImGui::TreeNode("Rotation")) {
                ImGui::DragFloat("X", &mCameraRotation.x, 0.05f);
                ImGui::DragFloat("Y", &mCameraRotation.y, 0.05f);
                ImGui::DragFloat("Z", &mCameraRotation.z, 0.05f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
        if (ImGui::TreeNode("Light")) {
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
            if (ImGui::TreeNode("LightPosition")) {
                ImGui::DragFloat("X", &mSceneCB->lightPosition.x, 1.0f);
                ImGui::DragFloat("Y", &mSceneCB->lightPosition.y, 1.0f);
                ImGui::DragFloat("Z", &mSceneCB->lightPosition.z, 1.0f);
                ImGui::TreePop();
            }
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
            if (ImGui::TreeNode("LightColor")) {
                ImGui::DragFloat("R", &mSceneCB->lightDiffuse.r, 1.0f / 255.0f, 0.0f, 1.0f);
                ImGui::DragFloat("G", &mSceneCB->lightDiffuse.g, 1.0f / 255.0f, 0.0f, 1.0f);
                ImGui::DragFloat("B", &mSceneCB->lightDiffuse.b, 1.0f / 255.0f, 0.0f, 1.0f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_::ImGuiCond_Once);
        if (ImGui::TreeNode("Option")) {
            ImGui::DragFloat("Gamma(%)", &mSceneCB->gammaRate, 0.01f, 0.0f, 2.0f, "%.3f");
            ImGui::TreePop();
        }
        ImGui::End();
    }

#pragma endregion
#pragma region CONSTANT_BUFFER_UPDATE
    const float aspect = static_cast<float>(mWidth) / static_cast<float>(mHeight);

    Vec3 cameraPosition(
        mSceneCB->cameraPosition.x, mSceneCB->cameraPosition.y, mSceneCB->cameraPosition.z);
    Mat4 view = Mat4::createRotation(mCameraRotation) * Mat4::createTranslate(cameraPosition);
    view = view.inverse();
    Mat4 proj = Mat4::createProjection(Deg(45.0f), aspect, 0.1f, 100.0f);
    Mat4 vp = view * proj;
    mSceneCB->projectionToWorld = vp.inverse();
    mSceneCB->lightAmbient = mLightAmbient;
    mSceneCB->globalTime = static_cast<float>(mTime.getTime());
#pragma endregion
    static float rotHouse = 180.0f;
    mHouse.rotation = Quaternion::fromEular(Vec3(0, rotHouse, 0));
}

void Scene::render() {
    ID3D12Device* device = mDeviceResource->getDevice();
    ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice.getDXRCommandList();

    mTLASBuffer->clear();
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
        instanceDesc.hitGroupIndex = obj.model->mModelID;
        instanceDesc.blas = obj.blas;
        instanceDesc.transform = transform;
        mTLASBuffer->add(instanceDesc);
    };

    addTLAS(mFloor);
    for (auto&& obj : mSpheres) { addTLAS(obj); }
    addTLAS(mHouse);
    for (auto&& obj : mTree) { addTLAS(obj); }
    addTLAS(mCrate);

    mTLASBuffer->build(mDXRDevice, mDeviceResource,
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE);

    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature->getRootSignature());
    mSceneCB.updateStaging();

    mDeviceResource->getHeapManager()->setDescriptorHeap(
        commandList, DescriptorHeapType::RaytracingGlobal);

    DescriptorSet globalSet;
    globalSet.setCbvHandle(0, mSceneCB.getView().getInfo().cpuHandle);
    globalSet.setSrvHandle(0, mTLASBuffer->getView().getInfo().cpuHandle);
    globalSet.setSrvHandle(1, mResourceIndexBufferSRV.getInfo().cpuHandle);
    globalSet.setSrvHandle(2, mResourceVertexBufferSRV.getInfo().cpuHandle);
    globalSet.setUavHandle(0, mRaytracingOutputUAV.getInfo().cpuHandle);

    mDeviceResource->getHeapManager()->copyAndSetComputeDescriptorHeap(
        DescriptorHeapType ::RaytracingGlobal, mDeviceResource, commandList, globalSet);
    mDXRStateObject->doRaytracing(mWidth, mHeight);

    D3D12_RESOURCE_BARRIER preCopyBarriers[2];
    preCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
        mDeviceResource->getRenderTarget()->getBuffer().getResource(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
    preCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.getResource(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->ResourceBarrier(_countof(preCopyBarriers), preCopyBarriers);

    commandList->CopyResource(mDeviceResource->getRenderTarget()->getBuffer().getResource(),
        mRaytracingOutput.getResource());

    D3D12_RESOURCE_BARRIER postCopyBarriers[2];
    postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
        mDeviceResource->getRenderTarget()->getBuffer().getResource(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
    postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.getResource(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    commandList->ResourceBarrier(_countof(postCopyBarriers), postCopyBarriers);

    commandList->RSSetViewports(1, &mDeviceResource->getScreenViewport());
    commandList->RSSetScissorRects(1, &mDeviceResource->getScissorRect());
    commandList->SetGraphicsRootSignature(mDefaultRootSignature.getRootSignature());
    commandList->SetPipelineState(mGrayScalePipelineState.getPipelineState());
    mDeviceResource->getHeapManager()->setDescriptorHeap(
        commandList, DescriptorHeapType::CbvSrvUav, DescriptorHeapType::Sampler);

    DescriptorSet grayScaleSet;
    grayScaleSet.setSrvHandle(0, mRaytracingOutputUAV.getInfo().cpuHandle);
    grayScaleSet.setSamplerHandle(0, mSamplerInfo.cpuHandle);
    mDeviceResource->getHeapManager()->copyAndSetGraphicsDescriptorHeap(
        DescriptorHeapType::CbvSrvUav, mDeviceResource, commandList, grayScaleSet);
    mDeviceResource->getHeapManager()->copyAndSetGraphicsDescriptorHeap(
        DescriptorHeapType::Sampler, mDeviceResource, commandList, grayScaleSet);

    mQuadVertex.setCommandList(commandList);
    mQuadIndex.setCommandList(commandList);
    mQuadIndex.draw(commandList);
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
        mGlobalRootSignature = std::make_unique<RootSignature>();
        mGlobalRootSignature->init(mDeviceResource, desc);
    }
    //ミスシェーダー
    {
        std::vector<CD3DX12_ROOT_PARAMETER> params(1);
        params[0].InitAsConstants(
            Framework::Math::MathUtil::alignPow2(sizeof(MissConstant), sizeof(UINT32)), 1);

        RootSignatureDesc desc{ params, {}, RootSignature::Flags::Local };
        mMissLocalRootSignature = std::make_unique<RootSignature>();
        mMissLocalRootSignature->init(mDeviceResource, desc);
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
        mHitGroupLocalRootSignature = std::make_unique<RootSignature>();
        mHitGroupLocalRootSignature->init(mDeviceResource, desc);
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

        UINT id = 0;
        for (auto&& load : MODEL_NAMES) {
            Model model = loadModel(modelPath / load.second.name, load.second.shaderKey);
            resourceIndices.insert(
                resourceIndices.end(), model.mIndices.begin(), model.mIndices.end());
            resourceVertices.insert(
                resourceVertices.end(), model.mVertices.begin(), model.mVertices.end());
            model.mModelID = id++;
            mBLASBuffers[load.first] = std::make_unique<BottomLevelAccelerationStructure>();
            mBLASBuffers[load.first]->init(mDXRDevice, model.mVertexBuffer,
                static_cast<UINT>(sizeof(Vertex)), model.mIndexBuffer,
                static_cast<UINT>(sizeof(Index)));
            mLoadedModels[load.first] = model;
        }

        mResourcesIndexBuffer.init(mDeviceResource, resourceIndices,
            D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"ResourceIndex");
        mResourceIndexBufferSRV = mResourcesIndexBuffer.createSRV(
            mDeviceResource, DescriptorHeapType::RaytracingGlobal);

        mResourcesVertexBuffer.init(mDeviceResource, resourceVertices, L"ResourceVertex");
        mResourceVertexBufferSRV.initAsBuffer(mDeviceResource, mResourcesVertexBuffer.getBuffer(),
            DescriptorHeapType::RaytracingGlobal);

        mTLASBuffer = std::make_unique<TopLevelAccelerationStructure>();

        mDeviceResource->executeCommandList();
        mDeviceResource->waitForGPU();

        auto createModel = [&](ModelType::Enum type, const Vec3& position,
                               const Quaternion& rotation, const Vec3& scale) {
            Object model = {};
            model.model = &mLoadedModels[type];
            model.blas = mBLASBuffers[type].get();
            model.position = position;
            model.rotation = rotation;
            model.scale = scale;
            return model;
        };

        mFloor
            = createModel(ModelType::Floor, Vec3(0, 0, 0), Quaternion::IDENTITY, Vec3(500, 1, 500));
        mHouse = createModel(
            ModelType::House, Vec3(-20, 0, 0), Quaternion::IDENTITY, Vec3(30, 30, 30));
        for (float z = -200; z <= 200; z += 80.0f) {
            mTree.emplace_back(createModel(
                ModelType::Tree, Vec3(-200, 0, z), Quaternion::IDENTITY, Vec3(1, 1, 1)));
            mTree.emplace_back(
                createModel(ModelType::Tree, Vec3(200, 0, z), Quaternion::IDENTITY, Vec3(1, 1, 1)));
        }
        mCrate
            = createModel(ModelType::Crate, Vec3(0, 0, -100), Quaternion::IDENTITY, Vec3(1, 1, 1));
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

            mDXRStateObject->setShaderTableConfig(ShaderType::HitGroup, ShaderKey::HITGROUP_NUM,
                sizeof(RootArgument), L"HitGroupShaderTable");

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
                    model.second.mShaderKey, &setRootArgument(model.second));
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
    mRaytracingOutputUAV.initAsTexture2D(
        mDeviceResource, mRaytracingOutput, DescriptorHeapType::RaytracingGlobal);
}

void Scene::releaseWindowDependentResources() {}
