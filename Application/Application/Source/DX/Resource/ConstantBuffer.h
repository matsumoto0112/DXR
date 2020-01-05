#pragma once
#include "DX/Resource/Buffer.h"
#include "DX/Resource/ConstantBufferView.h"

namespace Framework::DX {
    /**
     * @class ConstantBuffer
     * @brief コンスタントバッファ管理クラス
     */
    template <class T>
    class ConstantBuffer {
    public:
        /**
         * @brief コンストラクタ
         */
        ConstantBuffer() {}
        /**
         * @brief コピーコンストラクタ
         */
        ConstantBuffer(const ConstantBuffer& other);
        /**
         * @brief デストラクタ
         */
        ~ConstantBuffer();
        /**
         * @brief コピー
         */
        ConstantBuffer& operator=(const ConstantBuffer& other);
        /**
         * @brief 初期化
         */
        void init(DeviceResource* device, const std::wstring& name);
        /**
         * @brief シェーダーリソースビューを作成する
         * @param device デバイス
         */
        void createCBV(DeviceResource* device, DescriptorHeapFlag flag);
        /**
         * @brief ステージング内容へのアクセス演算子
         */
        T* operator->() {
            return &mStaging;
        }
        /**
         * @brief 現在のステージングを取得する
         */
        T& getStaging() {
            return mStaging;
        }
        /**
         * @brief ビューの取得
         */
        const ConstantBufferView& getView() const {
            return mCBV;
        }
        /**
         * @brief ステージング内容を更新する
         */
        void updateStaging();

    private:
        T mStaging;
        Buffer mBuffer;
        ConstantBufferView mCBV;
        void* mMapped = nullptr;
    };
    template <class T>
    inline ConstantBuffer<T>::ConstantBuffer(const ConstantBuffer& other) {
        *this = other;
    }
    template <class T>
    inline ConstantBuffer<T>::~ConstantBuffer() {
        if (mMapped) {
            mBuffer.unmap();
            mMapped = nullptr;
        }
    }
    template <class T>
    inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(const ConstantBuffer& other) {
        if (this == &other) return *this;
        this->mCBV = other.mCBV;
        this->mStaging = other.mStaging;
        this->mBuffer = other.mBuffer;

        //相手のマップを使うと解放済みにアクセスするので
        //ここでマップ処理をする
        this->mMapped = this->mBuffer.map();
        return *this;
    }
    //初期化
    template <class T>
    inline void ConstantBuffer<T>::init(DeviceResource* device, const std::wstring& name) {
        mBuffer.init(device, Buffer::Usage::ConstantBuffer, sizeof(T), 0, name);

        mMapped = mBuffer.map();
    }
    template <class T>
    inline void ConstantBuffer<T>::createCBV(DeviceResource* device, DescriptorHeapFlag flag) {
        mCBV.init(device, mBuffer, flag);
    }
    //ステージングの更新
    template <class T>
    inline void ConstantBuffer<T>::updateStaging() {
        memcpy(mMapped, &mStaging, sizeof(T));
    }
} // namespace Framework::DX
