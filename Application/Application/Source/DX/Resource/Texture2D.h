#pragma once
#include "DX/Resource/Buffer.h"
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
        Texture2D();
        /**
         * @brief コンストラクタ
         * @param device デバイス
         * @param desc テクスチャディスク
         */
        Texture2D(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
         * @brief デストラクタ
         */
        ~Texture2D();
        /**
         * @brief バッファを作成する
         * @param device デバイス
         * @param desc テクスチャディスク
         */
        void createBuffer(ID3D12Device* device, const Desc::TextureDesc& desc);
        /**
         * @brief シェーダーリソースビューを作成する
         * @param device デバイス
         */
        void createSRV(ID3D12Device* device);

        //private:
        Buffer mBuffer;
        UINT mWidth = 0;
        UINT mHeight = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
        Desc::TextureFormat mFormat;
    };
} // namespace Framework::DX
