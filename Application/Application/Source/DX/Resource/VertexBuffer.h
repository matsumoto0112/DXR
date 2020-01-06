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
        void init(DeviceResource* device, const std::vector<T>& vertices, const std::wstring& name);
        /**
         * @brief コマンドリストにセットする
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        /**
         * @brief バッファの取得
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief バッファビューの取得
         */
        const VertexBufferView& getView() const {
            return mView;
        }

    private:
        Buffer mBuffer = {};
        VertexBufferView mView = {};
        UINT mVertexCount = 0;
    };
    template <class T>
    inline void VertexBuffer::init(
        DeviceResource* device, const std::vector<T>& vertices, const std::wstring& name) {
        mVertexCount = static_cast<UINT>(vertices.size());
        mBuffer.init(device, Buffer::Usage::VertexBuffer,
            static_cast<UINT>(mVertexCount * sizeof(T)), static_cast<UINT>(sizeof(T)), name);
        mBuffer.writeResource(vertices.data(), static_cast<UINT>(mVertexCount * sizeof(T)));
        mView.init(mBuffer);
    }
} // namespace Framework::DX
