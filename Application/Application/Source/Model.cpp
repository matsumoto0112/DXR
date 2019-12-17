#include "Model.h"
#include "Desc/TextureDesc.h"
#include "Utility/Color4.h"
#include "Utility/IO/GLBLoader.h"

using namespace Framework::Desc;
using namespace Framework::DX;
using namespace Framework::Utility;

namespace {
    inline std::vector<Index> toLinearList(const std::vector<std::vector<Index>>& list) {
        std::vector<Index> res;
        for (size_t i = 0; i < list.size(); i++) {
            res.insert(res.end(), list[i].begin(), list[i].end());
        }
        return res;
    }

    inline std::vector<Framework::DX::Vertex> toLinearVertices(
        const std::vector<PositionList>& positions, const std::vector<NormalList>& normals = {},
        const std::vector<UVList>& uvs = {}, const std::vector<TangentList>& tangents = {}) {
        std::vector<Framework::DX::Vertex> res;
        for (size_t i = 0; i < positions.size(); i++) {
            for (size_t j = 0; j < positions[i].size(); j++) {
                Framework::DX::Vertex v;
                v.position = positions[i][j];
                v.normal = normals.empty() || normals[i].empty() ? Vec3(0, 0, 0) : normals[i][j];
                v.uv = uvs.empty() || uvs[i].empty() ? Vec2(0, 0) : uvs[i][j];
                v.tangent
                    = tangents.empty() || tangents[i].empty() ? Vec4(0, 0, 0, 0) : tangents[i][j];
                res.emplace_back(v);
            }
        }
        return res;
    }
    static const std::vector<BYTE> unitTexture(const Color4& color) {
        return {
            static_cast<BYTE>(color.r * 255.0f),
            static_cast<BYTE>(color.g * 255.0f),
            static_cast<BYTE>(color.b * 255.0f),
            static_cast<BYTE>(color.a * 255.0f),
        };
    }
    static const TextureDesc DEFAULT_ALBEDO = { L"Default_Albedo", unitTexture(Color4(1, 1, 1, 1)),
        1, 1, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, TextureFlags::None };
    static const TextureDesc DEFAULT_NORMALMAP
        = { L"Default_NormalMap", unitTexture(Color4(0.5f, 0.5f, 1.0f, 1)), 1, 1,
              DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, TextureFlags::None };
    static const TextureDesc DEFAULT_METALLICROUGHNESSMAP
        = { L"Default_MetallicRoughness", unitTexture(Color4(0, 0, 1, 1)), 1, 1,
              DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, TextureFlags::None };
    static const TextureDesc DEFAULT_EMISSIVEMAP
        = { L"Default_EmissiveMap", unitTexture(Color4(0, 0, 0, 1)), 1, 1,
              DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, TextureFlags::None };
    static const TextureDesc DEFAULT_OCCLUSIONMAP
        = { L"Default_OcclusionMap", unitTexture(Color4(1, 1, 1, 1)), 1, 1,
              DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, TextureFlags::None };
} // namespace

void Model::init(Framework::DX::DeviceResource* device, ID3D12GraphicsCommandList* commandList,
    const std::filesystem::path& filepath, UINT id) {
    mModelID = id;

    GLBLoader loader(filepath);
    //インデックス配列を二次元配列から線形に変換する
    std::vector<IndexList> indices = loader.getIndicesPerSubMeshes();
    mIndices = toLinearList(indices);

    //頂点配列を線形に変換する
    std::vector<PositionList> positions = loader.getPositionsPerSubMeshes();
    std::vector<NormalList> normals = loader.getNormalsPerSubMeshes();
    std::vector<UVList> uvs = loader.getUVsPerSubMeshes();
    std::vector<TangentList> tangents = loader.getTangentsPerSubMeshes();
    mVertices = toLinearVertices(positions, normals, uvs, tangents);

    mIndexBuffer.init(device, mIndices, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
        filepath.filename().wstring() + L"Index");
    mVertexBuffer.init(device, mVertices, filepath.filename().wstring() + L"Vertex");

    //マテリアルを読み込む
    std::vector<GlbMaterial> materials = loader.getMaterialDatas();
    //マテリアルがすれば最初のマテリアルを、存在しなければデフォルトのマテリアルを使用する
    GlbMaterial material = materials.empty() ? GlbMaterial{} : materials[0];
    std::vector<TextureDesc> descs = loader.getImageDatas();

    //テクスチャの読み込み処理
    mAlbedo.init(device, commandList, (descs.empty() ? DEFAULT_ALBEDO : descs[0]));
    mAlbedo.createSRV(device, false);

    mNormalMap.init(device, commandList,
        (material.normalMapID == -1 ? DEFAULT_NORMALMAP : descs[material.normalMapID]));
    mNormalMap.createSRV(device, false);

    mMetallicRoughness.init(device, commandList,
        (material.metallicRoughnessMapID == -1 ? DEFAULT_METALLICROUGHNESSMAP
                                               : descs[material.metallicRoughnessMapID]));
    mMetallicRoughness.createSRV(device, false);

    mEmissiveMap.init(device, commandList,
        (material.emissiveMapID == -1 ? DEFAULT_EMISSIVEMAP : descs[material.emissiveMapID]));
    mEmissiveMap.createSRV(device, false);

    mOcclusionMap.init(device, commandList,
        (material.occlusionMapID == -1 ? DEFAULT_OCCLUSIONMAP : descs[material.occlusionMapID]));
    mOcclusionMap.createSRV(device, false);
}
