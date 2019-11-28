#pragma once

namespace Framework::DX {
    /**
     * @class DXRPipelineStateObject
     * @brief discription
     */
    class DXRPipelineStateObject {
    public:
        struct HitGroupDesc {
            std::wstring name;
            D3D12_HIT_GROUP_TYPE type;
            std::wstring closestHitName;
            std::wstring anyHitName;
            std::wstring intersectionName;

            HitGroupDesc(const std::wstring& name, D3D12_HIT_GROUP_TYPE type,
                const std::wstring& closestHitName = L"", const std::wstring& anyHitName = L"",
                const std::wstring& intersectionName = L"")
                : name(name),
                  type(type),
                  closestHitName(closestHitName),
                  anyHitName(anyHitName),
                  intersectionName(intersectionName) {}
        };

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
        void bindHitGroup(const HitGroupDesc& desc);
        void setConfig(UINT payloadSize, UINT attributeSize);
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
