#pragma once

#include <wincodec.h>
#include <memory>
#include <vector>
#include <string>
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
/**
* @class TextureLoader
* @brief テクスチャ読み込み機
*/
    class TextureLoader {
    public:
        /**
        * @brief コンストラクタ
        */
        TextureLoader();
        /**
        * @brief デストラクタ
        */
        ~TextureLoader();
        /**
        * @brief テクスチャ読み込み
        * @param filepath ファイルへのパス
        */
        Desc::TextureDesc load(const std::wstring& filepath);
    private:
        IWICBitmapDecoder* mDecoder; //!< デコーダー
        IWICBitmapFrameDecode* mFrame; //!< フレームデコード
        IWICFormatConverter* mConverter; //!< フォーマット変換器
        IWICImagingFactory* mFactory; //!< イメージ生成器
    };

} //Framework::Utility
