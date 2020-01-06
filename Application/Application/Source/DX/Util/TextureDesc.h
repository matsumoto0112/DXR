#pragma once

namespace Framework::Desc {
    enum class TextureFlags {
        None,
        RenderTarget,
        DepthStencil,
    };
    /**
     * @brief �e�N�X�`���f�X�N
     */
    struct TextureDesc {
        std::wstring name;
        std::vector<BYTE> pixels;
        UINT width;
        UINT height;
        DXGI_FORMAT format;
        TextureFlags flags = TextureFlags::None;
    };
} // namespace Framework::Desc