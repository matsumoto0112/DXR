/**
 * @file Singleton.h
 * @brief �V���O���g�����N���X
 */

#pragma once

namespace Framework::Utility {
    /**
     * @class Singleton
     * @brief �V���O���g�����N���X
     */
    template <class T>
    class Singleton {
    public:
        /**
         * @brief �C���X�^���X�̎擾
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
         * @brief �R���X�g���N�^
         */
        Singleton(){};
        /**
         * @brief �f�X�g���N�^
         */
        virtual ~Singleton() = default;

    private:
        //�R�s�[�֎~
        void operator=(const Singleton& other) = delete;
        Singleton(const Singleton& other) = delete;
    };
} // namespace Framework::Utility
