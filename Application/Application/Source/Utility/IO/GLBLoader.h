/**
 * @file GLBLoader.h
 * @brief GLBモデル読み込み
 */

#pragma once
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
    /**
     * @brief アルファの種類
     */
    enum class GlbAlphaMode {
        Opaque, //!< 不透明
        Blend,
        Mask,
    };
    /**
     * @brief マテリアルデータ
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
         * @brief サブメッシュの数を取得する
         */
        UINT getSubmeshesCount() const;
        /**
         * @brief 画像データを取得する
         */
        std::vector<Desc::TextureDesc> getImageDatas() const;
        /**
         * @brief マテリアルデータを取得する
         */
        std::vector<GlbMaterial> getMaterialDatas() const;
        /**
         * @brief サブメッシュのマテリアル名を取得する
         */
        std::vector<std::string> getSubmeshesMaterialNames() const;
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
} // namespace Framework::Utility
