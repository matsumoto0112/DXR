#pragma once
#include <string>
#include <Windows.h>
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include "Framework/Math/Vector2.h"
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Vector4.h"

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
        AlphaMode alphaMode;
    };
    /**
    * @brief �e�N�X�`���f�[�^
    */
    struct TextureData {
        std::vector<BYTE> data; //!< �e�N�X�`���̒��g
        UINT width; //!< �e�N�X�`���̕�
        UINT height; //!< �e�N�X�`���̍���
        UINT textureSizePerPixel; //!< �s�N�Z���P�ʂ̃o�C�g�̑傫��
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
        GLBLoader(const std::wstring& filepath);
        /**
        * @brief �f�X�g���N�^
        */
        ~GLBLoader();
        /**
        * @brief �摜�f�[�^���擾����
        */
        std::vector<TextureData> getImageDatas() const;
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
        std::unique_ptr<Microsoft::glTF::GLBResourceReader> mResourceReader;
        Microsoft::glTF::Document mDocument;
    };
} //Framework::Utility
