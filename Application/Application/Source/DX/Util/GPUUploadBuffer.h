/**
 * @file GPUUploadBuffer.h
 * @brief GPUにアップロード可能なバッファ
 */

#pragma once

namespace Framework::DX {
    /**
     * @class GPUUploadBuffer
     * @brief GPUにアップロードするバッファ
     */
    class GPUUploadBuffer {
    public:
        /**
         * @brief コンストラクタ
         */
        GPUUploadBuffer();
        /**
         * @brief デストラクタ
         */
        virtual ~GPUUploadBuffer();
        /**
         * @brief メモリのアロケート処理
         * @device デバイス
         * @bufferSize バッファの大きさ
         * @param name リソース名
         */
        void allocate(ID3D12Device* device, UINT bufferSize, LPCWSTR name);
        /**
         * @brief CPU書き込み用のメモリを返す
         */
        uint8_t* getMapCPUWriteOnly();
        /**
         * @brief リソースを取得する
         */
        Comptr<ID3D12Resource> getResource() const {
            return mResource;
        }

    protected:
        Comptr<ID3D12Resource> mResource; //!< リソース
    };
} // namespace Framework::DX
