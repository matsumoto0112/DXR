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
    * @brief ���f���N���X
    */
    class Model {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        Model() { }
        /**
        * @brief �f�X�g���N�^
        */
        ~Model() { }
    protected:
        std::vector<Mesh> mSubMeshes;
    };
} //Framework::DX
