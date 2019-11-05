#pragma once
#include "ImGui/Item/CallBackItem.h"

namespace Framework::ImGUI {
    /**
    * @class FloatField
    * @brief 浮動小数点入力エリア
    */
    class FloatField : public CallBackItem<float> {
    public:
        /**
        * @brief コンストラクタ
        * @param text テキスト
        * @param defaultValue デフォルト値
        */
        FloatField(const std::string& text, float defaultValue);
        /**
        * @brief デストラクタ
        */
        ~FloatField();
        /**
        * @brief 描画
        */
        virtual void draw() override;
        /**
        * @brief 現在の値を取得する
        */
        float getValue() const { return mValue; }
        /**
        * @brief 現在の値を設定する
        */
        void setValue(float value) { mValue = value; }
        /**
        * @brief 最小値を取得する
        */
        float getMinValue() const { return mMinValue; }
        /**
        * @brief 最小値を設定する
        */
        void setMinValue(float value) { mMinValue = value; }
        /**
        * @brief 最大値を取得する
        */
        float getMaxValue() const { return mMaxValue; }
        /**
        * @brief 最大値を設定する
        */
        void setMaxValue(float value) { mMaxValue = value; }
    private:
        float mValue; //!< 現在の値
        float mMinValue; //!< 最小値
        float mMaxValue; //!< 最大値
    };
} //Framework ::ImGUI
