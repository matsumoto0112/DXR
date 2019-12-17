/**
 * @file Adapter.h
 * @brief �f�o�C�X�A�_�v�^�[
 */

#pragma once

namespace Framework::DX {
    /**
     * @class Adapter
     * @brief �A�_�v�^�[
     */
    class Adapter {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        Adapter() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~Adapter() {}
        /**
         * @brief ������
         */
        void init(IDXGIFactory4* factory);
        /**
         * @brief �A�_�v�^�[�̃��Z�b�g
         */
        void reset();
        /**
         * @brief �A�_�v�^�[�̎擾
         */
        IDXGIAdapter1* getAdapter() const {
            return mAdapter.Get();
        }
        /**
         * @brief �A�_�v�^�[ID�̎擾
         */
        UINT getAdapterID() const {
            return mAdapterID;
        }
        /**
         * @brief �A�_�v�^�[�̐��������擾
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
