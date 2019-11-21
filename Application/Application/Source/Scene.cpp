#include "Scene.h"
#include <numeric>
#include "DX/Raytracing/Helper.h"
#include "Utility/Debug.h"
#include "Utility/IO/GLBLoader.h"
#include "Utility/IO/TextureLoader.h"
#include "Utility/Path.h"
#include "Utility/StringUtil.h"
#include "Framework/DX/GLBModel.h"
#include "ImGui/ImGuiManager.h"

#include "CompiledShaders/Miss.hlsl.h"
#include "CompiledShaders/ClosestHit_Normal.hlsl.h"
#include "CompiledShaders/ClosestHit_Plane.hlsl.h"
#include "CompiledShaders/RayGenShader.hlsl.h"
#include "CompiledShaders/Shadow.hlsl.h"

#include "Application/Assets/Shader/Raytracing/Util/MissCompat.h"
#include "Application/Assets/Shader/Raytracing/Util/HitGroupCompat.h"

using namespace Framework::DX;
using namespace Framework::Utility;

namespace {
    static const std::wstring MISS_SHADER_NAME = L"Miss";
    static const std::wstring MISS_SHADOW_SHADER_NAME = L"Shadow";
    static const std::wstring CLOSEST_HIT_NORMAL_NAME = L"ClosestHit_Normal";
    static const std::wstring CLOSEST_HIT_PLANE_NAME = L"ClosestHit_Plane";
    static const std::wstring RAY_GEN_NAME = L"RayGenShader";
    static const std::wstring HIT_GROUP_SPHERE_NAME = L"HitGroup_Sphere";
    static const std::wstring HIT_GROUP_QUAD_NAME = L"HitGroup_Quad";
    static const std::wstring HIT_GROUP_FLOOR_NAME = L"HitGroup_Floor";

    static const std::unordered_map<BottomLevelASType::MyEnum, std::wstring> MODEL_NAMES =
    {
        {BottomLevelASType::UFO , L"igloo.glb" },
        {BottomLevelASType::Floor , L"floor.glb" },
    };

    static constexpr UINT TRIANGLE_COUNT = 1;
    static constexpr UINT QUAD_COUNT = 0;
    static constexpr UINT FLOOR_COUNT = 1;
    static constexpr UINT TLAS_NUM = TRIANGLE_COUNT + QUAD_COUNT + FLOOR_COUNT;

    inline void createBuffer(ID3D12Device* device, ID3D12Resource** resource, void* data, UINT64 size, LPCWSTR name = nullptr) {
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

    using PositionList = std::vector<Framework::Math::Vector3>;
    using NormalList = std::vector<Framework::Math::Vector3>;
    using UVList = std::vector<Framework::Math::Vector2>;
    using TangentList = std::vector<Framework::Math::Vector4>;

    inline std::vector<Framework::DX::Vertex> toLinearVertices(
        const std::vector<PositionList>& positions,
        const std::vector<NormalList>& normals = {},
        const std::vector<UVList>& uvs = {},
        const std::vector<TangentList>& tangents = {}) {
        std::vector<Framework::DX::Vertex> res;
        for (size_t i = 0; i < positions.size(); i++) {
            for (size_t j = 0; j < positions[i].size(); j++) {
                Framework::DX::Vertex v;
                v.position = positions[i][j];
                v.normal = normals.empty() ? Vec3(0, 0, 0) : normals[i][j];
                v.uv = uvs.empty() ? Vec2(0, 0) : uvs[i][j];
                v.tangent = tangents.empty() ? Vec4(0, 0, 0, 0) : tangents[i][j];
                res.emplace_back(v);
            }
        }
        return res;
    }

    inline void allocateUAVBuffer(ID3D12Device* device, UINT64 bufferSize, ID3D12Resource** resource, D3D12_RESOURCE_STATES initResourceState, LPCWSTR name = nullptr) {
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

    Deg mQuadRotate = Deg(0.0f);
    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO topLevelPreInfo = {};
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC topLevelBuildDesc = {};

    enum  ModelTextureType {
        Default_Albedo,
        Default_NormalMap,
        Default_MetalRough,
        Default_Emissive,

        UFO_Albedo,
        UFO_Normal,
        UFO_MetalRough,
        UFO_Emissive,

        Quad_Albedo,

        Plane_Albedo,
        Plane_Normal,
        Plane_MetalRough,
        Plane_Emissive,
    };
    std::unordered_map<ModelTextureType, UINT> mTextureIDs;

    D3D12_GPU_DESCRIPTOR_HANDLE mDefaultAlbedoTexture;
    D3D12_GPU_DESCRIPTOR_HANDLE mDefaultNormalMapTexture;
    D3D12_GPU_DESCRIPTOR_HANDLE mDefaultMetalRoughTexture;
    D3D12_GPU_DESCRIPTOR_HANDLE mDefaultEmissiveTexture;

    std::shared_ptr<Framework::ImGUI::Text> mRotSphereText;

    ComPtr<ID3D12Resource> mInstanceDescs;
}

Scene::Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager, UINT width, UINT height)
    :mDeviceResource(device),
    mInputManager(inputManager),
    mDXRDevice(std::make_unique<DXRDevice>(mDeviceResource)),
    mWidth(width),
    mHeight(height) {
    mDebugWindow = std::make_unique<Framework::ImGUI::Window>("Debug");
    mFPSText = std::make_shared<Framework::ImGUI::Text>("FPS:");
    mDebugWindow->addItem(mFPSText);
    Framework::Desc::DescriptorTableDesc desc = { L"ResourceTable",10000,Framework::Desc::HeapType::CBV_SRV_UAV,Framework::Desc::HeapFlag::ShaderVisible };
    mDescriptorTable = std::make_unique<CountingDescriptorTable>(device->getDevice(), desc);
    mCameraPosition = Vec3(0, 0, -10);
    mLightPosition = Vec3(0, 100, -100);
    mLightDiffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    mLightAmbient = Color4(0.1f, 0.1f, 0.1f, 1.0f);

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

    mTextures.resize(TEXTURE_NUM);

    mRotSphereText = std::make_shared<Framework::ImGUI::Text>("Rot");
    mDebugWindow->addItem(mRotSphereText);
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

            CD3DX12_STATIC_SAMPLER_DESC sampler[1];
            sampler[0] = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

            CD3DX12_ROOT_SIGNATURE_DESC global(_countof(rootParams), rootParams, _countof(sampler), sampler);
            serializeAndCreateRootSignature(mDeviceResource->getDevice(),
                global, &mGlobalRootSignature);
        }
        //ローカルルートシグネチャを作る
        {
            //Missシェーダー用ローカルルートシグネチャ
            {
                CD3DX12_ROOT_PARAMETER rootParams[LocalRootSignature::Miss::Count];
                rootParams[0].InitAsConstants(align(sizeof(MissConstant), sizeof(UINT32)), 1);

                CD3DX12_ROOT_SIGNATURE_DESC local(_countof(rootParams), rootParams);
                local.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
                serializeAndCreateRootSignature(mDeviceResource->getDevice(),
                    local, &mMissLocalRootSignature);
            }
            //HitGroupシェーダー用ローカルルートシグネチャ
            {
                CD3DX12_DESCRIPTOR_RANGE range[4];
                range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); //アルベド
                range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4); //法線
                range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5); //金属、粗さ
                range[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 6); //発光

                CD3DX12_ROOT_PARAMETER params[LocalRootSignature::HitGroup::Constants::Count];
                UINT contSize = align(sizeof(HitGroupConstant), sizeof(UINT32));
                params[LocalRootSignature::HitGroup::Constants::Albedo].InitAsDescriptorTable(1, &range[0]);
                params[LocalRootSignature::HitGroup::Constants::Normal].InitAsDescriptorTable(1, &range[1]);
                params[LocalRootSignature::HitGroup::Constants::MetalRough].InitAsDescriptorTable(1, &range[2]);
                params[LocalRootSignature::HitGroup::Constants::Emissive].InitAsDescriptorTable(1, &range[3]);

                params[LocalRootSignature::HitGroup::Constants::SceneConstants].InitAsConstants(contSize, 1);

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
            auto exportShader = [&pipeline](void* shaderFile, SIZE_T shaderSize, const auto&... names) {
                CD3DX12_DXIL_LIBRARY_SUBOBJECT* lib = pipeline.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
                lib->SetDXILLibrary(&CD3DX12_SHADER_BYTECODE(shaderFile, shaderSize));
                for (auto&& name : std::initializer_list<std::wstring>{ names... }) {
                    lib->DefineExport(name.c_str());
                }
            };

            exportShader((void*)g_pMiss, _countof(g_pMiss), MISS_SHADER_NAME);
            exportShader((void*)g_pClosestHit_Normal, _countof(g_pClosestHit_Normal), CLOSEST_HIT_NORMAL_NAME);
            exportShader((void*)g_pClosestHit_Plane, _countof(g_pClosestHit_Plane), CLOSEST_HIT_PLANE_NAME);
            exportShader((void*)g_pRayGenShader, _countof(g_pRayGenShader), RAY_GEN_NAME);
            exportShader((void*)g_pShadow, _countof(g_pShadow), MISS_SHADOW_SHADER_NAME);
        }
        //HitGroupをまとめる
        {
            auto bindHitGroup = [&pipeline](const std::wstring& hitGroupName, D3D12_HIT_GROUP_TYPE type, const std::wstring& closestHit = L"",
                const std::wstring& anyHit = L"", const std::wstring& intersection = L"") {
                    CD3DX12_HIT_GROUP_SUBOBJECT* hitGroup = pipeline.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
                    if (!closestHit.empty()) hitGroup->SetClosestHitShaderImport(closestHit.c_str());
                    if (!anyHit.empty()) hitGroup->SetAnyHitShaderImport(anyHit.c_str());
                    if (!intersection.empty()) hitGroup->SetIntersectionShaderImport(intersection.c_str());
                    hitGroup->SetHitGroupExport(hitGroupName.c_str());
                    hitGroup->SetHitGroupType(type);
            };

            bindHitGroup(HIT_GROUP_SPHERE_NAME, D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_NORMAL_NAME);
            bindHitGroup(HIT_GROUP_QUAD_NAME, D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_NORMAL_NAME);
            bindHitGroup(HIT_GROUP_FLOOR_NAME, D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES, CLOSEST_HIT_PLANE_NAME);
        }
        //シェーダーコンフィグ
        {
            CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT* config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
            UINT payloadSize = Framework::Math::MathUtil::mymax<UINT>({ sizeof(RayPayload) ,sizeof(ShadowPayload) });
            UINT attrSize = sizeof(float) * 2;
            config->Config(payloadSize, attrSize);
        }
        //ローカルルートシグネチャの設定
        {
            auto bindLocalRootSignature = [&pipeline](ID3D12RootSignature* rootSig, const std::wstring& exportShader) {
                CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT* local = pipeline.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
                local->SetRootSignature(rootSig);

                CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT* asso = pipeline.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
                asso->SetSubobjectToAssociate(*local);
                asso->AddExport(exportShader.c_str());
            };

            bindLocalRootSignature(mMissLocalRootSignature.Get(), MISS_SHADER_NAME);
            bindLocalRootSignature(mHitGroupLocalRootSignature[LocalRootSignature::HitGroup::Normal].Get(), HIT_GROUP_SPHERE_NAME);
            bindLocalRootSignature(mHitGroupLocalRootSignature[LocalRootSignature::HitGroup::Normal].Get(), HIT_GROUP_QUAD_NAME);
            bindLocalRootSignature(mHitGroupLocalRootSignature[LocalRootSignature::HitGroup::Normal].Get(), HIT_GROUP_FLOOR_NAME);
        }
        //グローバルルートシグネチャの設定
        {
            CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT* global = pipeline.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
            global->SetRootSignature(mGlobalRootSignature.Get());
        }
        //パイプラインコンフィグ
        {
            CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT* config = pipeline.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
            UINT maxRecursionDepth = MAX_RAY_RECURSION_DEPTH;
            config->Config(maxRecursionDepth);
        }

        PrintStateObjectDesc(pipeline);
        MY_THROW_IF_FAILED(mDXRDevice->getDXRDevice()->CreateStateObject(pipeline, IID_PPV_ARGS(&mDXRStateObject)));
    }
    //ヒープの確保
    {
        using namespace Framework::Desc;
        DescriptorTableDesc desc = { L"DescriptorTable",10000,HeapType::CBV_SRV_UAV,HeapFlag::ShaderVisible };
        mDescriptorTable->create(mDeviceResource->getDevice(), desc);
    }

    std::array<UINT, LocalRootSignature::HitGroupIndex::Count > mIndexOffsets;
    std::array<UINT, LocalRootSignature::HitGroupIndex::Count > mVertexOffsets;
    auto getOffset = [&mIndexOffsets, &mVertexOffsets](LocalRootSignature::HitGroupIndex::MyEnum e) {
        if (e == 0)return std::make_tuple<UINT, UINT>(0, 0);
        else return std::make_tuple(std::accumulate(mIndexOffsets.begin(), mIndexOffsets.begin() + e, 0u), std::accumulate(mVertexOffsets.begin(), mVertexOffsets.begin() + e, 0u));
    };

    //ジオメトリを生成する
    {
        ID3D12Device* device = mDeviceResource->getDevice();
    //BLAS構築用のバッファ
        std::array< IBuffer, BottomLevelASType::Count> mIndexBuffer;
        std::array<IBuffer, BottomLevelASType::Count> mVertexBuffer;

        std::vector<Framework::DX::Vertex> resourceVertices;
        std::vector<Index> resourceIndices;
        {

            auto path = Framework::Utility::ExePath::getInstance()->exe();
            path = path.remove_filename();
            auto modelPath = path / "Resources" / "Model";
            auto texPath = path / "Resources" / "Texture";

            //デフォルトのテクスチャ読み込み
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
                {
                    auto texture = std::make_shared<Texture2D>(device, createUnitTexture(Color4(1, 1, 1, 1), L"DefaultAlbedo"));
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[ModelTextureType::Default_Albedo] = (texture);
                    mTextureIDs[ModelTextureType::Default_Albedo] = ModelTextureType::Default_Albedo;
                }
                {
                    auto texture = std::make_shared<Texture2D>(device, createUnitTexture(Color4(0.5f, 0.5f, 1.0f, 1.0f), L"DefaultNormal"));
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[ModelTextureType::Default_NormalMap] = (texture);
                    mTextureIDs[ModelTextureType::Default_NormalMap] = ModelTextureType::Default_NormalMap;
                }
                {
                    auto texture = std::make_shared<Texture2D>(device, createUnitTexture(Color4(0, 0, 0, 1), L"DefaultMetallicRoughness"));
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[ModelTextureType::Default_MetalRough] = (texture);
                    mTextureIDs[ModelTextureType::Default_MetalRough] = ModelTextureType::Default_MetalRough;
                }
                {
                    auto texture = std::make_shared<Texture2D>(device, createUnitTexture(Color4(0, 0, 0, 1), L"DefaultEmissive"));
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[ModelTextureType::Default_Emissive] = (texture);
                    mTextureIDs[ModelTextureType::Default_Emissive] = ModelTextureType::Default_Emissive;
                }
            }

            auto loadTextures = [&](const Material& material, const std::vector<Framework::Desc::TextureDesc>& textureDatas, ModelTextureType idOffset) {
                ModelTextureType nextID = idOffset;
                if (textureDatas.empty()) {
                    mTextureIDs[nextID] = mTextureIDs[ModelTextureType::Default_Albedo];
                }
                else {
                    auto albedo = textureDatas[0];
                    auto texture = std::make_shared<Texture2D>(device, albedo);
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[nextID] = (texture);
                    mTextureIDs[nextID] = nextID;
                }
                nextID = ModelTextureType(nextID + 1);
                if (material.normalMapID == -1) {
                    mTextureIDs[nextID] = mTextureIDs[ModelTextureType::Default_NormalMap];
                }
                else {
                    auto normal = textureDatas[material.normalMapID];
                    auto texture = std::make_shared<Texture2D>(device, normal);
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[nextID] = (texture);
                    mTextureIDs[nextID] = nextID;
                }

                nextID = ModelTextureType(nextID + 1);
                if (material.metalRoughID == -1) {
                    mTextureIDs[nextID] = mTextureIDs[ModelTextureType::Default_MetalRough];
                }
                else {
                    auto metalRough = textureDatas[material.metalRoughID];
                    auto texture = std::make_shared<Texture2D>(device, metalRough);
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[nextID] = (texture);
                    mTextureIDs[nextID] = nextID;
                }

                nextID = ModelTextureType(nextID + 1);
                if (material.emissiveMapID == -1) {
                    mTextureIDs[nextID] = mTextureIDs[ModelTextureType::Default_Emissive];
                }
                else {
                    auto emissive = textureDatas[material.emissiveMapID];
                    auto texture = std::make_shared<Texture2D>(device, emissive);
                    mDescriptorTable->allocate(texture.get());
                    texture->createSRV(device);
                    mTextures[nextID] = (texture);
                    mTextureIDs[nextID] = nextID;
                }
            };

           //UFOのバッファ作成
            {
                Framework::Utility::GLBLoader loader(modelPath / MODEL_NAMES.at(BottomLevelASType::UFO));
                auto indices = toLinearList(loader.getIndicesPerSubMeshes());
                auto vertices = toLinearVertices(loader.getPositionsPerSubMeshes(),
                    loader.getNormalsPerSubMeshes(), loader.getUVsPerSubMeshes(),
                    loader.getTangentsPerSubMeshes());

                createBuffer(mDeviceResource->getDevice(), &mIndexBuffer[BottomLevelASType::UFO].mResource, indices.data(), indices.size() * sizeof(indices[0]), L"IndexBuffer");
                createBuffer(mDeviceResource->getDevice(), &mVertexBuffer[BottomLevelASType::UFO].mResource, vertices.data(), vertices.size() * sizeof(vertices[0]), L"VertexBuffer");

                resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
                resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
                mIndexOffsets[LocalRootSignature::HitGroupIndex::UFO] = (UINT)indices.size();
                mVertexOffsets[LocalRootSignature::HitGroupIndex::UFO] = (UINT)vertices.size();

                auto materialList = loader.getMaterialDatas();
                Material material = {};
                if (!materialList.empty()) material = loader.getMaterialDatas()[0];
                auto descs = loader.getImageDatas();
                loadTextures(material, descs, ModelTextureType::UFO_Albedo);
            }
            //四角形のバッファ作成
            {
                std::vector<Index> indices = { 0,1,2,0,2,3 };
                createBuffer(mDeviceResource->getDevice(), &mIndexBuffer[BottomLevelASType::Quad].mResource, indices.data(), indices.size() * sizeof(indices[0]), L"IndexBuffer");
                std::vector<Framework::DX::Vertex> vertices =
                {
                    { Vec3(-1,1,0),Vec3(0,0,-1),Vec2(0,0) },
                    { Vec3(1,1,0),Vec3(0,0,-1),Vec2(1,0)  },
                    { Vec3(1,-1,0) ,Vec3(0,0,-1),Vec2(1,1) },
                    { Vec3(-1,-1,0),Vec3(0,0,-1),Vec2(0,1)  },
                };
                createBuffer(mDeviceResource->getDevice(), &mVertexBuffer[BottomLevelASType::Quad].mResource, vertices.data(), vertices.size() * sizeof(vertices[0]), L"VertexBuffer");

                resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
                resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
                mIndexOffsets[LocalRootSignature::HitGroupIndex::Quad] = (UINT)indices.size();
                mVertexOffsets[LocalRootSignature::HitGroupIndex::Quad] = (UINT)vertices.size();

                Framework::Desc::TextureDesc desc = Framework::Utility::TextureLoader::load(texPath / "back2.png");
                auto texture = std::make_shared<Texture2D>(device, desc);
                mDescriptorTable->allocate(texture.get());
                texture->createSRV(device);
                mTextures[ModelTextureType::Quad_Albedo] = (texture);
                mTextureIDs[ModelTextureType::Quad_Albedo] = ModelTextureType::Quad_Albedo;
            }

            //床のバッファ作成
            {
                Framework::Utility::GLBLoader loader(modelPath / MODEL_NAMES.at(BottomLevelASType::Floor));
                auto indices = toLinearList(loader.getIndicesPerSubMeshes());
                auto vertices = toLinearVertices(loader.getPositionsPerSubMeshes(), loader.getNormalsPerSubMeshes(), loader.getUVsPerSubMeshes());
                createBuffer(mDeviceResource->getDevice(), &mIndexBuffer[BottomLevelASType::Floor].mResource, indices.data(), indices.size() * sizeof(indices[0]), L"IndexBuffer");
                createBuffer(mDeviceResource->getDevice(), &mVertexBuffer[BottomLevelASType::Floor].mResource, vertices.data(), vertices.size() * sizeof(vertices[0]), L"VertexBuffer");

                resourceIndices.insert(resourceIndices.end(), indices.begin(), indices.end());
                resourceVertices.insert(resourceVertices.end(), vertices.begin(), vertices.end());
                mIndexOffsets[LocalRootSignature::HitGroupIndex::Floor] = (UINT)indices.size();
                mVertexOffsets[LocalRootSignature::HitGroupIndex::Floor] = (UINT)vertices.size();

                auto materialList = loader.getMaterialDatas();
                Material material = {};
                if (!materialList.empty()) material = loader.getMaterialDatas()[0];
                auto descs = loader.getImageDatas();
                loadTextures(material, descs, ModelTextureType::Plane_Albedo);

            }
        }
        //BLAS・TLASの構築
        {
            ID3D12Device* device = mDeviceResource->getDevice();
            ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
            ID3D12CommandAllocator* allocator = mDeviceResource->getCommandAllocator();
            ID3D12Device5* dxrDevice = mDXRDevice->getDXRDevice();
            ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice->getDXRCommandList();

            commandList->Reset(allocator, nullptr);

            //BLAS用のジオメトリディスク作成
            for (int i = 0; i < BottomLevelASType::Count; i++) {
                D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
                geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
                geometryDesc.Triangles.IndexBuffer = mIndexBuffer[i].mResource->GetGPUVirtualAddress();
                geometryDesc.Triangles.IndexCount = static_cast<UINT>(mIndexBuffer[i].mResource->GetDesc().Width) / sizeof(Index);
                geometryDesc.Triangles.IndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
                geometryDesc.Triangles.Transform3x4 = 0;
                geometryDesc.Triangles.VertexBuffer.StartAddress = mVertexBuffer[i].mResource->GetGPUVirtualAddress();
                geometryDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(Framework::DX::Vertex);
                geometryDesc.Triangles.VertexCount = static_cast<UINT>(mVertexBuffer[i].mResource->GetDesc().Width) / sizeof(Framework::DX::Vertex);
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
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

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

            topLevelBuildDesc.DestAccelerationStructureData = mTLASBuffer.buffer->GetGPUVirtualAddress();
            topLevelBuildDesc.ScratchAccelerationStructureData = mTLASBuffer.scratch->GetGPUVirtualAddress();

            mDeviceResource->executeCommandList();
            mDeviceResource->waitForGPU();

            //バッファのシェーダーリソースビュー作成
            createBuffer(device, &mResourcesIndexBuffer.mResource, resourceIndices.data(), resourceIndices.size() * sizeof(resourceIndices[0]), L"ResourceIndex");
            createBuffer(device, &mResourcesVertexBuffer.mResource, resourceVertices.data(), resourceVertices.size() * sizeof(resourceVertices[0]), L"ResourceVertex");

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
                mDescriptorTable->allocate(&mResourcesIndexBuffer);
                device->CreateShaderResourceView(mResourcesIndexBuffer.mResource.Get(), &srvDesc, mResourcesIndexBuffer.mCPUHandle);
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
                mDescriptorTable->allocate(&mResourcesVertexBuffer);
                device->CreateShaderResourceView(mResourcesVertexBuffer.mResource.Get(), &srvDesc, mResourcesVertexBuffer.mCPUHandle);
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
        void* missShadowShaderID = stateObjectProp->GetShaderIdentifier(MISS_SHADOW_SHADER_NAME.c_str());
        void* hitGroup_SphereShaderID = stateObjectProp->GetShaderIdentifier(HIT_GROUP_SPHERE_NAME.c_str());
        void* hitGroup_QuadShaderID = stateObjectProp->GetShaderIdentifier(HIT_GROUP_QUAD_NAME.c_str());
        void* hitGroup_FloorShaderID = stateObjectProp->GetShaderIdentifier(HIT_GROUP_FLOOR_NAME.c_str());
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
            rootArgument.cb.back = Color(0.1f, 0.2f, 0.3f, 1);
            UINT num = 2;
            UINT recordSize = shaderIDSize + sizeof(RootArgument);
            ShaderTable table(device, num, recordSize, L"MissShaderTable");
            table.push_back(ShaderRecord(missShaderID, shaderIDSize, &rootArgument, sizeof(RootArgument)));
            table.push_back(ShaderRecord(missShadowShaderID, shaderIDSize));
            mMissTable = table.getResource();
            mMissStride = table.getShaderRecordSize();
        }
        //HitGroup
        {
            struct RootArgument {
                D3D12_GPU_DESCRIPTOR_HANDLE albedo;
                D3D12_GPU_DESCRIPTOR_HANDLE normal;
                D3D12_GPU_DESCRIPTOR_HANDLE metalRough;
                D3D12_GPU_DESCRIPTOR_HANDLE emissive;
                HitGroupConstant cb;
            } rootArguments;
            UINT num = 3;
            UINT recordSize = shaderIDSize + sizeof(RootArgument);
            ShaderTable table(device, num, recordSize, L"HitGroupShaderTable");
            //UFO
            {
                rootArguments.cb.indexOffset = std::get<0>(getOffset(LocalRootSignature::HitGroupIndex::UFO));
                rootArguments.cb.vertexOffset = std::get<1>(getOffset(LocalRootSignature::HitGroupIndex::UFO));
                rootArguments.albedo = mTextures[mTextureIDs[ModelTextureType::UFO_Albedo]]->getGPUHandle();
                rootArguments.normal = mTextures[mTextureIDs[ModelTextureType::UFO_Normal]]->getGPUHandle();
                rootArguments.metalRough = mTextures[mTextureIDs[ModelTextureType::UFO_MetalRough]]->getGPUHandle();
                rootArguments.emissive = mTextures[mTextureIDs[ModelTextureType::UFO_Emissive]]->getGPUHandle();
                table.push_back(ShaderRecord(hitGroup_SphereShaderID, shaderIDSize, &rootArguments, sizeof(RootArgument)));
            }
            //Quad
            {
                rootArguments.cb.indexOffset = std::get<0>(getOffset(LocalRootSignature::HitGroupIndex::Quad));
                rootArguments.cb.vertexOffset = std::get<1>(getOffset(LocalRootSignature::HitGroupIndex::Quad));
                rootArguments.albedo = mTextures[mTextureIDs[ModelTextureType::Quad_Albedo]]->getGPUHandle();
                table.push_back(ShaderRecord(hitGroup_QuadShaderID, shaderIDSize, &rootArguments, sizeof(RootArgument)));
            }
            //Floor
            {
                rootArguments.cb.indexOffset = std::get<0>(getOffset(LocalRootSignature::HitGroupIndex::Floor));
                rootArguments.cb.vertexOffset = std::get<1>(getOffset(LocalRootSignature::HitGroupIndex::Floor));
                rootArguments.albedo = mTextures[mTextureIDs[ModelTextureType::Plane_Albedo]]->getGPUHandle();
                rootArguments.normal = mTextures[mTextureIDs[ModelTextureType::Plane_Normal]]->getGPUHandle();
                rootArguments.metalRough = mTextures[mTextureIDs[ModelTextureType::Plane_MetalRough]]->getGPUHandle();
                rootArguments.emissive = mTextures[mTextureIDs[ModelTextureType::Plane_Emissive]]->getGPUHandle();
                table.push_back(ShaderRecord(hitGroup_FloorShaderID, shaderIDSize, &rootArguments, sizeof(RootArgument)));
            }
            mHitGroupStride = table.getShaderRecordSize();
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
            IID_PPV_ARGS(&mRaytracingOutput.mResource)));
        mRaytracingOutput.mResource->SetName(L"RaytracingOutput");

        mDescriptorTable->allocate(&mRaytracingOutput);
        //mRaytracingOutput.cpuHandle = mDescriptorTable->getCPUHandle(DescriptorIndex::RaytracingOutput);
        //mRaytracingOutput.gpuHandle = mDescriptorTable->getGPUHandle(DescriptorIndex::RaytracingOutput);
        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
        device->CreateUnorderedAccessView(mRaytracingOutput.mResource.Get(), nullptr, &uavDesc, mRaytracingOutput.mCPUHandle);
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

    if (mInputManager->getKeyboard()->getKey(Framework::Input::KeyCode::LShift)) {
        mQuadRotate += 1.0f;
    }
}

void Scene::render() {
    ID3D12Device* device = mDeviceResource->getDevice();
    ID3D12GraphicsCommandList5* dxrCommandList = mDXRDevice->getDXRCommandList();

    //TLASのデータ更新
    std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDesc(TLAS_NUM);
    UINT offset = 0;
    for (UINT n = 0; n < TRIANGLE_COUNT; n++) {
        Rad rotX = mQuadRotate.toRadians() * 0.37f;
        Rad rotY = mQuadRotate.toRadians() * 1.45f;
        Rad rotZ = mQuadRotate.toRadians() * 0.73f;
        XMMATRIX transform = XMMatrixRotationRollPitchYaw(rotX.getRad(), rotY.getRad(), rotZ.getRad()) *
            XMMatrixTranslation((float)n * 5, 0, 0);
        std::stringstream ss;
        ss << "(" << rotX.toDegree().getDeg() << "," << rotY.toDegree().getDeg() << "," << rotZ.toDegree().getDeg() << ")\n";
        mRotSphereText->setText(ss.str());
        instanceDesc[n + offset].InstanceID = 0;
        instanceDesc[n + offset].InstanceMask = 0xff;
        instanceDesc[n + offset].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
        instanceDesc[n + offset].InstanceContributionToHitGroupIndex = LocalRootSignature::HitGroupIndex::UFO;
        instanceDesc[n + offset].AccelerationStructure = mBLASBuffers[BottomLevelASType::UFO].buffer->GetGPUVirtualAddress();
        XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc[n + offset].Transform), transform);
    }

    offset += TRIANGLE_COUNT;
    for (UINT n = 0; n < QUAD_COUNT; n++) {
        XMMATRIX transform = XMMatrixScaling(1, 1, 1) * XMMatrixRotationRollPitchYaw(0, mQuadRotate.toRadians().getRad(), 0) * XMMatrixTranslation((float)n * 5 + 20, 3, 0);
        instanceDesc[n + offset].InstanceID = 0;
        instanceDesc[n + offset].InstanceMask = 0xff;
        instanceDesc[n + offset].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
        instanceDesc[n + offset].InstanceContributionToHitGroupIndex = LocalRootSignature::HitGroupIndex::Quad;
        instanceDesc[n + offset].AccelerationStructure = mBLASBuffers[BottomLevelASType::Quad].buffer->GetGPUVirtualAddress();
        XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc[n + offset].Transform), transform);
    }
    offset += QUAD_COUNT;
    for (UINT n = 0; n < FLOOR_COUNT; n++) {
        XMMATRIX transform = XMMatrixScaling(100, 1, 100) * XMMatrixTranslation(0, -5, 0);
        instanceDesc[n + offset].InstanceID = 0;
        instanceDesc[n + offset].InstanceMask = 0xff;
        instanceDesc[n + offset].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
        instanceDesc[n + offset].InstanceContributionToHitGroupIndex = LocalRootSignature::HitGroupIndex::Floor;
        instanceDesc[n + offset].AccelerationStructure = mBLASBuffers[BottomLevelASType::Floor].buffer->GetGPUVirtualAddress();
        XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(instanceDesc[n + offset].Transform), transform);
    }

    createBuffer(device, mInstanceDescs.ReleaseAndGetAddressOf(), instanceDesc.data(), instanceDesc.size() * sizeof(D3D12_RAYTRACING_INSTANCE_DESC), L"InstanceDescs");
    topLevelBuildDesc.Inputs.InstanceDescs = mInstanceDescs->GetGPUVirtualAddress();

    //TLASの構築終了まで待機する用のバリア
    dxrCommandList->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);
    mDeviceResource->getCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::UAV(mTLASBuffer.buffer.Get()));

    //描画開始
    ID3D12GraphicsCommandList* commandList = mDeviceResource->getCommandList();
    UINT frameIndex = mDeviceResource->getCurrentFrameIndex();
    commandList->SetComputeRootSignature(mGlobalRootSignature.Get());
    mSceneCB.copyStatingToGPU(frameIndex);

    ID3D12DescriptorHeap* heaps[] = { mDescriptorTable->getHeap() };
    commandList->SetDescriptorHeaps(_countof(heaps), heaps);
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::RenderTarget, mRaytracingOutput.mGPUHandle);
    commandList->SetComputeRootShaderResourceView(GlobalRootSignature::Slot::AccelerationStructure, mTLASBuffer.buffer->GetGPUVirtualAddress());
    commandList->SetComputeRootConstantBufferView(GlobalRootSignature::Slot::SceneConstant, mSceneCB.gpuVirtualAddress());
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::IndexBuffer, mResourcesIndexBuffer.mGPUHandle);
    commandList->SetComputeRootDescriptorTable(GlobalRootSignature::Slot::VertexBuffer, mResourcesVertexBuffer.mGPUHandle);

    D3D12_DISPATCH_RAYS_DESC dispatchDesc = {};
    dispatchDesc.RayGenerationShaderRecord.StartAddress = mRayGenTable->GetGPUVirtualAddress();
    dispatchDesc.RayGenerationShaderRecord.SizeInBytes = mRayGenTable->GetDesc().Width;

    dispatchDesc.MissShaderTable.StartAddress = mMissTable->GetGPUVirtualAddress();
    dispatchDesc.MissShaderTable.SizeInBytes = mMissTable->GetDesc().Width;
    dispatchDesc.MissShaderTable.StrideInBytes = mMissStride;

    dispatchDesc.HitGroupTable.StartAddress = mHitGroupTable->GetGPUVirtualAddress();
    dispatchDesc.HitGroupTable.SizeInBytes = mHitGroupTable->GetDesc().Width;
    dispatchDesc.HitGroupTable.StrideInBytes = mHitGroupStride;

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
    preCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.mResource.Get(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->ResourceBarrier(_countof(preCopyBarriers), preCopyBarriers);

    commandList->CopyResource(mDeviceResource->getRenderTarget(), mRaytracingOutput.mResource.Get());

    D3D12_RESOURCE_BARRIER postCopyBarriers[2];
    postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(mDeviceResource->getRenderTarget(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
    postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(mRaytracingOutput.mResource.Get(),
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    commandList->ResourceBarrier(_countof(postCopyBarriers), postCopyBarriers);

    mDebugWindow->draw();
}
