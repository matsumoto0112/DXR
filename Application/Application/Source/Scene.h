#pragma once
#include "DX/ConstantBuffer.h"
#include "DX/CountingDescriptorTable.h"
#include "DX/DeviceResource.h"
#include "DX/Raytracing/DXRDevice.h"
#include "Utility/GPUTimer.h"
#include "Utility/Time.h"
#include "Define.h"
#include "Input/InputManager.h"
#include "DX/Resource/Texture2D.h"
#include "Application/Assets/Shader/Raytracing/Util/GlobalCompat.h"
#include "Device/ISystemEventNotify.h"

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
    * @brief �R���X�g���N�^
    */
    Scene(Framework::DX::DeviceResource* device, Framework::Input::InputManager* inputManager, UINT width, UINT height);
    /**
    * @brief �f�X�g���N�^
    */
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
    ComPtr<ID3D12RootSignature> mGlobalRootSignature; //!< �O���[�o�����[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> mMissLocalRootSignature; //!< Miss�V�F�[�_�[�p���[�J�����[�g�V�O�l�`��
    std::array<ComPtr<ID3D12RootSignature>, LocalRootSignature::HitGroup::Count> mHitGroupLocalRootSignature;
    ComPtr<ID3D12StateObject> mDXRStateObject; //!< ���C�g���[�V���O�p�C�v���C���X�e�[�g
    std::unique_ptr<Framework::DX::CountingDescriptorTable> mDescriptorTable;
    Framework::DX::IBuffer mResourcesIndexBuffer; //!< ���\�[�X�̃C���f�b�N�X�o�b�t�@
    Framework::DX::IBuffer mResourcesVertexBuffer; //!< ���\�[�X�̒��_�o�b�t�@
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
private:
    std::unique_ptr<Framework::ImGUI::Window> mDebugWindow;
    std::shared_ptr<Framework::ImGUI::Text> mFPSText;
};
