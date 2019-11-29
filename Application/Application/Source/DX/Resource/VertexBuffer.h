/**
 * @file VertexBuffer.h
 * @brief 頂点バッファ
 */

#pragma once
#include "DX/Resource/IBuffer.h"
#include "DX/Util/Helper.h"

namespace Framework::DX {
    /**
     * @class VertexBuffer
     * @brief 頂点バッファ
     */
    class VertexBuffer : public IBuffer {
    public:
        /**
         * @brief コンストラクタ
         * @tparam T 頂点構造体
         * @param device デバイス
         * @param vertices 頂点データ
         * @param name リソース名
         */
        template <class T>
        VertexBuffer(
            ID3D12Device* device, const std::vector<T>& vertices, const std::wstring& name);
        /**
         * @brief デストラクタ
         */
        ~VertexBuffer();
        /**
         * @brief シェーダーリソースビューを作成する
         * @param device デバイス
         */
        void createSRV(ID3D12Device* device);

    private:
        const UINT mVertexByteLength; //!< 頂点の大きさ
        const UINT mVertexCount; //!< 頂点数
    };
    template <class T>
    inline VertexBuffer::VertexBuffer(
        ID3D12Device* device, const std::vector<T>& vertices, const std::wstring& name)
        : mVertexByteLength(sizeof(T)), mVertexCount(static_cast<UINT>(vertices.size())) {
        size_t size = mVertexByteLength * mVertexCount;
        mResource = createUploadBuffer(device, size, name);
        writeToResource(mResource.Get(), vertices.data(), size);
    }
} // namespace Framework::DX
