#pragma once
#include <Windows.h>
#include <vector>

namespace Framework::Desc {
    enum class TextureFormat {
        R8G8B8A8,
    };

    /**
    * @brief テクスチャディスク
    */
    struct TextureDesc {
        std::wstring name; //!< テクスチャ名
        std::vector<BYTE> pixels; //!< ピクセルデータ
        UINT width; //!< 幅
        UINT height; //!< 高さ
        TextureFormat format; //!< フォーマット
    };
} //Framework::Desc
