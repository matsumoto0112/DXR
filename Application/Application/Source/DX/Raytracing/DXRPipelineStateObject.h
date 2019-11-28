#pragma once

namespace Framework::DX {
    /**
     * @class DXRPipelineStateObject
     * @brief discription
     */
    class DXRPipelineStateObject {
    public:
        /**
         * @brief
         */
        DXRPipelineStateObject();
        /**
         * @brief
         */
        ~DXRPipelineStateObject();
        template <class... T>
        void exportShader(void* shaderCode, size_t byteLength, const T&... names);

        //private:
        CD3DX12_STATE_OBJECT_DESC mPipelineStateObjectDesc;
        ComPtr<ID3D12StateObject> mPipelineStateObject;
    };
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
