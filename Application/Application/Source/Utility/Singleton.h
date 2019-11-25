/**
 * @file Singleton.h
 * @brief シングルトン基底クラス
 */

#pragma once

namespace Framework::Utility {
    /**
     * @class Singleton
     * @brief シングルトン基底クラス
     */
    template <class T>
    class Singleton {
    public:
        /**
         * @brief インスタンスの取得
         */
        static inline T* getInstance() {
            struct temp : T {
                temp() : T() {}
            };
            static temp instance;
            return &instance;
        }

    protected:
        /**
         * @brief コンストラクタ
         */
        Singleton(){};
        /**
         * @brief デストラクタ
         */
        virtual ~Singleton() = default;

    private:
        //コピー禁止
        void operator=(const Singleton& other) = delete;
        Singleton(const Singleton& other) = delete;
    };
} // namespace Framework::Utility
