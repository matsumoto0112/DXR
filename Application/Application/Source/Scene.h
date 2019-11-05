#pragma once
#include "DX/DeviceResource.h"
#include "DX/Raytracing/DXRDevice.h"

/**
* @class Scene
* @brief
*/
class Scene {
public:
    /**
    * @brief �R���X�g���N�^
    */
    Scene(Framework::DX::DeviceResource* device);
    /**
    * @brief �f�X�g���N�^
    */
    ~Scene();

    void create();

    void reset();
private:
    Framework::DX::DeviceResource* mDeviceResource;
    std::unique_ptr<Framework::DX::DXRDevice> mDXRDevice;
};
