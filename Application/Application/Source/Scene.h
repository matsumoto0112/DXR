#pragma once
#include "DX/ConstantBuffer.h"
#include "DX/DeviceResource.h"
#include "DX/Raytracing/DXRDevice.h"
#include "DX/Util/Buffer.h"
#include "Utility/GPUTimer.h"
#include "Utility/Time.h"
#include "Define.h"
#include "Application/Assets/Shader/Raytracing/Util/Compat.h"

namespace DescriptorIndex {
    enum MyEnum {
        IndexBuffer,
        VertexBuffer,
        RaytracingOutput,

        Count
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
    Scene(Framework::DX::DeviceResource* device, UINT width, UINT height);
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
    std::unique_ptr<Framework::DX::DXRDevice> mDXRDevice;
    std::array<AccelerationBuffer, BottomLevelASType::Count> mBLASBuffers;
    AccelerationBuffer mTLASBuffer;
    ComPtr<ID3D12Resource> mRayGenTable;
    ComPtr<ID3D12Resource> mMissTable;
    ComPtr<ID3D12Resource> mHitGroupTable;
    Framework::DX::ConstantBuffer<SceneConstantBuffer> mSceneCB;
private:
    ComPtr<ID3D12RootSignature> mGlobalRootSignature; //!< グローバルルートシグネチャ
    ComPtr<ID3D12RootSignature> mMissLocalRootSignature; //!< Missシェーダー用ローカルルートシグネチャ
    ComPtr<ID3D12StateObject> mDXRStateObject; //!< レイトレーシングパイプラインステート
    std::unique_ptr<Framework::DX::DescriptorTable> mDescriptorTable;
    Framework::DX::Buffer mRaytracingOutput;
private:
    UINT mWidth;
    UINT mHeight;
    Framework::Utility::Time mTime;
    Framework::Utility::GPUTimer mGpuTimer;
    XMFLOAT4 mCameraPosition;
    XMFLOAT4 mCameraRotation;
private:
    std::unique_ptr<Framework::ImGUI::Window> mDebugWindow;
    std::shared_ptr<Framework::ImGUI::Text> mFPSText;
};
