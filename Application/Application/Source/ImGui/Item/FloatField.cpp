#include "FloatField.h"

namespace Framework::ImGUI {
    //�R���X�g���N�^
    FloatField::FloatField(const std::string& text, float defaultValue)
        :CallBackItem(text), mValue(defaultValue), mMinValue(0.0f), mMaxValue(1.0f) { }
    //�f�X�g���N�^
    FloatField::~FloatField() { }
    //�`��
    void FloatField::draw() {
        if (ImGui::SliderFloat(mName.c_str(), &mValue, mMinValue, mMaxValue) && mCallBack) {
            mCallBack(mValue);
        }
    }
} //Framework::ImGUI
