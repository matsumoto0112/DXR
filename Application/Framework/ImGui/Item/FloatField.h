#pragma once
#include "ImGui/Item/CallBackItem.h"

namespace Framework::ImGUI {
    /**
    * @class FloatField
    * @brief ���������_���̓G���A
    */
    class FloatField : public CallBackItem<float> {
    public:
        /**
        * @brief �R���X�g���N�^
        * @param text �e�L�X�g
        * @param defaultValue �f�t�H���g�l
        */
        FloatField(const std::string& text, float defaultValue);
        /**
        * @brief �f�X�g���N�^
        */
        ~FloatField();
        /**
        * @brief �`��
        */
        virtual void draw() override;
        /**
        * @brief ���݂̒l���擾����
        */
        float getValue() const { return mValue; }
        /**
        * @brief ���݂̒l��ݒ肷��
        */
        void setValue(float value) { mValue = value; }
        /**
        * @brief �ŏ��l���擾����
        */
        float getMinValue() const { return mMinValue; }
        /**
        * @brief �ŏ��l��ݒ肷��
        */
        void setMinValue(float value) { mMinValue = value; }
        /**
        * @brief �ő�l���擾����
        */
        float getMaxValue() const { return mMaxValue; }
        /**
        * @brief �ő�l��ݒ肷��
        */
        void setMaxValue(float value) { mMaxValue = value; }
    private:
        float mValue; //!< ���݂̒l
        float mMinValue; //!< �ŏ��l
        float mMaxValue; //!< �ő�l
    };
} //Framework ::ImGUI
