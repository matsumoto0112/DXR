#pragma once

#include <functional>
#include "ImGui/Item/CallBackItem.h"

namespace Framework::ImGUI {
    /**
    * @class IntField
    * @brief 整数値入力エリア
    */
    class IntField :public CallBackItem<int> {
    public:
        /**
        * @brief コンストラクタ
        * @param text テキスト
        * @param defaultValue デフォルト値
        */
        IntField(const std::string& text, int defaultValue);
        /**
        * @brief デストラクタ
        */
        ~IntField();
        /**
        * @brief 今の値を取得する
        */
        int getValue() const { return mValue; }
        /**
        * @brief 値を設定する
        */
        int setValue(int value) { mValue = value; }
        /**
        * @brief 最小値を取得する
        */
        int getMinValue() const { return mMinValue; }
        /**
        * @brief 最小値を設定する
        */
        void setMinValue(int value) { mMinValue = value; }
        /**
        * @brief 最大値を取得する
        */
        int getMaxValue() const { return mMaxValue; }
        /**
        * @brief 最大値を設定する
        */
        void setMaxValue(int value) { mMaxValue = value; }
        /**
        * @brief 描画
        */
        virtual void draw() override;
    private:
        int mValue; //!< 今の値
        int mMinValue; //!< 最小値
        int mMaxValue; //!< 最大値
    };
} //Framework::ImGUI
