/**
 * @file Shader.h
 * @brief �V�F�[�_�[
 */

#pragma once

namespace Framework::DX {
    /**
     * @class Shader
     * @brief �V�F�[�_�[
     */
    class Shader {
    protected:
        using ShaderCode = std::vector<BYTE>;

    public:
        /**
         * @brief �R���X�g���N�^
         * @param filepath �V�F�[�_�[�t�@�C���ւ̃p�X
         */
        Shader(const std::filesystem::path& filepath);
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~Shader();
        /**
         * @brief �V�F�[�_�[�R�[�h�̎擾
         */
        CD3DX12_SHADER_BYTECODE get() const {
            return CD3DX12_SHADER_BYTECODE(mShaderCode.data(), mShaderCode.size());
        }

    protected:
        ShaderCode mShaderCode;
    };

    /**
     * @class VertexShader
     * @brief ���_�V�F�[�_�[
     */
    class VertexShader final : public Shader {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param filepath �V�F�[�_�[�t�@�C���ւ̃p�X
         */
        VertexShader(const std::filesystem::path& filepath);
        /**
         * @brief �f�X�g���N�^
         */
        ~VertexShader();
        /**
         * @brief ���̓��C�A�E�g�̎擾
         */
        D3D12_INPUT_LAYOUT_DESC getLayout() {
            return { mInputElements.data(), static_cast<UINT>(mInputElements.size()) };
        }

    private:
        std::vector<LPCSTR> mSemanticNames; //!< �Z�}���e�B�N�X��
        std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElements; //!< ���̓G�������g���X�g
    };

    using PixelShader = Shader;
    using GeometryShader = Shader;
} // namespace Framework::DX
