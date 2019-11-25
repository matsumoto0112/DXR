#pragma once
#include "Assets/Shader/Raytracing/Util/GlobalCompat.h"
#include "DX/ConstantBuffer.h"
#include "DX/CountingDescriptorTable.h"
#include "DX/DeviceResource.h"
#include "DX/Raytracing/DXRDevice.h"
#include "DX/Resource/Texture2D.h"
#include "Define.h"
#include "Device/ISystemEventNotify.h"
#include "Input/InputManager.h"
#include "Utility/GPUTimer.h"
#include "Utility/Time.h"

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
}
struct AccelerationBuffer {
    ComPtr<ID3D12Resource> buffer;
    ComPtr<ID3D12Resource> scratch;
};

class Scene {
public:
    Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager,
        UINT width, UINT height);
    ~Scene();
    void create();
    void reset();
    void update();
    void render();
    void onWindowSizeChanged(UINT width, UINT height);

private:
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void createWindowDependentResources();
    void releaseWindowDependentResources();

private:
    Framework::DX::DeviceResource* mDeviceResource;
    Framework::Input::InputManager* mInputManager;
    Framework::DX::DXRDevice mDXRDevice;
    std::array<AccelerationBuffer, BottomLevelASType::Count> mBLASBuffers;
    AccelerationBuffer mTLASBuffer;
    ComPtr<ID3D12Resource> mRayGenTable;
    UINT mMissStride;
    ComPtr<ID3D12Resource> mMissTable;
    UINT mHitGroupStride;
    ComPtr<ID3D12Resource> mHitGroupTable;
    Framework::DX::ConstantBuffer<SceneConstantBuffer> mSceneCB;

private:
    ComPtr<ID3D12RootSignature> mGlobalRootSignature;
    ComPtr<ID3D12RootSignature> mMissLocalRootSignature;
    std::array<ComPtr<ID3D12RootSignature>, LocalRootSignature::HitGroup::Count>
        mHitGroupLocalRootSignature;
    ComPtr<ID3D12StateObject> mDXRStateObject;
    std::unique_ptr<Framework::DX::CountingDescriptorTable> mDescriptorTable;
    Framework::DX::IBuffer mResourcesIndexBuffer;
    Framework::DX::IBuffer mResourcesVertexBuffer;
    Framework::DX::IBuffer mRaytracingOutput;
    std::vector<std::shared_ptr<Framework::DX::Texture2D>> mTextures;

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

    //private:
    //    std::unique_ptr<Framework::ImGUI::Window> mDebugWindow;
    //    std::shared_ptr<Framework::ImGUI::Text> mFPSText;
};
