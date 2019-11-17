#include "GLBLoader.h"
#include <fstream>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "Libs/stb/stb_image.h"
#include "Utility/Debug.h"
#include "Utility/StringUtil.h"

using namespace Microsoft::glTF;

namespace {
    /**
    * @class StreamReader
    * @brief discription
    */
    class StreamReader : public IStreamReader {
    public:
        /**
        * @brief コンストラクタ
        */
        StreamReader() { }
        /**
        * @brief デストラクタ
        */
        ~StreamReader() { }
        //データを取得する
        std::shared_ptr<std::istream> GetInputStream(const std::string& filepath) const override {
            auto stream = std::make_shared<std::ifstream>(filepath, std::ios::binary);
            MY_THROW_IF_FALSE(stream->is_open());
            return stream;
        }
    };
}

namespace Framework::Utility {
    GLBLoader::GLBLoader(const std::filesystem::path& filepath) {
        auto streamReader = std::make_unique<StreamReader>();
        auto glbStream = streamReader->GetInputStream(filepath.generic_string());
        mResourceReader = std::make_unique<GLBResourceReader>(std::move(streamReader), std::move(glbStream));
        auto manifest = mResourceReader->GetJson();
        mDocument = Deserialize(manifest);

        MY_DEBUG_LOG("%s loaded \n", filepath.filename().generic_string().c_str());
    }

    GLBLoader::~GLBLoader() { }

    std::vector<Desc::TextureDesc> GLBLoader::getImageDatas() const {
        static constexpr UINT SIZE_PER_PIXEL = 4;
        std::vector<Desc::TextureDesc> result;
        for (auto&& image : mDocument.images.Elements()) {
            Desc::TextureDesc tex = {};
            tex.format = Desc::TextureFormat::R8G8B8A8;
            std::vector<BYTE> texRowData = mResourceReader->ReadBinaryData(mDocument, image);
            int width, height, bpp;

            BYTE* texByte = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texRowData.data()), static_cast<int>(texRowData.size()), &width, &height, &bpp, 0);
            const int size = width * height * SIZE_PER_PIXEL;
            std::vector<BYTE> textureByte;
            int src, dst;
            //アルファ値がないテクスチャデータならアルファデータを追加する
            if (bpp == 3) {
                textureByte.resize(size);
                for (src = 0, dst = 0; src < width * height * bpp; src++, dst++) {
                    textureByte[dst] = texByte[src];
                    if (src % 3 == 2) {
                        dst++;
                        textureByte[dst] = 0xff;
                    }
                }
            }
            else {
                textureByte = std::vector<BYTE>(texByte, texByte + size);
            }

            tex.name = image.name == "" ? L"image" : toWString(image.name);
            tex.pixels = textureByte;
            tex.width = width;
            tex.height = height;
            result.emplace_back(tex);

            stbi_image_free(texByte);
        }
        return result;
    }

    std::vector<Material> GLBLoader::getMaterialDatas() const {
        auto toAlphaMode = [](Microsoft::glTF::AlphaMode mode) {
            switch (mode) {
                case ALPHA_UNKNOWN:
                case ALPHA_OPAQUE: return AlphaMode::Opaque;
                case ALPHA_BLEND:return AlphaMode::Blend;
                default:return AlphaMode::Mask;
            }
        };
        std::vector<Material> result;
        for (auto&& mat : mDocument.materials.Elements()) {
            Material material;
            material.name = mat.name;
            //マップが存在しなければ-1にしておく
            material.normalMapID = (mat.normalTexture.textureId != "") ? std::stoi(mat.normalTexture.textureId) : -1;
            material.emissiveMapID = (mat.emissiveTexture.textureId != "") ? std::stoi(mat.emissiveTexture.textureId) : -1;
            material.metalRoughID = (mat.metallicRoughness.metallicRoughnessTexture.textureId != "") ? std::stoi(mat.metallicRoughness.metallicRoughnessTexture.textureId) : -1;
            material.alphaMode = toAlphaMode(mat.alphaMode);
            result.emplace_back(material);
        }
        return result;
    }
    std::vector<IndexList> GLBLoader::getIndicesPerSubMeshes() const {
        std::vector<IndexList> result;
        for (auto&& mesh : mDocument.meshes.Elements()) {
            for (auto&& prim : mesh.primitives) {
                auto&& index_accesor = mDocument.accessors.Get(prim.indicesAccessorId);
                auto data_uint8 = mResourceReader->ReadBinaryData<uint16_t>(mDocument, index_accesor);
                IndexList data(data_uint8.size());
                const size_t size = data.size() / 3;
                for (int i = 0; i < size; i++) {
                    data[i * 3 + 0] = static_cast<UINT16>(data_uint8[i * 3 + 0]);
                    data[i * 3 + 1] = static_cast<UINT16>(data_uint8[i * 3 + 1]);
                    data[i * 3 + 2] = static_cast<UINT16>(data_uint8[i * 3 + 2]);
                }
                result.emplace_back(data);
            }
        }
        return result;
    }
    std::vector<std::vector<Math::Vector3>> GLBLoader::getPositionsPerSubMeshes() const {
        std::vector<std::vector<Math::Vector3>> result;
        for (auto&& mesh : mDocument.meshes.Elements()) {
            for (auto&& prim : mesh.primitives) {
                std::string accessorID;
                if (!prim.TryGetAttributeAccessorId(ACCESSOR_POSITION, accessorID))continue;
                auto&& accessor = mDocument.accessors.Get(accessorID);
                std::vector<float> data = mResourceReader->ReadBinaryData<float>(mDocument, accessor);
                const int elemCount = 3;
                const int vertexSize = static_cast<int>(data.size()) / elemCount;
                std::vector<Math::Vector3> positions(vertexSize);
                for (int i = 0; i < vertexSize; i++) {
                    positions[i] = Math::Vector3(data[i * elemCount], data[i * elemCount + 1], data[i * elemCount + 2]);
                }
                result.emplace_back(positions);
            }
        }
        return result;
    }
    std::vector<std::vector<Math::Vector3>> GLBLoader::getNormalsPerSubMeshes() const {
        std::vector<std::vector<Math::Vector3>> result;
        for (auto&& mesh : mDocument.meshes.Elements()) {
            for (auto&& prim : mesh.primitives) {
                std::string accessorID;
                if (!prim.TryGetAttributeAccessorId(ACCESSOR_NORMAL, accessorID))continue;
                auto&& accessor = mDocument.accessors.Get(accessorID);
                std::vector<float> data = mResourceReader->ReadBinaryData<float>(mDocument, accessor);
                const int elemCount = 3;
                const int vertexSize = static_cast<int>(data.size()) / elemCount;
                std::vector<Math::Vector3> normals(vertexSize);
                for (int i = 0; i < vertexSize; i++) {
                    normals[i] = Math::Vector3(data[i * elemCount], data[i * elemCount + 1], data[i * elemCount + 2]);
                }
                result.emplace_back(normals);
            }
        }
        return result;
    }
    std::vector<TangentList> GLBLoader::getTangentsPerSubMeshes() const {
        std::vector<TangentList> result;
        for (auto&& mesh : mDocument.meshes.Elements()) {
            for (auto&& prim : mesh.primitives) {
                std::string accessorID;
                if (!prim.TryGetAttributeAccessorId(ACCESSOR_TANGENT, accessorID))continue;
                auto&& accessor = mDocument.accessors.Get(accessorID);
                std::vector<float> data = mResourceReader->ReadBinaryData<float>(mDocument, accessor);
                const int elemCount = 4;
                const int vertexSize = static_cast<int>(data.size()) / elemCount;
                std::vector<Math::Vector4> tangents(vertexSize);
                for (int i = 0; i < vertexSize; i++) {
                    tangents[i] = Math::Vector4(data[i * elemCount], data[i * elemCount + 1], data[i * elemCount + 2], data[i * elemCount + 3]);
                }
                result.emplace_back(tangents);
            }
        }
        return result;
    }

    std::vector<std::vector<Math::Vector2>> GLBLoader::getUVsPerSubMeshes() const {
        std::vector<std::vector<Math::Vector2>> result;
        for (auto&& mesh : mDocument.meshes.Elements()) {
            for (auto&& prim : mesh.primitives) {
                std::string accessorID;
                if (!prim.TryGetAttributeAccessorId(ACCESSOR_TEXCOORD_0, accessorID))continue;
                auto&& accessor = mDocument.accessors.Get(accessorID);
                std::vector<float> data = mResourceReader->ReadBinaryData<float>(mDocument, accessor);
                const int elemCount = 2;
                const int vertexSize = static_cast<int>(data.size()) / elemCount;
                std::vector<Math::Vector2> uvs(vertexSize);
                for (int i = 0; i < vertexSize; i++) {
                    uvs[i] = Math::Vector2(data[i * elemCount], data[i * elemCount + 1]);
                }
                result.emplace_back(uvs);
            }
        }
        return result;
    }
} //Framework::Utility
