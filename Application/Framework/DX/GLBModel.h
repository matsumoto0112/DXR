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
        * @brief コンストラクタ
        */
        GLBModel(const std::filesystem::path& path);
        /**
        * @brief デストラクタ
        */
        ~GLBModel();
    private:

    };
} //Framework::DX
