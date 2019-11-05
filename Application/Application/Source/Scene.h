#pragma once
#include "DX/DeviceResource.h"
#include "DX/Raytracing/DXRDevice.h"
#include "Utility/Time.h"
#include "Utility/GPUTimer.h"
#include "Define.h"

/**
* @class Scene
* @brief
*/
class Scene {
public:
    /**
    * @brief コンストラクタ
    */
    Scene(Framework::DX::DeviceResource* device);
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
private:

private:
    Framework::Utility::Time mTime;
    Framework::Utility::GPUTimer mGpuTimer;
private:
    std::unique_ptr<Framework::ImGUI::Window> mDebugWindow;
    std::shared_ptr<Framework::ImGUI::Text> mFPSText;
};
