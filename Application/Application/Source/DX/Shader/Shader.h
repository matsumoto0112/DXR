/**
 * @file Shader.h
 * @brief シェーダー
 */

#pragma once

namespace Framework::DX {
    /**
     * @class Shader
     * @brief シェーダー
     */
    class Shader {
    protected:
        using ShaderCode = std::vector<BYTE>;

    public:
        /**
         * @brief コンストラクタ
         * @param filepath シェーダーファイルへのパス
         */
        Shader(const std::filesystem::path& filepath);
        /**
         * @brief デストラクタ
         */
        virtual ~Shader();
        /**
         * @brief シェーダーコードの取得
         */
        CD3DX12_SHADER_BYTECODE get() const {
            return CD3DX12_SHADER_BYTECODE(mShaderCode.data(), mShaderCode.size());
        }

    protected:
        ShaderCode mShaderCode;
    };

    /**
     * @class VertexShader
     * @brief 頂点シェーダー
     */
    class VertexShader final : public Shader {
    public:
        /**
         * @brief コンストラクタ
         * @param filepath シェーダーファイルへのパス
         */
        VertexShader(const std::filesystem::path& filepath);
        /**
         * @brief デストラクタ
         */
        ~VertexShader();
        /**
         * @brief 入力レイアウトの取得
         */
        D3D12_INPUT_LAYOUT_DESC getLayout() {
            return { mInputElements.data(), static_cast<UINT>(mInputElements.size()) };
        }

    private:
        std::vector<LPCSTR> mSemanticNames; //!< セマンティクス名
        std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElements; //!< 入力エレメントリスト
    };

    using PixelShader = Shader;
    using GeometryShader = Shader;
} // namespace Framework::DX
