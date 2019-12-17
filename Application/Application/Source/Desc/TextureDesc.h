/**
 * @file TextureDesc.h
 * @brief テクスチャディスク
 */

#pragma once

namespace Framework::Desc {
    /**
     * @brief テクスチャの使用フラグ
     */
    enum class TextureFlags {
        None,
        RenderTarget,
        DepthStencil,
        UnorderedAccess,
    };

    /**
     * @struct TextureDesc
     * @brief テクスチャディスク
     */
    struct TextureDesc {
        std::wstring name; //!< テクスチャ名
        std::vector<BYTE> pixels; //!< ピクセルデータ
        UINT width; //!< 幅
        UINT height; //!< 高さ
        DXGI_FORMAT format; //!< フォーマット
        TextureFlags flags = TextureFlags::None; //!< テクスチャの使用フラグ
    };

} // namespace Framework::Desc
