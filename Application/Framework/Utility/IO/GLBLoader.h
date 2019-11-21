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
    * @brief アルファの種類
    */
    enum class AlphaMode {
        Opaque,
        Blend,
        Mask,
    };
    /**
    * @brief マテリアルデータ
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
    * @brief .glbファイルの読み込み
    */
    class GLBLoader {
    public:
        /**
        * @brief コンストラクタ
        */
        GLBLoader(const std::filesystem::path& filepath);
        /**
        * @brief デストラクタ
        */
        ~GLBLoader();
        /**
        * @brief 画像データを取得する
        */
        std::vector<Desc::TextureDesc> getImageDatas() const;
        /**
        * @brief マテリアルデータを取得する
        */
        std::vector<Material> getMaterialDatas() const;
        /**
        * @brief サブメッシュごとの頂点インデックスを取得する
        */
        std::vector<IndexList> getIndicesPerSubMeshes() const;
        /**
        * @brief サブメッシュごとの頂点座標を取得する
        */
        std::vector<PositionList> getPositionsPerSubMeshes() const;
        /**
        * @brief サブメッシュごとの法線を取得する
        */
        std::vector<NormalList> getNormalsPerSubMeshes() const;
        /**
        * @brief サブメッシュごとのタンジェントを取得する
        */
        std::vector<TangentList> getTangentsPerSubMeshes() const;
        /**
        * @brief サブメッシュごとのUV座標を取得する
        */
        std::vector<UVList> getUVsPerSubMeshes() const;
    private:
        UniquePtr<Microsoft::glTF::GLBResourceReader> mResourceReader;
        Microsoft::glTF::Document mDocument;
    };
} //Framework::Utility
