#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/IndexBufferView.h"

namespace Framework::DX {
    /**
     * @class IndexBuffer
     * @brief インデックスバッファクラス
     */
    class IndexBuffer {
    public:
        /**
         * @brief コンストラクタ
         */
        IndexBuffer() {}
        /**
         * @brief デストラクタ
         */
        ~IndexBuffer() {}
        /**
         * @brief 初期化処理
         */
        void init(ID3D12Device* device, const std::vector<UINT>& indices,
            D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name);
        /**
         * @brief 初期化処理
         */
        void init(ID3D12Device* device, const std::vector<UINT16>& indices,
            D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name);
        /**
         * @brief コマンドリストにセットする
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief 描画する
         */
        void draw(ID3D12GraphicsCommandList* commandList);
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        void createSRV(ID3D12Device* device) {
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
            srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
            srvDesc.Buffer.NumElements = mIndexNum * mBuffer.getStride() / sizeof(float);
            srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_RAW;
            srvDesc.Buffer.StructureByteStride = 0;
            device->CreateShaderResourceView(mBuffer.getResource(), &srvDesc, mCPUHandle);
        }
        //private:
        Buffer mBuffer; //!< バッファ
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
        IndexBufferView mView; //!< ビュー
        D3D_PRIMITIVE_TOPOLOGY mTopology
            = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; //!< トポロジーの種類
        UINT mIndexNum = 0; //!< インデックス数
    };
} // namespace Framework::DX
