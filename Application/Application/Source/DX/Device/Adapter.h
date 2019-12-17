/**
 * @file Adapter.h
 * @brief デバイスアダプター
 */

#pragma once

namespace Framework::DX {
    /**
     * @class Adapter
     * @brief アダプター
     */
    class Adapter {
    public:
        /**
         * @brief コンストラクタ
         */
        Adapter() {}
        /**
         * @brief デストラクタ
         */
        ~Adapter() {}
        /**
         * @brief 初期化
         */
        void init(IDXGIFactory4* factory);
        /**
         * @brief アダプターのリセット
         */
        void reset();
        /**
         * @brief アダプターの取得
         */
        IDXGIAdapter1* getAdapter() const {
            return mAdapter.Get();
        }
        /**
         * @brief アダプターIDの取得
         */
        UINT getAdapterID() const {
            return mAdapterID;
        }
        /**
         * @brief アダプターの説明文を取得
         */
        const std::wstring& getDescription() const {
            return mAdapterDescription;
        }

    private:
        Comptr<IDXGIAdapter1> mAdapter;
        UINT mAdapterID = UINT_MAX;
        std::wstring mAdapterDescription;
    };
} // namespace Framework::DX
