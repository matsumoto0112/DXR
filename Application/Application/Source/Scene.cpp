#include "Scene.h"
using namespace Framework::DX;

Scene::Scene(Framework::DX::DeviceResource* device)
    :mDeviceResource(device),
    mDXRDevice(std::make_unique<DXRDevice>(mDeviceResource)) { }

Scene::~Scene() { }

void Scene::create() {
    mDXRDevice->recreate();
}

void Scene::reset() {
    mDXRDevice->reset();
}
