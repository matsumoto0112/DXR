#include "Text.h"

namespace Framework::ImGUI {
    //コンストラクタ
    Text::Text(const std::string& text)
        :IItem(text) { }
    //デストラクタ
    Text::~Text() { }
    //テキストをセットする
    void Text::setText(const std::string& text) {
        mName = text;
    }
    //描画
    void Text::draw() {
        ImGui::Text(mName.c_str());
    }

} //Framework::ImGUI
