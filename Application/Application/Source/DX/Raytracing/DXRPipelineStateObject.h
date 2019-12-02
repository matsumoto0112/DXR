/**
 * @file DXRPipelineStateObject.h
 * @brief レイトレーシング用パイプライン
 */

#pragma once
#include "DX/Raytracing/DXRDevice.h"
#include "DX/Raytracing/Shader/ShaderTable.h"

namespace Framework::DX {
    class RootSignature;
    /**
     * @brief シェーダーの種類
     */
    enum class ShaderType {
        RayGeneration,
        Miss,
        HitGroup,
    };
    /**
     * @class DXRPipelineStateObject
     * @brief レイトレーシング用パイプライン
     */
    class DXRPipelineStateObject {
    public:
        /**
         * @struct HitGroupDesc
         * @brief ヒットグループディスク
         */
        struct HitGroupDesc {
            std::wstring name; //!< ヒットグループ名
            D3D12_HIT_GROUP_TYPE type; //!< ヒットグループの種類
            std::wstring closestHitName; //!< closesthitシェーダー名
            std::wstring anyHitName; //!< anyhitシェーダー名
            std::wstring intersectionName; //!< intersectionシェーダー名
            /**
             * @brief コンストラクタ
             */
            HitGroupDesc(const std::wstring& name, D3D12_HIT_GROUP_TYPE type,
                const std::wstring& closestHitName = L"", const std::wstring& anyHitName = L"",
                const std::wstring& intersectionName = L"")
                : name(name),
                  type(type),
                  closestHitName(closestHitName),
                  anyHitName(anyHitName),
                  intersectionName(intersectionName) {}
        };

        /**
         * @brief シェーダーデータ
         */
        struct ShaderData {
            ShaderType type;
            std::wstring name;
            void* id;
        };
        /**
         * @brief シェーダーリソース
         */
        struct ShaderResource {
            ComPtr<ID3D12Resource> resource; //!< リソース
            UINT stride; //!< シェーダーテーブルのストライド
        };

    public:
        /**
         * @brief コンストラクタ
         */
        DXRPipelineStateObject(DXRDevice* device);
        /**
         * @brief デストラクタ
         */
        ~DXRPipelineStateObject();
        /**
         * @brief シェーダーファイルからシェーダーを取り出す
         * @param shaderCode シェーダーコード
         * @param byteLength シェーダーコードの長さ
         * @param names 取り出すシェーダーのエントリーポイント名
         */
        template <class... T>
        void exportShader(void* shaderCode, size_t byteLength, const T&... names);
        /**
         * @brief ヒットグループを結びつける
         */
        void bindHitGroup(const HitGroupDesc& desc);
        /**
         * @brief レイトレーシングの設定を行う
         */
        void setConfig(UINT payloadSize, UINT attributeSize, UINT maxRecursionDepth);
        /**
         * @brief ローカルルートシグネチャのバインドをする
         * @param localRootSignature ローカルルートシグネチャ
         * @param targetShaderName 対象となるシェーダー名
         */
        void bindLocalRootSignature(
            const RootSignature& localRootSignature, const std::wstring& targetShaderName);
        /**
         * @brief グローバルルートシグネチャのバインドをする
         */
        void bindGlobalRootSignature(const RootSignature& rootSignature);
        /**
         * @brief パイプラインの作成
         */
        void createPipeline();
        /**
         * @brief パイプラインの取得
         */
        ID3D12StateObject* getStateObject() const {
            return mPipelineStateObject.Get();
        }
        /**
         * @brief シェーダー情報とキーを関連付ける
         */
        void associateShaderInfoWithKey(int key, ShaderType type, const std::wstring& name);
        /**
         * @brief ビルドの事前準備
         */
        void prebuild();
        /**
         * @brief シェーダータイプに対する情報を設定する
         * @param type シェーダーの種類
         * @param num テーブルの要素数
         * @param appendSize 追加するArgumentの大きさ
         * @param name テーブル名
         */
        void setShaderTableConfig(
            ShaderType type, UINT num, UINT appendSize, const std::wstring& name);
        /**
         * @brief シェーダーテーブルに要素を追加する
         * @param key シェーダーキー
         * @param rootArgument シェーダーに対する追加データ
         */
        void appendShaderTable(int key, void* rootArgument = nullptr);
        /**
         * @brief シェーダーテーブルを構築する
         */
        void buildShaderTable();
        /**
         * @brief レイトレーシングを実行する
         */
        void doRaytracing(UINT width, UINT height);

    private:
        DXRDevice* mDevice; //!< DXR用デバイス
        CD3DX12_STATE_OBJECT_DESC mPipelineStateObjectDesc; //!< パイプラインディスク
        ComPtr<ID3D12StateObject> mPipelineStateObject; //!< パイプラインオブジェクト
        std::unordered_map<int, ShaderData> mShaderDatas; //!< シェーダーのデータ
        std::unordered_map<ShaderType, UniquePtr<ShaderTable>>
            mShaderTables; //!< シェーダーテーブルリスト
        std::unordered_map<ShaderType, ShaderResource>
            mShaderResources; //!< シェーダーリソースリスト
    };

    //シェーダーの読み込み
    template <class... T>
    inline void DXRPipelineStateObject::exportShader(
        void* shaderCode, size_t byteLength, const T&... names) {
        CD3DX12_DXIL_LIBRARY_SUBOBJECT* lib
            = mPipelineStateObjectDesc.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
        lib->SetDXILLibrary(&CD3DX12_SHADER_BYTECODE(shaderCode, byteLength));

        for (auto&& name : std::initializer_list<std::wstring>{ names... }) {
            lib->DefineExport(name.c_str());
        }
    }
} // namespace Framework::DX
