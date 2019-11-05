#pragma once

#include <functional>
#include "ImGui/Item/CallBackItem.h"

namespace Framework::ImGUI {
    /**
    * @class IntField
    * @brief �����l���̓G���A
    */
    class IntField :public CallBackItem<int> {
    public:
        /**
        * @brief �R���X�g���N�^
        * @param text �e�L�X�g
        * @param defaultValue �f�t�H���g�l
        */
        IntField(const std::string& text, int defaultValue);
        /**
        * @brief �f�X�g���N�^
        */
        ~IntField();
        /**
        * @brief ���̒l���擾����
        */
        int getValue() const { return mValue; }
        /**
        * @brief �l��ݒ肷��
        */
        int setValue(int value) { mValue = value; }
        /**
        * @brief �ŏ��l���擾����
        */
        int getMinValue() const { return mMinValue; }
        /**
        * @brief �ŏ��l��ݒ肷��
        */
        void setMinValue(int value) { mMinValue = value; }
        /**
        * @brief �ő�l���擾����
        */
        int getMaxValue() const { return mMaxValue; }
        /**
        * @brief �ő�l��ݒ肷��
        */
        void setMaxValue(int value) { mMaxValue = value; }
        /**
        * @brief �`��
        */
        virtual void draw() override;
    private:
        int mValue; //!< ���̒l
        int mMinValue; //!< �ŏ��l
        int mMaxValue; //!< �ő�l
    };
} //Framework::ImGUI
