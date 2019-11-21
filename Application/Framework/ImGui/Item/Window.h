#pragma once
#include "ImGui/Item/IItem.h"
#include "Utility/Typedef.h"

namespace Framework::ImGUI {
    /**
    * @class Window
    * @brief ImGUIウィンドウクラス
    */
    class Window {
    public:
        /**
        * @brief コンストラクタ
        * @param name ウィンドウ名
        */
        Window(const std::string& name);
        /**
        * @brief デストラクタ
        */
        ~Window();
        /**
        * @brief アイテムを追加する
        * @param 追加するアイテム
        */
        void addItem(std::shared_ptr<IItem> item);
        /**
        * @brief 描画
        */
        void draw();
    private:
        std::string mName; //!< ウィンドウ名
        std::vector<SharedPtr<IItem>> mItems; //!< ウィンドウにぶら下がるアイテム群
    };
} //Framework::ImGUI
