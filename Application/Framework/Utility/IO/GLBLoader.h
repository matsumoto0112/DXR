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
        AlphaMode alphaMode;
    };
    /**
    * @brief テクスチャデータ
    */
    struct TextureData {
        std::vector<BYTE> data; //!< テクスチャの中身
        UINT width; //!< テクスチャの幅
        UINT height; //!< テクスチャの高さ
        UINT textureSizePerPixel; //!< ピクセル単位のバイトの大きさ
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
        GLBLoader(const std::wstring& filepath);
        /**
        * @brief デストラクタ
        */
        ~GLBLoader();
        /**
        * @brief 画像データを取得する
        */
        std::vector<TextureData> getImageDatas() const;
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
        std::unique_ptr<Microsoft::glTF::GLBResourceReader> mResourceReader;
        Microsoft::glTF::Document mDocument;
    };
} //Framework::Utility
