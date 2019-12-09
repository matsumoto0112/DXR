#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/ShaderResourceView.h"
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    /**
     * @class Texture2D
     * @brief 2Dテクスチャクラス
     */
    class Texture2D {
    public:
        /**
         * @brief コンストラクタ
         */
        Texture2D() {}
        /**
         * @brief デストラクタ
         */
        ~Texture2D() {}
        /**
         * @brief 初期化処理
         */
        void init(ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
            const Desc::TextureDesc& desc);
        /**
         * @brief シェーダーリソースビューを作成する
         * @param device デバイス
         */
        void createSRV(ID3D12Device* device, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
        /**
         * @brief テクスチャバッファの取得
         */
        const Buffer& getBuffer() const {
            return mBuffer;
        }
        /**
         * @brief ビューの取得
         */
        const ShaderResourceView& getView() const {
            return mView;
        }

    private:
        Buffer mBuffer;
        Buffer mImmediateBuffer;
        DXGI_FORMAT mFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        ShaderResourceView mView;
    };
} // namespace Framework::DX
