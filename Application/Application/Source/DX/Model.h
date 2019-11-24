#pragma once
#include <vector>
#include <d3d12.h>
#include "DX/ModelCompat.h"

namespace Framework::DX {
    struct Mesh {
        UINT materialID;
        std::vector<Vertex> vertices;
        std::vector<Index> indices;
    };

    /**
    * @class Model
    * @brief モデルクラス
    */
    class Model {
    public:
        /**
        * @brief コンストラクタ
        */
        Model() { }
        /**
        * @brief デストラクタ
        */
        ~Model() { }
    protected:
        std::vector<Mesh> mSubMeshes;
    };
} //Framework::DX
