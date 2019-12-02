#include "Scene.h"
#include <DirectXMath.h>
#include <numeric>
#include "DX/Raytracing/Shader/ShaderTable.h"
#include "ImGui/ImGuiManager.h"
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
using namespace Framework::Utility;
using namespace DirectX;

namespace {
    namespace ShaderKey {
        enum MyEnum {
            RayGenShader,
            MissShader,
            MissShadowShader,
            HitGroup_UFO,
            HitGroup_Quad,
            HitGroup_Floor,
            HitGroup_Sphere,
        };
    }
    static const std::wstring MISS_SHADER_NAME = L"Miss";
    static const std::wstring MISS_SHADOW_SHADER_NAME = L"Shadow";
    static const std::wstring CLOSEST_HIT_NORMAL_NAME = L"ClosestHit_Normal";
    static const std::wstring CLOSEST_HIT_PLANE_NAME = L"ClosestHit_Plane";
    static const std::wstring CLOSEST_HIT_SPHERE_NAME = L"ClosestHit_Sphere";
    static const std::wstring RAY_GEN_NAME = L"RayGenShader";
    static const std::wstring HIT_GROUP_UFO_NAME = L"HitGroup_UFO";
    static const std::wstring HIT_GROUP_QUAD_NAME = L"HitGroup_Quad";
    static const std::wstring HIT_GROUP_FLOOR_NAME = L"HitGroup_Floor";
    static const std::wstring HIT_GROUP_SPHERE_NAME = L"HitGroup_Sphere";

    static const std::unordered_map<BottomLevelASType::MyEnum, std::wstring> MODEL_NAMES = {
        { BottomLevelASType::UFO, L"UFO.glb" },
        { BottomLevelASType::Sphere, L"sphere.glb" },
        { BottomLevelASType::Floor, L"floor.glb" },
    };

    static constexpr UINT UFO_COUNT = 0;
    static constexpr UINT QUAD_COUNT = 0;
    static constexpr UINT FLOOR_COUNT = 1;
    static constexpr UINT SPHERE_COUNT = 100;

    template <class T>
    inline std::vector<T> toLinearList(const std::vector<std::vector<T>>& list) {
        std::vector<T> res;
        for (size_t i = 0; i < list.size(); i++) {
            res.insert(res.end(), list[i].begin(), list[i].end());
        }
        return res;
    }

    using PositionList = std::vector<Framework::Math::Vector3>;
    using NormalList = std::vector<Framework::Math::Vector3>;
    using UVList = std::vector<Framework::Math::Vector2>;
    using TangentList = std::vector<Framework::Math::Vector4>;

    inline std::vector<Framework::DX::Vertex> toLinearVertices(
        const std::vector<PositionList>& positions, const std::vector<NormalList>& normals = {},
        const std::vector<UVList>& uvs = {}, const std::vector<TangentList>& tangents = {}) {
        std::vector<Framework::DX::Vertex> res;
        for (size_t i = 0; i < positions.size(); i++) {
            for (size_t j = 0; j < positions[i].size(); j++) {
                Framework::DX::Vertex v;
                v.position = positions[i][j];
                v.normal = normals.empty() || normals[i].empty() ? Vec3(0, 0, 0) : normals[i][j];
                v.uv = uvs.empty() || uvs[i].empty() ? Vec2(0, 0) : uvs[i][j];
                v.tangent
                    = tangents.empty() || tangents[i].empty() ? Vec4(0, 0, 0, 0) : tangents[i][j];
                res.emplace_back(v);
            }
        }
        return res;
    }

    enum ModelTextureType {
        Default_AlbedoTexture,
        Default_NormalMap,
        Default_MetallicRoughnessMap,
        Default_EmissiveMap,
        Default_OcclusionMap,

        UFO_AlbedoTexture,
        UFO_NormalMap,
        UFO_MetallicRoughness,
        UFO_EmissiveMap,
        UFO_OcclusionMap,

        Quad_AlbedoTexture,
        Quad_NormalMap,
        Quad_MetallicRoughnessMap,
        Quad_EmissiveMap,
        Quad_OcclusionMap,

        Plane_AlbedoTexture,
        Plane_NormalMap,
        Plane_MetallicRoughnessMap,
        Plane_EmissiveMap,
        Plane_OcclusionMap,

        Sphere_AlbedoTexture,
        Sphere_NormalMap,
        Sphere_MetallicRoughnessMap,
        Sphere_EmissiveMap,
        Sphere_OcclusionMap,
    };
    std::unordered_map<ModelTextureType, UINT> mTextureIDs;
} // namespace

Scene::Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager,
    UINT width, UINT height)
    : mDeviceResource(device),
      mInputManager(inputManager),
      mDXRDevice(),
      mWidth(width),
      mHeight(height) {
    Framework::Desc::DescriptorTableDesc desc = { L"ResourceTable", 10000,
        Framework::Desc::HeapType::CBV_SRV_UAV, Framework::Desc::HeapFlag::ShaderVisible };
    mDescriptorTable = std::make_unique<DescriptorTable>(device->getDevice(), desc);
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
        mSceneCB = std::make_unique<ConstantBuffer<SceneConstantBuffer>>(
            device, backBufferCount, L"SceneConstantBuffer");
    }
}

void Scene::reset() {
    mDXRDevice.reset();
    mGpuTimer.reset();
    mGpuTimer.releaseDevice();
}

void Scene::update() {
    mTime.update();
    ImGui::Begin("Status");
    { ImGui::Text("FPS:%0.3f", mTime.getFPS()); }
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
    }
    ImGui::End();

    mSceneCB->staging().cameraPosition = Vec4(mCameraPosition, 1.0f);
    const float aspect = static_cast<float>(mWidth) / static_cast<float>(mHeight);

    Mat4 view = Mat4::createRotation(mCameraRotation) * Mat4::createTranslate(mCameraPosition);
    view = view.inverse();
    Mat4 proj = Mat4::createProjection(Deg(45.0f), aspect, 0.1f, 100.0f);
    Mat4 vp = view * proj;
    mSceneCB->staging().projectionToWorld = vp.inverse();
    mSceneCB->staging().lightPosition = Vec4(mLightPosition, 1.0f);
    mSceneCB->staging().lightDiffuse = mLightDiffuse;
    mSceneCB->staging().lightAmbient = mLightAmbient;
    mSceneCB->staging().globalTime = static_cast<float>(mTime.getTime());
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

    for (UINT n = 0; n < FLOOR_COUNT; n++) {
        XMMATRIX transform = XMMatrixScaling(100, 1, 100) * XMMatrixTranslation(0, -5, 0);
        instanceDesc.hitGroupIndex = LocalRootSignature::HitGroupIndex::Floor;
        instanceDesc.blas = mBLASBuffers[BottomLevelASType::Floor].get();
        instanceDesc.transform = transform;
        mTLASBuffer->add(instanceDesc);
    }

    static float X = 0.0f;
    X += 0.001f;
    UINT root = static_cast<UINT>(Framework::Math::MathUtil::sqrt(SPHERE_COUNT));
    for (UINT n = 0; n < SPHERE_COUNT; n++) {
        float x = (n % root) * 20.0f;
        float z = (n / root) * 20.0f;
        XMMATRIX transform = XMMatrixRotationY(X) * XMMatrixTranslation(x, 5.0f, z);
        instanceDesc.hitGroupIndex = LocalRootSignature::HitGroupIndex::Sphere;
        instanceDesc.blas = mBLASBuffers[BottomLevelASType::Sphere].get();
        instanceDesc.transform = transform;
        mTLASBuffer->add(instanceDesc);
    }

    mTLASBuffer->build(mDXRDevice,
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE);

    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature->getRootSignature());
    mSceneCB->copyStatingToGPU(frameIndex);

    ID3D12DescriptorHeap* heaps[] = { mDescriptorTable->getHeap() };
    commandList->SetDescriptorHeaps(_countof(heaps), heaps);
    commandList->SetComputeRootDescriptorTable(
        GlobalRootSignature::Slot::RenderTarget, mRaytracingOutput.getGPUHandle());
    commandList->SetComputeRootShaderResourceView(GlobalRootSignature::Slot::AccelerationStructure,
        mTLASBuffer->getBuffer()->GetGPUVirtualAddress());
    commandList->SetComputeRootConstantBufferView(
        GlobalRootSignature::Slot::SceneConstant, mSceneCB->gpuVirtualAddress());
    commandList->SetComputeRootDescriptorTable(
        GlobalRootSignature::Slot::IndexBuffer, mResourcesIndexBuffer->getGPUHandle());
    commandList->SetComputeRootDescriptorTable(
        GlobalRootSignature::Slot::VertexBuffer, mResourcesVertexBuffer->getGPUHandle());

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
    { //グローバルルートシグネチャ
        { CD3DX12_DESCRIPTOR_RANGE ranges[3];
    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
    ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
    ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

    std::vector<CD3DX12_ROOT_PARAMETER> rootParams(GlobalRootSignature::Slot::Count);
    rootParams[GlobalRootSignature::Slot::RenderTarget].InitAsDescriptorTable(1, &ranges[0]);
    rootParams[GlobalRootSignature::Slot::AccelerationStructure].InitAsShaderResourceView(0);
    rootParams[GlobalRootSignature::Slot::SceneConstant].InitAsConstantBufferView(0);
    rootParams[GlobalRootSignature::Slot::IndexBuffer].InitAsDescriptorTable(1, &ranges[1]);
    rootParams[GlobalRootSignature::Slot::VertexBuffer].InitAsDescriptorTable(1, &ranges[2]);

    std::vector<CD3DX12_STATIC_SAMPLER_DESC> sampler(1);
    sampler[0]
        = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

    RootSignatureDesc desc{ rootParams, sampler, RootSignature::Flags::None };
    mGlobalRootSignature = std::make_unique<RootSignature>(mDeviceResource->getDevice(), desc);
}
//ミスシェーダー
{
    std::vector<CD3DX12_ROOT_PARAMETER> params(1);
    params[0].InitAsConstants(
        Framework::Math::MathUtil::alignPow2(sizeof(MissConstant), sizeof(UINT32)), 1);

    RootSignatureDesc desc{ params, {}, RootSignature::Flags::Local };
    mMissLocalRootSignature = std::make_unique<RootSignature>(mDeviceResource->getDevice(), desc);
}
//ヒットグループシェーダー
{
    CD3DX12_DESCRIPTOR_RANGE range[5];
    range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);
    range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);
    range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);
    range[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 6);
    range[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 7);

    std::vector<CD3DX12_ROOT_PARAMETER> params(6);
    UINT contSize = Framework::Math::MathUtil::alignPow2(sizeof(HitGroupConstant), sizeof(UINT32));

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
}
{
    using namespace Framework::Desc;
    DescriptorTableDesc desc
        = { L"DescriptorTable", 10000, HeapType::CBV_SRV_UAV, HeapFlag::ShaderVisible };
    mDescriptorTable->create(mDeviceResource->getDevice(), desc);
}

std::array<UINT, LocalRootSignature::HitGroupIndex::Count> mIndexOffsets;
std::array<UINT, LocalRootSignature::HitGroupIndex::Count> mVertexOffsets;
auto getOffset = [&mIndexOffsets, &mVertexOffsets](LocalRootSignature::HitGroupIndex::MyEnum e) {
    if (e == 0)
        return std::make_tuple<UINT, UINT>(0, 0);
    else
        return std::make_tuple(
            std::accumulate(mIndexOffsets.begin(), mIndexOffsets.begin() + e, 0u),
            std::accumulate(mVertexOffsets.begin(), mVertexOffsets.begin() + e, 0u));
};

{
    ID3D12Device* device = mDeviceResource->getDevice();
    std::array<std::unique_ptr<IndexBuffer>, BottomLevelASType::Count> mIndexBuffer;
    std::array<std::unique_ptr<VertexBuffer>, BottomLevelASType::Count> mVertexBuffer;

    std::vector<Framework::DX::Vertex> resourceVertices;
    std::vector<Index> resourceIndices;
    {

        auto path = Framework::Utility::ExePath::getInstance()->exe();
        path = path.remove_filename();
        auto modelPath = path / "Resources" / "Model";
        auto texPath = path / "Resources" / "Texture";

        {
            auto createUnitTexture = [](const Color4& color, const std::wstring& name) {
                Framework::Desc::TextureDesc desc;
                desc.format = Framework::Desc::TextureFormat::R8G8B8A8;
                desc.width = 1;
                desc.height = 1;
                desc.pixels.resize(desc.width * desc.height * 4);
                desc.pixels[0] = static_cast<BYTE>(color.r * 255.0f);
                desc.pixels[1] = static_cast<BYTE>(color.g * 255.0f);
                desc.pixels[2] = static_cast<BYTE>(color.b * 255.0f);
                desc.pixels[3] = static_cast<BYTE>(color.a * 255.0f);
                desc.name = name;
                return desc;
            };
            auto createDefaultTexture = [&](const std::wstring& name, ModelTextureType texType,
                                            const Color4& col, UINT heapIndex) {
                std::shared_ptr<Texture2D> texture
                    = std::make_shared<Texture2D>(device, createUnitTexture(col, name));

                texture->setCPUHandle(mDescriptorTable->getCPUHandle(heapIndex));
                texture->setGPUHandle(mDescriptorTable->getGPUHandle(heapIndex));
                texture->createSRV(device);
                mTextures[texType] = texture;
                mTextureIDs[texType] = texType;
            };

            createDefaultTexture(L"DefaultAlbedo", ModelTextureType::Default_AlbedoTexture,
                Color4(1, 1, 1, 1), DescriptorHeapIndex::DefaultTexture_Albedo);
            createDefaultTexture(L"DefaultNormal", ModelTextureType::Default_NormalMap,
                Color4(0.5f, 0.5f, 1.0f, 1.0f), DescriptorHeapIndex::DefaultTexture_NormalMap);
            createDefaultTexture(L"DefaultMetallicRoughness",
                ModelTextureType::Default_MetallicRoughnessMap, Color4(0, 0, 0, 1),
                DescriptorHeapIndex::DefaultTexture_MetallicRoughnessMap);
            createDefaultTexture(L"DefaultEmissive", ModelTextureType::Default_EmissiveMap,
                Color4(0, 0, 0, 1), DescriptorHeapIndex::DefaultTexture_EmissiveMap);
            createDefaultTexture(L"DefaultOcculusion", ModelTextureType::Default_OcclusionMap,
                Color4(1, 1, 1, 1), DescriptorHeapIndex::DefaultTexture_OcclusionMap);
        }

        auto loadTextureIfExist = [&](bool expr, ModelTextureType type,
                                      const std::vector<Framework::Desc::TextureDesc>& textureDatas,
                                      UINT descID, UINT heapIndex, ModelTextureType defaultID) {
            if (expr) {
                Framework::Desc::TextureDesc desc = textureDatas[descID];
                auto texture = std::make_shared<Texture2D>(device, desc);
                texture->setCPUHandle(mDescriptorTable->getCPUHandle(heapIndex));
                texture->setGPUHandle(mDescriptorTable->getGPUHandle(heapIndex));
                texture->createSRV(device);
                mTextures[type] = texture;
                mTextureIDs[type] = type;
            } else {
                mTextureIDs[type] = mTextureIDs[defaultID];
            }
        };

        {
            Framework::Utility::GLBLoader loader(
                modelPath / MODEL_NAMES.at(BottomLevelASType::UFO));
            auto indices = toLinearList(loader.getIndicesPerSubMeshes());
            auto vertices = toLinearVertices(loader.getPositionsPerSubMeshes(),
                loader.getNormalsPerSubMeshes(), loader.getUVsPerSubMeshes(),
                loader.getTangentsPerSubMeshes());

            mIndexBuffer[BottomLevelASType::UFO] = std::make_unique<IndexBuffer>(device, indices,
                D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"IndexBuffer_UFO");
            mVertexBuffer[BottomLevelASType::UFO]
                = std::make_unique<VertexBuffer>(device, vertices, L"VertexBuffer_UFO");

            resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
            resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
            mIndexOffsets[LocalRootSignature::HitGroupIndex::UFO] = (UINT)indices.size();
            mVertexOffsets[LocalRootSignature::HitGroupIndex::UFO] = (UINT)vertices.size();

            auto materialList = loader.getMaterialDatas();
            GlbMaterial material = {};
            if (!materialList.empty()) material = loader.getMaterialDatas()[0];
            auto descs = loader.getImageDatas();

            loadTextureIfExist(!descs.empty(), ModelTextureType::UFO_AlbedoTexture, descs, 0,
                DescriptorHeapIndex::UFO_Albedo, ModelTextureType::Default_AlbedoTexture);
            loadTextureIfExist(material.normalMapID != -1, ModelTextureType::UFO_NormalMap, descs,
                material.normalMapID, DescriptorHeapIndex::UFO_NormalMap,
                ModelTextureType::Default_NormalMap);
            loadTextureIfExist(material.metallicRoughnessMapID != -1,
                ModelTextureType::UFO_MetallicRoughness, descs, material.metallicRoughnessMapID,
                DescriptorHeapIndex::UFO_MetallicRoughnessMap,
                ModelTextureType::Default_MetallicRoughnessMap);
            loadTextureIfExist(material.emissiveMapID != -1, ModelTextureType::UFO_EmissiveMap,
                descs, material.emissiveMapID, DescriptorHeapIndex::UFO_EmissiveMap,
                ModelTextureType::Default_EmissiveMap);
            loadTextureIfExist(material.occlusionMapID != -1, ModelTextureType::UFO_OcclusionMap,
                descs, material.occlusionMapID, DescriptorHeapIndex::UFO_OcclusionMap,
                ModelTextureType::Default_OcclusionMap);
        }
        {
            std::vector<Index> indices = { 0, 1, 2, 0, 2, 3 };
            mIndexBuffer[BottomLevelASType::Quad] = std::make_unique<IndexBuffer>(device, indices,
                D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"IndexBuffer_Quad");
            std::vector<Framework::DX::Vertex> vertices = {
                { Vec3(-1, 1, 0), Vec3(0, 0, -1), Vec2(0, 0) },
                { Vec3(1, 1, 0), Vec3(0, 0, -1), Vec2(1, 0) },
                { Vec3(1, -1, 0), Vec3(0, 0, -1), Vec2(1, 1) },
                { Vec3(-1, -1, 0), Vec3(0, 0, -1), Vec2(0, 1) },
            };
            mVertexBuffer[BottomLevelASType::Quad]
                = std::make_unique<VertexBuffer>(device, vertices, L"VertexBuffer_Quad");

            resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
            resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
            mIndexOffsets[LocalRootSignature::HitGroupIndex::Quad] = (UINT)indices.size();
            mVertexOffsets[LocalRootSignature::HitGroupIndex::Quad] = (UINT)vertices.size();

            Framework::Desc::TextureDesc desc
                = Framework::Utility::TextureLoader::load(texPath / "back2.png");
            auto texture = std::make_shared<Texture2D>(device, desc);

            texture->setCPUHandle(mDescriptorTable->getCPUHandle(DescriptorHeapIndex::Quad_Albedo));
            texture->setGPUHandle(mDescriptorTable->getGPUHandle(DescriptorHeapIndex::Quad_Albedo));
            texture->createSRV(device);
            mTextures[ModelTextureType::Quad_AlbedoTexture] = texture;
            mTextureIDs[ModelTextureType::Quad_AlbedoTexture]
                = ModelTextureType::Quad_AlbedoTexture;
            mTextureIDs[ModelTextureType::Quad_NormalMap] = ModelTextureType::Default_NormalMap;
            mTextureIDs[ModelTextureType::Quad_MetallicRoughnessMap]
                = ModelTextureType::Default_MetallicRoughnessMap;
            mTextureIDs[ModelTextureType::Quad_EmissiveMap] = ModelTextureType::Default_EmissiveMap;
            mTextureIDs[ModelTextureType::Quad_OcclusionMap]
                = ModelTextureType::Default_OcclusionMap;
        }

        {
            Framework::Utility::GLBLoader loader(
                modelPath / MODEL_NAMES.at(BottomLevelASType::Floor));
            auto indices = toLinearList(loader.getIndicesPerSubMeshes());
            mIndexBuffer[BottomLevelASType::Floor] = std::make_unique<IndexBuffer>(device, indices,
                D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"IndexBuffer_Floor");

            auto vertices = toLinearVertices(loader.getPositionsPerSubMeshes(),
                loader.getNormalsPerSubMeshes(), loader.getUVsPerSubMeshes());
            mVertexBuffer[BottomLevelASType::Floor]
                = std::make_unique<VertexBuffer>(device, vertices, L"VertexBuffer_Floor");

            resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
            resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
            mIndexOffsets[LocalRootSignature::HitGroupIndex::Floor] = (UINT)indices.size();
            mVertexOffsets[LocalRootSignature::HitGroupIndex::Floor] = (UINT)vertices.size();

            auto materialList = loader.getMaterialDatas();
            GlbMaterial material = {};
            if (!materialList.empty()) material = loader.getMaterialDatas()[0];
            auto descs = loader.getImageDatas();
            loadTextureIfExist(!descs.empty(), ModelTextureType::Plane_AlbedoTexture, descs, 0,
                DescriptorHeapIndex::Plane_Albedo, ModelTextureType::Default_AlbedoTexture);
            loadTextureIfExist(material.normalMapID != -1, ModelTextureType::Plane_NormalMap, descs,
                material.normalMapID, DescriptorHeapIndex::Plane_NormalMap,
                ModelTextureType::Default_NormalMap);
            loadTextureIfExist(material.metallicRoughnessMapID != -1,
                ModelTextureType::Plane_MetallicRoughnessMap, descs,
                material.metallicRoughnessMapID, DescriptorHeapIndex::Plane_MetallicRoughnessMap,
                ModelTextureType::Default_MetallicRoughnessMap);
            loadTextureIfExist(material.emissiveMapID != -1, ModelTextureType::Plane_EmissiveMap,
                descs, material.emissiveMapID, DescriptorHeapIndex::Plane_EmissiveMap,
                ModelTextureType::Default_EmissiveMap);
            loadTextureIfExist(material.occlusionMapID != -1, ModelTextureType::Plane_OcclusionMap,
                descs, material.occlusionMapID, DescriptorHeapIndex::Plane_OcclusionMap,
                ModelTextureType::Default_OcclusionMap);
        }
        {

            Framework::Utility::GLBLoader loader(
                modelPath / MODEL_NAMES.at(BottomLevelASType::Sphere));
            auto indices = toLinearList(loader.getIndicesPerSubMeshes());
            mIndexBuffer[BottomLevelASType::Sphere] = std::make_unique<IndexBuffer>(device, indices,
                D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"IndexBuffer_Sphere");

            auto vertices = toLinearVertices(loader.getPositionsPerSubMeshes(),
                loader.getNormalsPerSubMeshes(), loader.getUVsPerSubMeshes(),
                loader.getTangentsPerSubMeshes());
            mVertexBuffer[BottomLevelASType::Sphere]
                = std::make_unique<VertexBuffer>(device, vertices, L"VertexBuffer_Sphere");

            resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
            resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
            mIndexOffsets[LocalRootSignature::HitGroupIndex::Sphere] = (UINT)indices.size();
            mVertexOffsets[LocalRootSignature::HitGroupIndex::Sphere] = (UINT)vertices.size();

            auto materialList = loader.getMaterialDatas();
            GlbMaterial material = {};
            if (!materialList.empty()) material = loader.getMaterialDatas()[0];
            auto descs = loader.getImageDatas();

            loadTextureIfExist(!descs.empty(), ModelTextureType::Sphere_AlbedoTexture, descs, 0,
                DescriptorHeapIndex::Sphere_Albedo, ModelTextureType::Default_AlbedoTexture);
            loadTextureIfExist(material.normalMapID != -1, ModelTextureType::Sphere_NormalMap,
                descs, material.normalMapID, DescriptorHeapIndex::Sphere_NormalMap,
                ModelTextureType::Default_NormalMap);
            loadTextureIfExist(material.metallicRoughnessMapID != -1,
                ModelTextureType::Sphere_MetallicRoughnessMap, descs,
                material.metallicRoughnessMapID, DescriptorHeapIndex::Sphere_MetallicRoughnessMap,
                ModelTextureType::Default_MetallicRoughnessMap);
            loadTextureIfExist(material.emissiveMapID != -1, ModelTextureType::Sphere_EmissiveMap,
                descs, material.emissiveMapID, DescriptorHeapIndex::Sphere_EmissiveMap,
                ModelTextureType::Default_EmissiveMap);
            loadTextureIfExist(material.occlusionMapID != -1, ModelTextureType::Sphere_OcclusionMap,
                descs, material.occlusionMapID, DescriptorHeapIndex::Sphere_OcclusionMap,
                ModelTextureType::Default_OcclusionMap);
        }
    }
    {
        ID3D12Device* device = mDeviceResource->getDevice();
        ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
        ID3D12CommandAllocator* allocator = mDeviceResource->getCommandAllocator();
        ID3D12Device5* dxrDevice = mDXRDevice.getDXRDevice();
        ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice.getDXRCommandList();

        commandList->Reset(allocator, nullptr);

        for (int i = 0; i < BottomLevelASType::Count; i++) {
            mBLASBuffers[i] = std::make_unique<BottomLevelAccelerationStructure>(mDXRDevice,
                *mVertexBuffer[i], static_cast<UINT>(sizeof(Vertex)), *mIndexBuffer[i],
                static_cast<UINT>(sizeof(Index)));
        }

        mTLASBuffer = std::make_unique<TopLevelAccelerationStructure>();

        mDeviceResource->executeCommandList();
        mDeviceResource->waitForGPU();

        mResourcesIndexBuffer = std::make_unique<IndexBuffer>(device, resourceIndices,
            D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"ResourceIndex");
        mResourcesIndexBuffer->setCPUHandle(
            mDescriptorTable->getCPUHandle(DescriptorHeapIndex::ResourceIndexBuffer));
        mResourcesIndexBuffer->setGPUHandle(
            mDescriptorTable->getGPUHandle(DescriptorHeapIndex::ResourceIndexBuffer));
        mResourcesIndexBuffer->createSRV(device);

        mResourcesVertexBuffer
            = std::make_unique<VertexBuffer>(device, resourceVertices, L"ResourceVertex");
        mResourcesVertexBuffer->setCPUHandle(
            mDescriptorTable->getCPUHandle(DescriptorHeapIndex::ResourceVertexBuffer));
        mResourcesVertexBuffer->setGPUHandle(
            mDescriptorTable->getGPUHandle(DescriptorHeapIndex::ResourceVertexBuffer));
        mResourcesVertexBuffer->createSRV(device);
    }
}

{

    mDXRStateObject = std::make_unique<DXRPipelineStateObject>(&mDXRDevice);
    mDXRStateObject->exportShader((void*)g_pMiss, _countof(g_pMiss), MISS_SHADER_NAME);
    mDXRStateObject->exportShader(
        (void*)g_pClosestHit_Normal, _countof(g_pClosestHit_Normal), CLOSEST_HIT_NORMAL_NAME);
    mDXRStateObject->exportShader(
        (void*)g_pClosestHit_Plane, _countof(g_pClosestHit_Plane), CLOSEST_HIT_PLANE_NAME);
    mDXRStateObject->exportShader(
        (void*)g_pClosestHit_Sphere, _countof(g_pClosestHit_Sphere), CLOSEST_HIT_SPHERE_NAME);
    mDXRStateObject->exportShader((void*)g_pRayGenShader, _countof(g_pRayGenShader), RAY_GEN_NAME);
    mDXRStateObject->exportShader((void*)g_pShadow, _countof(g_pShadow), MISS_SHADOW_SHADER_NAME);

    mDXRStateObject->bindHitGroup({ HIT_GROUP_UFO_NAME,
        D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_NORMAL_NAME });
    mDXRStateObject->bindHitGroup({ HIT_GROUP_QUAD_NAME,
        D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_NORMAL_NAME });
    mDXRStateObject->bindHitGroup({ HIT_GROUP_FLOOR_NAME,
        D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_PLANE_NAME });
    mDXRStateObject->bindHitGroup({ HIT_GROUP_SPHERE_NAME,
        D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_SPHERE_NAME });

    UINT payloadSize
        = Framework::Math::MathUtil::mymax<UINT>({ sizeof(RayPayload), sizeof(ShadowPayload) });
    UINT attrSize = sizeof(float) * 2;
    UINT maxRecursionDepth = MAX_RAY_RECURSION_DEPTH;
    mDXRStateObject->setConfig(payloadSize, attrSize, maxRecursionDepth);

    mDXRStateObject->bindLocalRootSignature(*mMissLocalRootSignature, MISS_SHADER_NAME);
    mDXRStateObject->bindLocalRootSignature(*mHitGroupLocalRootSignature, HIT_GROUP_UFO_NAME);
    mDXRStateObject->bindLocalRootSignature(*mHitGroupLocalRootSignature, HIT_GROUP_QUAD_NAME);
    mDXRStateObject->bindLocalRootSignature(*mHitGroupLocalRootSignature, HIT_GROUP_FLOOR_NAME);
    mDXRStateObject->bindLocalRootSignature(*mHitGroupLocalRootSignature, HIT_GROUP_SPHERE_NAME);

    mDXRStateObject->bindGlobalRootSignature(*mGlobalRootSignature);

    mDXRStateObject->createPipeline();
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::RayGenShader, { ShaderType::RayGeneration, RAY_GEN_NAME });
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::MissShader, { ShaderType::Miss, MISS_SHADER_NAME });
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::MissShadowShader, { ShaderType::Miss, MISS_SHADOW_SHADER_NAME });
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::HitGroup_UFO, { ShaderType::HitGroup, HIT_GROUP_UFO_NAME });
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::HitGroup_Quad, { ShaderType::HitGroup, HIT_GROUP_QUAD_NAME });
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::HitGroup_Floor, { ShaderType::HitGroup, HIT_GROUP_FLOOR_NAME });
    mDXRStateObject->associateShaderInfoWithKey(
        ShaderKey::HitGroup_Sphere, { ShaderType::HitGroup, HIT_GROUP_SPHERE_NAME });

    mDXRStateObject->prebuild();

    mDXRStateObject->setShaderTableConfig(ShaderType::RayGeneration, 1, 0, L"RayGenShaderTable");
    mDXRStateObject->buildShaderTable(ShaderKey::RayGenShader);
    {
        struct RootArgument {
            MissConstant cb;
        } rootArgument;
        rootArgument.cb.back = Color(188.0f / 255.0f, 226.0f / 255.0f, 232.0f / 255.0f, 1.0f);

        mDXRStateObject->setShaderTableConfig(
            ShaderType::Miss, 2, sizeof(RootArgument), L"MissShaderTable");
        mDXRStateObject->buildShaderTable(ShaderKey::MissShader, &rootArgument);
        mDXRStateObject->buildShaderTable(ShaderKey::MissShadowShader);
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

        auto setRootArgument = [&](LocalRootSignature::HitGroupIndex::MyEnum hitGroupIndex,
                                   ModelTextureType texOffset) {
            RootArgument arg;
            arg.cb.indexOffset = std::get<0>(getOffset(hitGroupIndex));
            arg.cb.vertexOffset = std::get<1>(getOffset(hitGroupIndex));
            arg.albedo = mTextures[mTextureIDs[texOffset]]->getGPUHandle();
            arg.normal = mTextures[mTextureIDs[ModelTextureType(texOffset + 1)]]->getGPUHandle();
            arg.metallicRoughness
                = mTextures[mTextureIDs[ModelTextureType(texOffset + 2)]]->getGPUHandle();
            arg.emissive = mTextures[mTextureIDs[ModelTextureType(texOffset + 3)]]->getGPUHandle();
            arg.occlusion = mTextures[mTextureIDs[ModelTextureType(texOffset + 4)]]->getGPUHandle();
            return arg;
        };

        mDXRStateObject->buildShaderTable(ShaderKey::HitGroup_UFO,
            &setRootArgument(
                LocalRootSignature::HitGroupIndex::UFO, ModelTextureType::UFO_AlbedoTexture));

        mDXRStateObject->buildShaderTable(ShaderKey::HitGroup_Quad,
            &setRootArgument(
                LocalRootSignature::HitGroupIndex::Quad, ModelTextureType::Quad_AlbedoTexture));

        mDXRStateObject->buildShaderTable(ShaderKey::HitGroup_Floor,
            &setRootArgument(
                LocalRootSignature::HitGroupIndex::Floor, ModelTextureType::Plane_AlbedoTexture));

        mDXRStateObject->buildShaderTable(ShaderKey::HitGroup_Sphere,
            &setRootArgument(
                LocalRootSignature::HitGroupIndex::Sphere, ModelTextureType::Sphere_AlbedoTexture));

        mDXRStateObject->build();
    }
}
}

void Scene::releaseDeviceDependentResources() {}

void Scene::createWindowDependentResources() {
    ID3D12Device* device = mDeviceResource->getDevice();
    DXGI_FORMAT backBufferFormat = mDeviceResource->getBackBufferFormat();

    auto uavResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(backBufferFormat, mWidth, mHeight, 1, 1, 1,
        0, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
    MY_THROW_IF_FAILED(
        device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &uavResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
            IID_PPV_ARGS(&mRaytracingOutput.mResource)));
    mRaytracingOutput.mResource->SetName(L"RaytracingOutput");

    mRaytracingOutput.setCPUHandle(
        mDescriptorTable->getCPUHandle(DescriptorHeapIndex::RaytracingOutput));
    mRaytracingOutput.setGPUHandle(
        mDescriptorTable->getGPUHandle(DescriptorHeapIndex::RaytracingOutput));

    D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
    device->CreateUnorderedAccessView(
        mRaytracingOutput.mResource.Get(), nullptr, &uavDesc, mRaytracingOutput.mCPUHandle);
}

void Scene::releaseWindowDependentResources() {
    mRaytracingOutput.mResource.Reset();
}
