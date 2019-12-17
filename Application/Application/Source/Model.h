#pragma once
#include "../Assets/Shader/Raytracing/Util/GlobalCompat.h"
#include "DX/DeviceResource.h"
#include "DX/ModelCompat.h"
#include "DX/Resource/IndexBuffer.h"
#include "DX/Resource/Texture2D.h"
#include "DX/Resource/VertexBuffer.h"
#include "Typedef.h"

/**
 * @class Model
 * @brief discription
 */
class Model {
public:
    /**
     * @brief
     */
    Model() {}
    /**
     * @brief
     */
    ~Model() {}
    void init(Framework::DX::DeviceResource* device, ID3D12GraphicsCommandList* commandList,
        const std::filesystem::path& filepath, UINT id);

    //private:
    UINT mModelID;
    std::vector<Framework::DX::Vertex> mVertices;
    std::vector<Index> mIndices;
    UINT mVertexOffset;
    UINT mIndexOffset;
    Framework::DX::VertexBuffer mVertexBuffer;
    Framework::DX::IndexBuffer mIndexBuffer;
    Framework::DX::Texture2D mAlbedo;
    Framework::DX::Texture2D mNormalMap;
    Framework::DX::Texture2D mMetallicRoughness;
    Framework::DX::Texture2D mEmissiveMap;
    Framework::DX::Texture2D mOcclusionMap;
};
