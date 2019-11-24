#include "Text.h"

namespace Framework::ImGUI {
    //�R���X�g���N�^
    Text::Text(const std::string& text)
        :IItem(text) { }
    //�f�X�g���N�^
    Text::~Text() { }
    //�e�L�X�g���Z�b�g����
    void Text::setText(const std::string& text) {
        mName = text;
    }
    //�`��
    void Text::draw() {
        ImGui::Text(mName.c_str());
    }

} //Framework::ImGUI
