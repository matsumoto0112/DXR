#include "Shader.h"
#include "Utility/IO/ByteReader.h"

namespace Framework::DX {
    //コンストラクタ
    Shader::Shader(const std::filesystem::path& filepath) {
        mShaderCode = Utility::ByteReader::read(filepath);
    }
    //デストラクタ
    Shader::~Shader() {
        mShaderCode.clear();
    }

    //コンストラクタ
    VertexShader::VertexShader(const std::filesystem::path& filepath) : Shader(filepath) {
        //シェーダファイルの解析
        const ShaderCode::size_type shaderSize = mShaderCode.size();
        BYTE* pInStruct = nullptr;
        //シェーダーファイルの開始場所を探す
        for (ShaderCode::size_type i = 0; i < shaderSize - 4; i++) {
            if (memcmp(&mShaderCode[i], "ISGN", 4) == NULL) {
                pInStruct = &mShaderCode.at(i);
                break;
            }
        }

        MY_ASSERTION(pInStruct, "入力エレメントが解析できません");

        //変数の数
        const int cntVariable = pInStruct[8];
        mSemanticNames.resize(cntVariable);
        std::vector<UINT> semanticsIndex(cntVariable);
        std::vector<DXGI_FORMAT> format(cntVariable);
        unsigned char* str = &pInStruct[16];

        //変数の解析
        for (int i = 0; i < cntVariable; i++) {
            mSemanticNames[i] = (LPCSTR)(str[i * 24] + pInStruct + 8);
            semanticsIndex[i] = str[i * 24 + 4];
            //ベクトルの次元数
            const char dimension = str[i * 24 + 20];
            //型の種類
            const char variant = str[i * 24 + 12];
            switch (dimension) {
            //四次元
            case '\x0f':
                switch (variant) {
                case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
                    format[i] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
                    break;
                default: format[i] = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN; break;
                }
                break;
                //三次元
            case '\x07':
                switch (variant) {
                case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
                    format[i] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
                    break;
                default: format[i] = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN; break;
                }
                break;
                //二次元
            case '\x03':
                switch (variant) {
                case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
                    format[i] = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
                    break;
                default: format[i] = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN; break;
                }
                break;
                //一次元
            case '\x01':
                switch (variant) {
                case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
                    format[i] = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
                    break;
                case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UINT32:
                    format[i] = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
                    break;
                default: format[i] = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN; break;
                }
                break;
            default: format[i] = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN; break;
            }
        }

        mInputElements.resize(cntVariable);
        for (int i = 0; i < cntVariable; i++) {
            mInputElements[i] = { mSemanticNames[i], semanticsIndex[i], format[i], 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
        }
    }
    //デストラクタ
    VertexShader::~VertexShader() {
        mInputElements.clear();
        mSemanticNames.clear();
    }
} // namespace Framework::DX
