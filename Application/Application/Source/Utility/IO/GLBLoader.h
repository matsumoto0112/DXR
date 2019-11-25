/**
 * @file GLBLoader.h
 * @brief GLB���f���ǂݍ���
 */

#pragma once
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
    /**
     * @brief �A���t�@�̎��
     */
    enum class GlbAlphaMode {
        Opaque, //!< �s����
        Blend,
        Mask,
    };
    /**
     * @brief �}�e���A���f�[�^
     */
    struct GlbMaterial {
        std::string name;
        int normalMapID;
        int metallicRoughnessMapID;
        int emissiveMapID;
        int occlusionMapID;
        Vec3 emissiveFactor;
        GlbAlphaMode alphaMode;

        GlbMaterial()
            : name(""),
              normalMapID(-1),
              metallicRoughnessMapID(-1),
              emissiveMapID(-1),
              occlusionMapID(-1),
              emissiveFactor(0, 0, 0),
              alphaMode(GlbAlphaMode::Opaque) {}
    };

    using IndexList = std::vector<UINT16>;
    using PositionList = std::vector<Vec3>;
    using NormalList = std::vector<Vec3>;
    using UVList = std::vector<Vec2>;
    using TangentList = std::vector<Vec4>;

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
         * @brief �T�u���b�V���̐����擾����
         */
        UINT getSubmeshesCount() const;
        /**
         * @brief �摜�f�[�^���擾����
         */
        std::vector<Desc::TextureDesc> getImageDatas() const;
        /**
         * @brief �}�e���A���f�[�^���擾����
         */
        std::vector<GlbMaterial> getMaterialDatas() const;
        /**
         * @brief �T�u���b�V���̃}�e���A�������擾����
         */
        std::vector<std::string> getSubmeshesMaterialNames() const;
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
} // namespace Framework::Utility
