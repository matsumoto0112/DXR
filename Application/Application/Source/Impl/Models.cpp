#include "Models.h"

void Models::load(Framework::DX::DeviceResource* device, ID3D12GraphicsCommandList* commandList,
    const std::filesystem::path& rootPath) {
    for (ModelType type = (ModelType)0; type != ModelType::End; type = (ModelType)((int)type + 1)) {
        std::shared_ptr<Model> model = std::make_shared<Model>();
        model->init(device, commandList, rootPath / LOAD_MODEL_NAMES.at(type), (int)type);
        mLoadModels.emplace(type, model);
    }
}

void Models::unload() {
    mLoadModels.clear();
}
