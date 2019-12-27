#pragma once
#include "Model.h"

enum class ModelType {
    UFO,
    Floor,
    Sphere,
    House,
    Tree,
    Crate,

    End
};

/**
 * @class Models
 */
class Models {
private:
    std::unordered_map<ModelType, std::wstring> LOAD_MODEL_NAMES = {
        { ModelType::UFO, L"UFO.glb" },
        { ModelType::Sphere, L"sphere.glb" },
        { ModelType::Floor, L"floor.glb" },
        { ModelType::House, L"house.glb" },
        { ModelType::Tree, L"tree.glb" },
        { ModelType::Crate, L"Crate.glb" },
    };

public:
    /**
     * @brief
     */
    Models() {}
    /**
     * @brief
     */
    ~Models() {}

    void load(Framework::DX::DeviceResource* device, ID3D12GraphicsCommandList* commandList,
        const std::filesystem::path& rootPath);
    void unload();

private:
    std::unordered_map<ModelType, std::shared_ptr<Model>> mLoadModels;
};
