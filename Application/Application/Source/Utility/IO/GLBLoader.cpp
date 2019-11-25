#include "GLBLoader.h"
#include <fstream>
#include "Utility/IO/TextureLoader.h"
#include "Utility/StringUtil.h"

namespace {
    /**
     * @class StreamReader
     * @brief �X�g���[���ǂݍ���
     */
    class StreamReader : public Microsoft::glTF::IStreamReader {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        StreamReader() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~StreamReader() {}
        //�f�[�^���擾����
        std::shared_ptr<std::istream> GetInputStream(const std::string& filepath) const override {
            auto stream = std::make_shared<std::ifstream>(filepath, std::ios::binary);
            MY_THROW_IF_FALSE(stream->is_open());
            return stream;
        }
    };
    using Func = std::function<void(
        const Microsoft::glTF::MeshPrimitive& prim)>; // �v���~�e�B�u�P�ʂł̏����֐�
    //���b�V���v���~�e�B�u���Ƃɏ���������
    inline void eachMeshPrimitives(const Microsoft::glTF::Document& doc, Func func) {
        for (auto&& mesh : doc.meshes.Elements()) {
            for (auto&& prim : mesh.primitives) { func(prim); }
        }
    }

    /**
     * @brief Vector2,3,4�̗v�f����int�ŕԂ�
     */
    template <class T>
    inline constexpr int getElementCount() {
        if constexpr (std::is_same<T, Framework::Math::Vector2>::value) return 2;
        if constexpr (std::is_same<T, Framework::Math::Vector3>::value) return 3;
        if constexpr (std::is_same<T, Framework::Math::Vector4>::value) return 4;
        return -1;
    }

    //�A�N�Z�T�̏���T�^�̃��X�g�ɂ��Ď擾����
    template <class T>
    inline std::vector<T> getAttributes(const Microsoft::glTF::GLBResourceReader* reader,
        const Microsoft::glTF::Document& doc, const Microsoft::glTF::MeshPrimitive& prim,
        const char* accessorName) {
        std::string accessorID;
        std::vector<T> result;
        //�A�N�Z�X�ł��Ȃ���΋�̃��X�g��ǉ�����
        if (!prim.TryGetAttributeAccessorId(accessorName, accessorID)) { return result; }

        auto&& accessor = doc.accessors.Get(accessorID);
        std::vector<float> data = reader->ReadBinaryData<float>(doc, accessor);

        //�v�f���͌^�̏�񂩂�擾����
        constexpr int elemCount = getElementCount<T>();
        MY_ASSERTION(elemCount != -1, "T�^���s���ł�");

        const int vertexSize = static_cast<int>(data.size()) / elemCount;
        result.resize(vertexSize);
        for (int i = 0; i < vertexSize; i++) {
            T t;
            //z,w�͑���ł���Ȃ�������
            t.x = data[i * elemCount + 0];
            t.y = data[i * elemCount + 1];
            if constexpr (elemCount >= 3) t.z = data[i * elemCount + 2];
            if constexpr (elemCount >= 4) t.w = data[i * elemCount + 3];
            result[i] = t;
        }
        return result;
    }

    //�v�f�����b�V���̃v���~�e�B�u���Ƃ̃��X�g�Ŏ擾����
    template <class T>
    inline std::vector<std::vector<T>> getAttributesList(
        const Microsoft::glTF::GLBResourceReader* reader, const Microsoft::glTF::Document& doc,
        const char* accessor) {
        std::vector<std::vector<T>> result;
        eachMeshPrimitives(doc, [&](const Microsoft::glTF::MeshPrimitive& prim) {
            result.emplace_back(std::move(getAttributes<T>(reader, doc, prim, accessor)));
        });
        return result;
    }

} // namespace

namespace Framework::Utility {
    //�R���X�g���N�^
    GLBLoader::GLBLoader(const std::filesystem::path& filepath) {
        auto streamReader = std::make_unique<StreamReader>();
        auto glbStream = streamReader->GetInputStream(filepath.generic_string());
        mResourceReader = std::make_unique<Microsoft::glTF::GLBResourceReader>(
            std::move(streamReader), std::move(glbStream));
        auto manifest = mResourceReader->GetJson();
        mDocument = Microsoft::glTF::Deserialize(manifest);

        MY_DEBUG_LOG("%s loaded \n", filepath.filename().generic_string().c_str());
    }
    //�f�X�g���N�^
    GLBLoader::~GLBLoader() {}
    UINT GLBLoader::getSubmeshesCount() const {
        UINT counter = 0;
        for (auto&& mesh : mDocument.meshes.Elements()) {
            counter += static_cast<UINT>(mesh.primitives.size());
        }
        return counter;
    }
    //�摜�̃f�B�X�N���擾����
    std::vector<Desc::TextureDesc> GLBLoader::getImageDatas() const {
        static constexpr UINT SIZE_PER_PIXEL = 4;
        std::vector<Desc::TextureDesc> result;
        for (auto&& image : mDocument.images.Elements()) {
            std::vector<BYTE> texRowData = mResourceReader->ReadBinaryData(mDocument, image);

            Desc::TextureDesc tex = TextureLoader::loadFromMemory(texRowData);
            tex.name = image.name == "" ? L"image" : toWString(image.name);
            result.emplace_back(tex);
        }
        return result;
    }
    //�}�e���A�������擾����
    std::vector<GlbMaterial> GLBLoader::getMaterialDatas() const {
        auto toAlphaMode = [](Microsoft::glTF::AlphaMode mode) {
            switch (mode) {
            case Microsoft::glTF::ALPHA_UNKNOWN:
            case Microsoft::glTF::ALPHA_OPAQUE: return GlbAlphaMode::Opaque;
            case Microsoft::glTF::ALPHA_BLEND: return GlbAlphaMode::Blend;
            default: return GlbAlphaMode::Mask;
            }
        };

        auto toTextureIDIfExist = [](const std::string& id) {
            if (id == "") return -1;
            return std::stoi(id);
        };

        auto toVector3 = [](const Microsoft::glTF::Color3 color) {
            return Math::Vector3(color.r, color.g, color.b);
        };

        std::vector<GlbMaterial> result;
        for (auto&& mat : mDocument.materials.Elements()) {
            GlbMaterial material;
            material.name = mat.name;
            material.normalMapID = toTextureIDIfExist(mat.normalTexture.textureId);
            material.metallicRoughnessMapID
                = toTextureIDIfExist(mat.metallicRoughness.metallicRoughnessTexture.textureId);
            material.emissiveMapID = toTextureIDIfExist(mat.emissiveTexture.textureId);
            material.emissiveFactor = toVector3(mat.emissiveFactor);
            material.occlusionMapID = toTextureIDIfExist(mat.occlusionTexture.textureId);
            material.alphaMode = toAlphaMode(mat.alphaMode);
            result.emplace_back(material);
        }
        return result;
    }
    //�T�u���b�V�����Ƃ̃}�e���A����
    std::vector<std::string> GLBLoader::getSubmeshesMaterialNames() const {
        std::vector<std::string> result;
        eachMeshPrimitives(mDocument, [&](const Microsoft::glTF::MeshPrimitive& prim) {
            result.emplace_back(prim.materialId);
        });
        return result;
    }
    //�T�u���b�V�����Ƃ̃C���f�b�N�X�z��
    std::vector<IndexList> GLBLoader::getIndicesPerSubMeshes() const {
        std::vector<IndexList> result;
        eachMeshPrimitives(mDocument, [&](const Microsoft::glTF::MeshPrimitive& prim) {
            auto&& index_accesor = mDocument.accessors.Get(prim.indicesAccessorId);
            auto data = mResourceReader->ReadBinaryData<uint16_t>(mDocument, index_accesor);
            result.emplace_back(data);
        });
        return result;
    }
    //�T�u���b�V�����Ƃ̒��_���W
    std::vector<std::vector<Math::Vector3>> GLBLoader::getPositionsPerSubMeshes() const {
        return getAttributesList<Math::Vector3>(
            mResourceReader.get(), mDocument, Microsoft::glTF::ACCESSOR_POSITION);
    }
    //�T�u���b�V�����Ƃ̖@��
    std::vector<std::vector<Math::Vector3>> GLBLoader::getNormalsPerSubMeshes() const {
        return getAttributesList<Math::Vector3>(
            mResourceReader.get(), mDocument, Microsoft::glTF::ACCESSOR_NORMAL);
    }
    //�T�u���b�V�����Ƃ̐ڐ�
    std::vector<TangentList> GLBLoader::getTangentsPerSubMeshes() const {
        return getAttributesList<Math::Vector4>(
            mResourceReader.get(), mDocument, Microsoft::glTF::ACCESSOR_TANGENT);
    }
    //�T�u���b�V�����Ƃ�UV���W
    std::vector<std::vector<Math::Vector2>> GLBLoader::getUVsPerSubMeshes() const {
        return getAttributesList<Math::Vector2>(
            mResourceReader.get(), mDocument, Microsoft::glTF::ACCESSOR_TEXCOORD_0);
    }
} // namespace Framework::Utility
