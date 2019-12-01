/**
 * @file ConstantBuffer.h
 * @brief コンスタントバッファ
 */

#pragma once
#include "DX/Util/GPUUploadBuffer.h"

namespace Framework::DX {
    /**
     * @class ConstantBuffer
     * @tparam T 使用する構造体
     * @brief コンスタントバッファ
     */
    template <class T>
    class ConstantBuffer : public GPUUploadBuffer {
    public:
        /**
         * @brief コンストラクタ
         * @param device デバイス
         * @param numInstances 総インスタンス数
         * @param name リソース名
         */
        ConstantBuffer(ID3D12Device* device, UINT numInstances, const std::wstring& name);
        /**
         * @brief デストラクタ
         */
        ~ConstantBuffer();
        /**
         * @brief ステージングの内容をGPUにコピーする
         * @param instanceIndex コピーするインデックス
         */
        void copyStatingToGPU(UINT instanceIndex = 0);
        /**
         * @brief インスタンス数を取得する
         */
        UINT numInstances() const {
            return mNumInstances;
        }
        /**
         * @brief GPUアドレスを取得する
         * @param instanceIndex 取得するインデックス
         */
        D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress(UINT instanceIndex = 0);
        /**
         * @brief ステージングの内容にアクセスする
         */
        inline T& staging() {
            return mStaging;
        }

    private:
        uint8_t* mMappedConstantData; //!< メモリの書き込み範囲
        UINT mAlignedInstanceSize; //!< アラインメントされたバッファサイズ
        UINT mNumInstances; //!< インスタンス数
        T mStaging; //!< コンスタントバッファの値
    };

    //コンストラクタ
    template <class T>
    inline ConstantBuffer<T>::ConstantBuffer(
        ID3D12Device* device, UINT numInstances, const std::wstring& name)
        : mMappedConstantData(nullptr),
          mAlignedInstanceSize(
              Math::MathUtil::alignPow2(sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT)),
          mNumInstances(numInstances) {
        UINT bufferSize = mNumInstances * mAlignedInstanceSize;
        allocate(device, bufferSize, name.c_str());
        mMappedConstantData = getMapCPUWriteOnly();
    }
    //デストラクタ
    template <class T>
    inline ConstantBuffer<T>::~ConstantBuffer() {}
    //GPUにデータをコピーする
    template <class T>
    inline void ConstantBuffer<T>::copyStatingToGPU(UINT instanceIndex) {
        MY_ASSERTION(instanceIndex < mNumInstances, "インデックスが範囲外です");
        memcpy(mMappedConstantData + instanceIndex * mAlignedInstanceSize, &mStaging, sizeof(T));
    }
    //GPUアドレスの取得
    template <class T>
    inline D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer<T>::gpuVirtualAddress(UINT instanceIndex) {
        MY_ASSERTION(instanceIndex < mNumInstances, "インデックスが範囲外です");
        return mResource->GetGPUVirtualAddress() + instanceIndex * mAlignedInstanceSize;
    }

} // namespace Framework::DX
