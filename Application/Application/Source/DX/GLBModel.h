#pragma once
#include <filesystem>
#include "DX/Model.h"

namespace Framework::DX {
    /**
    * @class GLBModel
    * @brief discription
    */
    class GLBModel :public Model {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        GLBModel(const std::filesystem::path& path);
        /**
        * @brief �f�X�g���N�^
        */
        ~GLBModel();
    private:

    };
} //Framework::DX
