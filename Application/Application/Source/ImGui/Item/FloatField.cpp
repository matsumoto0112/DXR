#include "FloatField.h"

namespace Framework::ImGUI {
    //コンストラクタ
    FloatField::FloatField(const std::string& text, float defaultValue)
        :CallBackItem(text), mValue(defaultValue), mMinValue(0.0f), mMaxValue(1.0f) { }
    //デストラクタ
    FloatField::~FloatField() { }
    //描画
    void FloatField::draw() {
        if (ImGui::SliderFloat(mName.c_str(), &mValue, mMinValue, mMaxValue) && mCallBack) {
            mCallBack(mValue);
        }
    }
} //Framework::ImGUI
