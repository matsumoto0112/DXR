#pragma once
#include <filesystem>
#include <string>
#include <Windows.h>
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include "Desc/TextureDesc.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Utility/Typedef.h"

namespace Framework::Utility {
    /**
    * @brief �A���t�@�̎��
    */
    enum class AlphaMode {
        Opaque,
        Blend,
        Mask,
    };
    /**
    * @brief �}�e���A���f�[�^
    */
    struct Material {
        std::string name;
        int normalMapID;
        int emissiveMapID;
        int metalRoughID;
        AlphaMode alphaMode;

        Material()
            :name(""), normalMapID(-1), emissiveMapID(-1), metalRoughID(-1), alphaMode(AlphaMode::Opaque) { }
    };

    using IndexList = std::vector<UINT16>;
    using PositionList = std::vector<Math::Vector3>;
    using NormalList = std::vector<Math::Vector3>;
    using UVList = std::vector<Math::Vector2>;
    using TangentList = std::vector<Math::Vector4>;

    /**
    * @class GLBLoader
    * @brief .glb�t�@�C���̓ǂݍ���
    */
    class GLBLoader {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        GLBLoader(const std::filesystem::path& filepath);
        /**
        * @brief �f�X�g���N�^
        */
        ~GLBLoader();
        /**
        * @brief �摜�f�[�^���擾����
        */
        std::vector<Desc::TextureDesc> getImageDatas() const;
        /**
        * @brief �}�e���A���f�[�^���擾����
        */
        std::vector<Material> getMaterialDatas() const;
        /**
        * @brief �T�u���b�V�����Ƃ̒��_�C���f�b�N�X���擾����
        */
        std::vector<IndexList> getIndicesPerSubMeshes() const;
        /**
        * @brief �T�u���b�V�����Ƃ̒��_���W���擾����
        */
        std::vector<PositionList> getPositionsPerSubMeshes() const;
        /**
        * @brief �T�u���b�V�����Ƃ̖@�����擾����
        */
        std::vector<NormalList> getNormalsPerSubMeshes() const;
        /**
        * @brief �T�u���b�V�����Ƃ̃^���W�F���g���擾����
        */
        std::vector<TangentList> getTangentsPerSubMeshes() const;
        /**
        * @brief �T�u���b�V�����Ƃ�UV���W���擾����
        */
        std::vector<UVList> getUVsPerSubMeshes() const;
    private:
        UniquePtr<Microsoft::glTF::GLBResourceReader> mResourceReader;
        Microsoft::glTF::Document mDocument;
    };
} //Framework::Utility
