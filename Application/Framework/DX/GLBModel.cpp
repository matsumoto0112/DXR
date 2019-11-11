#include "GLBModel.h"
#include "Utility/IO/GLBLoader.h"

namespace Framework::DX {

    GLBModel::GLBModel(const std::filesystem::path& path) {
        Utility::GLBLoader glbLoader(path.c_str());
    }

    GLBModel::~GLBModel() { }

} //Framework::DX
