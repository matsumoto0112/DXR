#pragma once
#include "DX/ConstantBuffer.h"
#include "DX/DeviceResource.h"
#include "DX/Raytracing/DXRDevice.h"
#include "DX/Util/Buffer.h"
#include "Utility/GPUTimer.h"
#include "Utility/Time.h"
#include "Define.h"
#include "Input/InputManager.h"
#include "Application/Assets/Shader/Raytracing/Util/GlobalCompat.h"

static constexpr UINT TEXTURE_NUM = 1000;
namespace DescriptorIndex {
    enum MyEnum {
        RaytracingOutput,
        IndexBuffer,
        VertexBuffer,
        TextureStart,
        TextureEnd = TextureStart + TEXTURE_NUM,

        Count = TextureEnd + 1
    };
} //DescriptorIndex

struct AccelerationBuffer {
    ComPtr<ID3D12Resource> buffer;
    ComPtr<ID3D12Resource> scratch;
};

/**
* @class Scene
* @brief
*/
class Scene {
public:
    /**
    * @brief コンストラクタ
    */
    Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager, UINT width, UINT height);
    /**
    * @brief デストラクタ
    */
    ~Scene();

    void create();

    void reset();

    void update();
    void render();
private:
    Framework::DX::DeviceResource* mDeviceResource;
    Framework::Input::InputManager* mInputManager;
    std::unique_ptr<Framework::DX::DXRDevice> mDXRDevice;
    std::array<AccelerationBuffer, BottomLevelASType::Count> mBLASBuffers;
    AccelerationBuffer mTLASBuffer;
    ComPtr<ID3D12Resource> mRayGenTable;
    UINT mMissStride;
    ComPtr<ID3D12Resource> mMissTable;
    UINT mHitGroupStride;
    ComPtr<ID3D12Resource> mHitGroupTable;
    Framework::DX::ConstantBuffer<SceneConstantBuffer> mSceneCB;
private:
    ComPtr<ID3D12RootSignature> mGlobalRootSignature; //!< グローバルルートシグネチャ
    ComPtr<ID3D12RootSignature> mMissLocalRootSignature; //!< Missシェーダー用ローカルルートシグネチャ
    std::array<ComPtr<ID3D12RootSignature>, LocalRootSignature::HitGroup::Count> mHitGroupLocalRootSignature;
    ComPtr<ID3D12StateObject> mDXRStateObject; //!< レイトレーシングパイプラインステート
    std::unique_ptr<Framework::DX::DescriptorTable> mDescriptorTable;
    Framework::DX::Buffer mResourcesIndexBuffer; //!< リソースのインデックスバッファ
    Framework::DX::Buffer mResourcesVertexBuffer; //!< リソースの頂点バッファ
    Framework::DX::Buffer mRaytracingOutput;
    std::vector<Framework::DX::Buffer> mTextures;
private:
    UINT mWidth;
    UINT mHeight;
    Framework::Utility::Time mTime;
    Framework::Utility::GPUTimer mGpuTimer;
    Vec3 mCameraPosition;
    Vec3 mCameraRotation;
    Vec3 mLightPosition;
    Color mLightDiffuse;
    Color mLightAmbient;
private:
    std::unique_ptr<Framework::ImGUI::Window> mDebugWindow;
    std::shared_ptr<Framework::ImGUI::Text> mFPSText;
};
