#include "Scene.h"
#include "Utility/StringUtil.h"

using namespace Framework::DX;
using namespace Framework::Utility;

Scene::Scene(Framework::DX::DeviceResource* device)
    :mDeviceResource(device),
    mDXRDevice(std::make_unique<DXRDevice>(mDeviceResource)) {
    mDebugWindow = std::make_unique<Framework::ImGUI::Window>("Debug");
    mFPSText = std::make_shared<Framework::ImGUI::Text>("FPS:");
    mDebugWindow->addItem(mFPSText);
}

Scene::~Scene() { }

void Scene::create() {
    mDXRDevice->recreate();
    mGpuTimer.storeDevice(mDeviceResource->getDevice(), mDeviceResource->getCommandQueue(), mDeviceResource->getBackBufferCount());
}

void Scene::reset() {
    mDXRDevice->reset();
    mGpuTimer.reset();
    mGpuTimer.releaseDevice();
}

void Scene::update() {
    mTime.update();
    mFPSText->setText(format("FPS:%0.3f", mTime.getFPS()));
}

void Scene::render() {
    mDebugWindow->draw();
}
