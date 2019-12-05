#pragma once
#include "DX/DeviceResource.h"
#include "DX/Resource/Buffer.h"
#include "DX/Resource/VertexBufferView.h"

namespace Framework::DX {
    /**
     * @class VertexBuffer
     * @brief 頂点バッファクラス
     */
    class VertexBuffer {
    public:
        /**
         * @brief コンストラクタ
         */
        VertexBuffer() {}
        /**
         * @brief デストラクタ
         */
        ~VertexBuffer() {}
        /**
         * @brief 初期化
         * @tparam T 頂点構造体
         */
        template <class T>
        void init(ID3D12Device* device, const std::vector<T>& vertices, const std::wstring& name);
        /**
         * @brief コマンドリストにセットする
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        void createSRV(ID3D12Device* device) {
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
            srvDesc.Buffer.NumElements = mVertexCount;
            srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
            srvDesc.Buffer.StructureByteStride = mBuffer.getStride();
            device->CreateShaderResourceView(mBuffer.getResource(), &srvDesc, mCPUHandle);
        }
        //private:
        Buffer mBuffer = {};
        VertexBufferView mView = {};
        UINT mVertexCount = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
    };
    template <class T>
    inline void VertexBuffer::init(
        ID3D12Device* device, const std::vector<T>& vertices, const std::wstring& name) {
        mVertexCount = static_cast<UINT>(vertices.size());
        mBuffer.init(device, Buffer::Usage::VertexBuffer,
            static_cast<UINT>(mVertexCount * sizeof(T)), static_cast<UINT>(sizeof(T)), name);
        mBuffer.writeResource(vertices.data(), static_cast<UINT>(mVertexCount * sizeof(T)));
        mView.init(&mBuffer);
    }
} // namespace Framework::DX
