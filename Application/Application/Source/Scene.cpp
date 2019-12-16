#include "Scene.h"
#include <DirectXMath.h>
#include <numeric>
#include "DX/Raytracing/Shader/ShaderTable.h"
#include "DX/Util/Helper.h"
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
using namespace Framework::Desc;
using namespace Framework::Utility;
using namespace DirectX;

namespace {

    Comptr<ID3D12DescriptorHeap> mGlobalHeap;
    Comptr<ID3D12DescriptorHeap> mLocalHeap;
    UINT mTLASBufferIndex = 0;

    UINT mGlobalHeapCount;
    UINT mHeapSize;
    UINT mLocalHeapAllocatedCount;

#define CPU_HANDLE                 \
    CD3DX12_CPU_DESCRIPTOR_HANDLE( \
        mLocalHeap->GetCPUDescriptorHandleForHeapStart(), mLocalHeapAllocatedCount, mHeapSize)
#define GPU_HANDLE                 \
    CD3DX12_GPU_DESCRIPTOR_HANDLE( \
        mLocalHeap->GetGPUDescriptorHandleForHeapStart(), mLocalHeapAllocatedCount, mHeapSize)

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
    static const std::wstring MISS_SHADER_NAME = L"Miss";
    static const std::wstring MISS_SHADOW_SHADER_NAME = L"Shadow";
    static const std::wstring RAY_GEN_NAME = L"RayGenShader";

    struct HitGroup {
        const unsigned char* shader;
        UINT shaderSize;
        std::wstring closestHitNames;
        std::wstring hitGroupName;
    };

    static const std::vector<HitGroup> HIT_GROUP_NAMES = {
        { g_pClosestHit_Normal, _countof(g_pClosestHit_Normal), L"ClosestHit_Normal",
            L"HitGroup_UFO" },
        { g_pClosestHit_Plane, _countof(g_pClosestHit_Plane), L"ClosestHit_Plane",
            L"HitGroup_Floor" },
        { g_pClosestHit_Sphere, _countof(g_pClosestHit_Sphere), L"ClosestHit_Sphere",
            L"HitGroup_Sphere" },
    };

    static const std::unordered_map<BottomLevelASType::MyEnum, std::wstring> MODEL_NAMES = {
        { BottomLevelASType::UFO, L"UFO.glb" },
        { BottomLevelASType::Sphere, L"sphere.glb" },
        { BottomLevelASType::Floor, L"field.glb" },
    };

    static constexpr UINT UFO_COUNT = 1;
    static constexpr UINT FLOOR_COUNT = 1;
    static int SPHERE_COUNT = 100;

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

    /**
     * @enum enum
     * @brief description
     */
    enum class TextureType {
        Albedo,
        NormalMap,
        MetallicRoughnessMap,
        EmissiveMap,
        OcclusionMap,
    };
    std::unordered_map<TextureType, std::shared_ptr<Texture2D>> mDefaultTextures;

    /**
     * @class GLBModel
     * @brief .glbから作成される3Dモデル
     */
    class GLBModel {
        using TexturePtr = std::shared_ptr<Texture2D>;

    public:
        /**
         * @brief
         */
        GLBModel() {}
        /**
         * @brief
         */
        ~GLBModel() {}
        void init(DeviceResource* device, ID3D12GraphicsCommandList* commandList,
            const std::filesystem::path& filepath, UINT& heapIndex) {
            Framework::Utility::GLBLoader loader(filepath);
            //インデックス配列を二次元配列から線形に変換する
            std::vector<IndexList> indices = loader.getIndicesPerSubMeshes();

            mIndices = toLinearList(indices);
            //頂点配列を線形に変換する
            std::vector<PositionList> positions = loader.getPositionsPerSubMeshes();
            std::vector<NormalList> normals = loader.getNormalsPerSubMeshes();
            std::vector<UVList> uvs = loader.getUVsPerSubMeshes();
            std::vector<TangentList> tangents = loader.getTangentsPerSubMeshes();

            mVertices = toLinearVertices(positions, normals, uvs, tangents);

            mIndexBuffer.init(device, mIndices,
                D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
                filepath.filename().wstring() + L"Index");
            mVertexBuffer.init(device, mVertices, filepath.filename().wstring() + L"Vertex");

            //マテリアルを読み込む
            std::vector<GlbMaterial> materials = loader.getMaterialDatas();
            //マテリアルがすれば最初のマテリアルを、存在しなければデフォルトのマテリアルを使用する
            GlbMaterial material = materials.empty() ? GlbMaterial{} : materials[0];
            std::vector<TextureDesc> descs = loader.getImageDatas();
            //テクスチャの読み込み処理
            mAlbedoTexture = createTextureIfExist(device, commandList, TextureType::Albedo,
                !descs.empty(), descs, material, heapIndex);
            mNormalMapTexture = createTextureIfExist(device, commandList, TextureType::NormalMap,
                material.normalMapID != -1, descs, material, heapIndex);
            mMetallicRoughnessTexture
                = createTextureIfExist(device, commandList, TextureType::MetallicRoughnessMap,
                    material.metallicRoughnessMapID != -1, descs, material, heapIndex);
            mEmissiveMapTexture = createTextureIfExist(device, commandList,
                TextureType::EmissiveMap, material.emissiveMapID != -1, descs, material, heapIndex);
            mOcclusionMapTexture
                = createTextureIfExist(device, commandList, TextureType::OcclusionMap,
                    material.occlusionMapID != -1, descs, material, heapIndex);
        }
        UINT getDescIndex(TextureType type, const GlbMaterial& mat) {
            switch (type) {
            case TextureType::Albedo: return 0;
            case TextureType::NormalMap: return mat.normalMapID;
            case TextureType::MetallicRoughnessMap: return mat.metallicRoughnessMapID;
            case TextureType::EmissiveMap: return mat.emissiveMapID;
            case TextureType::OcclusionMap: return mat.occlusionMapID;
            default: return 0;
            }
        }
        TexturePtr createTextureIfExist(DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, TextureType type, bool expr,
            const std::vector<TextureDesc>& descs, const GlbMaterial& material, UINT& heapIndex) {
            if (expr) {
                UINT descIndex = getDescIndex(type, material);
                auto tex = createTexture(device, commandList, descs[0], CPU_HANDLE, GPU_HANDLE);
                mLocalHeapAllocatedCount++;
                heapIndex++;
                return tex;
            } else {
                return mDefaultTextures[type];
            }
        }

        TexturePtr createTexture(DeviceResource* device, ID3D12GraphicsCommandList* commandList,
            const TextureDesc& desc, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle) {
            TexturePtr texture = std::make_shared<Texture2D>();
            texture->init(device, commandList, desc);
            texture->createSRV(device, cpuHandle, gpuHandle);
            return texture;
        }

        //private:
        std::vector<Index> mIndices;
        std::vector<Vertex> mVertices;
        VertexBuffer mVertexBuffer;
        IndexBuffer mIndexBuffer;
        TexturePtr mAlbedoTexture;
        TexturePtr mNormalMapTexture;
        TexturePtr mMetallicRoughnessTexture;
        TexturePtr mEmissiveMapTexture;
        TexturePtr mOcclusionMapTexture;
        ShaderKey::MyEnum mShaderKey;
        UINT mVertexOffset;
        UINT mIndexOffset;
    };

    std::unordered_map<ModelType::Enum, GLBModel> mModels;
} // namespace

Scene::Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager,
    UINT width, UINT height)
    : mDeviceResource(device),
      mInputManager(inputManager),
      mDXRDevice(),
      mWidth(width),
      mHeight(height) {
    //Framework::Desc::DescriptorTableDesc desc = { L"ResourceTable", 10000,
    //    Framework::Desc::HeapType::CBV_SRV_UAV, Framework::Desc::HeapFlag::ShaderVisible };
    //mDescriptorTable = std::make_unique<DescriptorTable>(device->getDevice(), desc);

    //Global
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = 10000;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        MY_THROW_IF_FAILED(
            mDeviceResource->getDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mGlobalHeap)));
    }
    //Local
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = 10000;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        MY_THROW_IF_FAILED(
            mDeviceResource->getDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mLocalHeap)));
    }
    mHeapSize = mDeviceResource->getDevice()->GetDescriptorHandleIncrementSize(
        D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    mGlobalHeapCount = 0;
    mLocalHeapAllocatedCount = 0;

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
        mSceneCB.createCBV(mDeviceResource, CPU_HANDLE, GPU_HANDLE);
        mLocalHeapAllocatedCount++;
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
        ImGui::DragInt("Count", &SPHERE_COUNT);
    }
    ImGui::End();

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
        XMMATRIX transform = XMMatrixScaling(50, 1, 50) * XMMatrixTranslation(-100, -20, -100);
        instanceDesc.hitGroupIndex = LocalRootSignature::HitGroupIndex::Floor;
        instanceDesc.blas = mBLASBuffers[ModelType::Floor].get();
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
        instanceDesc.blas = mBLASBuffers[ModelType::Sphere].get();
        instanceDesc.transform = transform;
        mTLASBuffer->add(instanceDesc);
    }

    mTLASBuffer->build(mDXRDevice, mDeviceResource,
        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
        CPU_HANDLE, GPU_HANDLE);

    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature->getRootSignature());
    mSceneCB.updateStaging();

    ID3D12DescriptorHeap* heaps[] = { mGlobalHeap.Get() };
    commandList->SetDescriptorHeaps(_countof(heaps), heaps);

    UINT index = 0;
    {
        D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle[] = { mSceneCB.getView().mCPUHandle };
        UINT count = _countof(cbvHandle);
        D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            mGlobalHeap->GetCPUDescriptorHandleForHeapStart(), index, mHeapSize);
        mDeviceResource->getDevice()->CopyDescriptors(1, &dstHandle, &count, count, cbvHandle,
            nullptr, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        commandList->SetComputeRootDescriptorTable(0,
            CD3DX12_GPU_DESCRIPTOR_HANDLE(
                mGlobalHeap->GetGPUDescriptorHandleForHeapStart(), index, mHeapSize));
        index += count;
    }
    {
        D3D12_CPU_DESCRIPTOR_HANDLE srvHandle[] = { mTLASBuffer->getView().getCPUHandle(),
            mResourceIndexBufferSRV.getCPUHandle(), mResourceVertexBufferSRV.getCPUHandle() };
        UINT count = _countof(srvHandle);
        D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            mGlobalHeap->GetCPUDescriptorHandleForHeapStart(), index, mHeapSize);
        mDeviceResource->getDevice()->CopyDescriptors(1, &dstHandle, &count, count, srvHandle,
            nullptr, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        commandList->SetComputeRootDescriptorTable(1,
            CD3DX12_GPU_DESCRIPTOR_HANDLE(
                mGlobalHeap->GetGPUDescriptorHandleForHeapStart(), index, mHeapSize));
        index += count;
    }
    {
        D3D12_CPU_DESCRIPTOR_HANDLE uavHandle[] = { mRaytracingOutputUAV.getCPUHandle() };
        UINT count = _countof(uavHandle);
        D3D12_CPU_DESCRIPTOR_HANDLE dstHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            mGlobalHeap->GetCPUDescriptorHandleForHeapStart(), index, mHeapSize);
        mDeviceResource->getDevice()->CopyDescriptors(1, &dstHandle, &count, count, uavHandle,
            nullptr, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        commandList->SetComputeRootDescriptorTable(2,
            CD3DX12_GPU_DESCRIPTOR_HANDLE(
                mGlobalHeap->GetGPUDescriptorHandleForHeapStart(), index, mHeapSize));
        index += count;
    }

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
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
        ranges[GlobalRootSignature::Slot::Srv].Init(
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0);
        ranges[GlobalRootSignature::Slot::Uav].Init(
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

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
        range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);
        range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4);
        range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5);
        range[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 6);
        range[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 7);

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
                    desc.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
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
                auto createDefaultTexture
                    = [&](const std::wstring& name, const Color4& col, UINT heapIndex) {
                          std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
                          texture->init(mDeviceResource, commandList, createUnitTexture(col, name));

                          texture->createSRV(mDeviceResource, CPU_HANDLE, GPU_HANDLE);
                          mLocalHeapAllocatedCount++;
                          return texture;
                      };

                mDefaultTextures[TextureType::Albedo] = createDefaultTexture(L"DefaultAlbedo",
                    Color4(1, 1, 1, 1), DescriptorHeapIndex::DefaultTexture_Albedo);
                mDefaultTextures[TextureType::NormalMap] = createDefaultTexture(L"DefaultNormal",
                    Color4(0.5f, 0.5f, 1.0f, 1.0f), DescriptorHeapIndex::DefaultTexture_NormalMap);
                mDefaultTextures[TextureType::MetallicRoughnessMap]
                    = createDefaultTexture(L"DefaultMetallicRoughness", Color4(0, 0, 0, 1),
                        DescriptorHeapIndex::DefaultTexture_MetallicRoughnessMap);
                mDefaultTextures[TextureType::EmissiveMap]
                    = createDefaultTexture(L"DefaultEmissive", Color4(0, 0, 0, 1),
                        DescriptorHeapIndex::DefaultTexture_EmissiveMap);
                mDefaultTextures[TextureType::OcclusionMap]
                    = createDefaultTexture(L"DefaultOcculusion", Color4(1, 1, 1, 1),
                        DescriptorHeapIndex::DefaultTexture_OcclusionMap);
            }
            UINT heapStartIndex = DescriptorHeapIndex::ModelTextureStart;
            UINT vertexOffset = 0;
            UINT indexOffset = 0;

            {
                GLBModel model;
                model.init(mDeviceResource, commandList,
                    modelPath / MODEL_NAMES.at(BottomLevelASType::UFO), heapStartIndex);
                resourceIndices.insert(
                    resourceIndices.end(), model.mIndices.begin(), model.mIndices.end());
                resourceVertices.insert(
                    resourceVertices.end(), model.mVertices.begin(), model.mVertices.end());
                model.mVertexOffset = vertexOffset;
                model.mIndexOffset = indexOffset;
                vertexOffset += model.mVertices.size();
                indexOffset += model.mIndices.size();
                model.mShaderKey = ShaderKey::HitGroup_UFO;
                mModels[ModelType::UFO] = model;
            }
            {
                GLBModel model;
                model.init(mDeviceResource, commandList,
                    modelPath / MODEL_NAMES.at(BottomLevelASType::Floor), heapStartIndex);
                resourceIndices.insert(
                    resourceIndices.end(), model.mIndices.begin(), model.mIndices.end());
                resourceVertices.insert(
                    resourceVertices.end(), model.mVertices.begin(), model.mVertices.end());
                model.mVertexOffset = vertexOffset;
                model.mIndexOffset = indexOffset;
                vertexOffset += model.mVertices.size();
                indexOffset += model.mIndices.size();
                model.mShaderKey = ShaderKey::HitGroup_Floor;
                mModels[ModelType::Floor] = model;
            }
            {
                GLBModel model;
                model.init(mDeviceResource, commandList,
                    modelPath / MODEL_NAMES.at(BottomLevelASType::Sphere), heapStartIndex);
                resourceIndices.insert(
                    resourceIndices.end(), model.mIndices.begin(), model.mIndices.end());
                resourceVertices.insert(
                    resourceVertices.end(), model.mVertices.begin(), model.mVertices.end());
                model.mVertexOffset = vertexOffset;
                model.mIndexOffset = indexOffset;
                vertexOffset += model.mVertices.size();
                indexOffset += model.mIndices.size();
                model.mShaderKey = ShaderKey::HitGroup_Sphere;
                mModels[ModelType::Sphere] = model;
            }
        }
        {
            ID3D12Device* device = mDeviceResource->getDevice();
            ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
            ID3D12CommandAllocator* allocator = mDeviceResource->getCommandAllocator();
            ID3D12Device5* dxrDevice = mDXRDevice.getDXRDevice();
            ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice.getDXRCommandList();

            for (auto&& model : mModels) {
                mBLASBuffers[model.first] = std::make_unique<BottomLevelAccelerationStructure>();
                mBLASBuffers[model.first]->init(mDXRDevice, model.second.mVertexBuffer,
                    static_cast<UINT>(sizeof(Vertex)), model.second.mIndexBuffer,
                    static_cast<UINT>(sizeof(Index)));
            }

            mTLASBuffer = std::make_unique<TopLevelAccelerationStructure>();

            mDeviceResource->executeCommandList();
            mDeviceResource->waitForGPU();

            mResourcesIndexBuffer.init(mDeviceResource, resourceIndices,
                D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, L"ResourceIndex");
            mResourceIndexBufferSRV
                = mResourcesIndexBuffer.createSRV(mDeviceResource, CPU_HANDLE, GPU_HANDLE);
            mLocalHeapAllocatedCount++;

            mResourcesVertexBuffer.init(mDeviceResource, resourceVertices, L"ResourceVertex");
            mResourceVertexBufferSRV.initAsBuffer(
                mDeviceResource, mResourcesVertexBuffer.getBuffer(), CPU_HANDLE, GPU_HANDLE);
            mLocalHeapAllocatedCount++;
        }
    }

    {

        mDXRStateObject = std::make_unique<DXRPipelineStateObject>(&mDXRDevice);

        mDXRStateObject->exportShader((void*)g_pMiss, _countof(g_pMiss), MISS_SHADER_NAME);

        UINT key = ShaderKey::HitGroup_UFO;
        for (auto&& shader : HIT_GROUP_NAMES) {
            mDXRStateObject->exportShader(
                (void*)shader.shader, shader.shaderSize, shader.closestHitNames);
            mDXRStateObject->bindHitGroup({ shader.hitGroupName,
                D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, shader.closestHitNames });
            mDXRStateObject->bindLocalRootSignature(
                *mHitGroupLocalRootSignature, shader.hitGroupName);
            mDXRStateObject->associateShaderInfoWithKey(
                key++, ShaderType::HitGroup, shader.hitGroupName);
        }
        mDXRStateObject->exportShader(
            (void*)g_pRayGenShader, _countof(g_pRayGenShader), RAY_GEN_NAME);
        mDXRStateObject->exportShader(
            (void*)g_pShadow, _countof(g_pShadow), MISS_SHADOW_SHADER_NAME);

        UINT payloadSize
            = Framework::Math::MathUtil::mymax<UINT>({ sizeof(RayPayload), sizeof(ShadowPayload) });
        UINT attrSize = sizeof(float) * 2;
        UINT maxRecursionDepth = MAX_RAY_RECURSION_DEPTH;
        mDXRStateObject->setConfig(payloadSize, attrSize, maxRecursionDepth);

        mDXRStateObject->bindLocalRootSignature(*mMissLocalRootSignature, MISS_SHADER_NAME);

        mDXRStateObject->bindGlobalRootSignature(*mGlobalRootSignature);

        mDXRStateObject->associateShaderInfoWithKey(
            ShaderKey::RayGenShader, ShaderType::RayGeneration, RAY_GEN_NAME);
        mDXRStateObject->associateShaderInfoWithKey(
            ShaderKey::MissShader, ShaderType::Miss, MISS_SHADER_NAME);
        mDXRStateObject->associateShaderInfoWithKey(
            ShaderKey::MissShadowShader, ShaderType::Miss, MISS_SHADOW_SHADER_NAME);
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

            auto setRootArgument = [&](const GLBModel& model) {
                RootArgument arg;
                arg.cb.indexOffset = model.mIndexOffset;
                arg.cb.vertexOffset = model.mVertexOffset;
                arg.albedo = model.mAlbedoTexture->getView().getGPUHandle();
                arg.normal = model.mNormalMapTexture->getView().getGPUHandle();
                arg.metallicRoughness = model.mMetallicRoughnessTexture->getView().getGPUHandle();
                arg.emissive = model.mEmissiveMapTexture->getView().getGPUHandle();
                arg.occlusion = model.mOcclusionMapTexture->getView().getGPUHandle();
                return arg;
            };

            for (auto&& model : mModels) {
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
        mDeviceResource, mRaytracingOutput, CPU_HANDLE, GPU_HANDLE);
    mLocalHeapAllocatedCount++;
}

void Scene::releaseWindowDependentResources() {}
