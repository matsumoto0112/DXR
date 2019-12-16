/**
 * @file IndexBuffer.h
 * @brief インデックスバッファ
 */

#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/IndexBufferView.h"
#include "DX/Resource/ShaderResourceView.h"

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
        template <class T>
        void init(DeviceResource* device, const std::vector<T>& indices,
            D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name);
        /**
         * @brief コマンドリストにセットする
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief 描画する
         */
        void draw(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief バッファの取得
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief シェーダーリソースビューを作成する
         */
        ShaderResourceView createSRV(DeviceResource* device,
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

    private:
        Buffer mBuffer; //!< バッファ
        IndexBufferView mView; //!< ビュー
        D3D_PRIMITIVE_TOPOLOGY mTopology
            = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; //!< トポロジーの種類
        UINT mIndexNum = 0; //!< インデックス数
    };
    //初期化
    template <class T>
    inline void IndexBuffer::init(DeviceResource* device, const std::vector<T>& indices,
        D3D_PRIMITIVE_TOPOLOGY topology, const std::wstring& name) {
        mIndexNum = static_cast<UINT>(indices.size());
        mTopology = topology;
        mBuffer.init(device, Buffer::Usage::IndexBuffer, mIndexNum * sizeof(T), sizeof(T), name);
        mBuffer.writeResource(indices.data(), mIndexNum * sizeof(T));
        mView.init(mBuffer);
    }
} // namespace Framework::DX
