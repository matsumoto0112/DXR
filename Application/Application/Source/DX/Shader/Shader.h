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
         * @brief
         */
        Shader(const std::filesystem::path& filepath);
        /**
         * @brief
         */
        virtual ~Shader();
        /**
         * @brief
         */
        CD3DX12_SHADER_BYTECODE get() const {
            return CD3DX12_SHADER_BYTECODE(mShaderCode.data(), mShaderCode.size());
        }

    protected:
        ShaderCode mShaderCode;
    };

    /**
     * @class VertexShader
     * @brief discription
     */
    class VertexShader final : public Shader {
    public:
        /**
         * @brief
         */
        VertexShader(const std::filesystem::path& filepath);
        /**
         * @brief
         */
        ~VertexShader();
        /**
         * @brief
         */
        D3D12_INPUT_LAYOUT_DESC getLayout() {
            return { mInputElements.data(), static_cast<UINT>(mInputElements.size()) };
        }

    private:
        std::vector<LPCSTR> mSemanticNames;
        std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElements;
    };

    using PixelShader = Shader;
    using GeometryShader = Shader;
} // namespace Framework::DX
