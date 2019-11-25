/**
 * @file TextureDesc.h
 * @brief テクスチャディスク
 */

#pragma once

namespace Framework::Desc {
    /**
     * @enum TextureFormat
     * @brief テクスチャのフォーマット
     */
    enum class TextureFormat { R8G8B8A8, UNKNOWN };

    /**
     * @struct TextureDesc
     * @brief テクスチャディスク
     */
    struct TextureDesc {
        std::wstring name; //!< テクスチャ名
        std::vector<BYTE> pixels; //!< ピクセルデータ
        UINT width; //!< 幅
        UINT height; //!< 高さ
        TextureFormat format; //!< フォーマット
    };
} // namespace Framework::Desc
