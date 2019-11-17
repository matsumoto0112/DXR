#pragma once
#include <Windows.h>
#include <vector>

namespace Framework::Desc {
    /**
    * @brief テクスチャディスク
    */
    struct TextureDesc {
        std::string name; //!< テクスチャ名
        std::vector<BYTE> pixels; //!< ピクセルデータ
        UINT width; //!< 幅
        UINT height; //!< 高さ
    };
} //Framework::Desc
