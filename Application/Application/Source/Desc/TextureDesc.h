/**
 * @file TextureDesc.h
 * @brief �e�N�X�`���f�B�X�N
 */

#pragma once

namespace Framework::Desc {
    /**
     * @brief �e�N�X�`���̎g�p�t���O
     */
    enum class TextureFlags {
        None,
        RenderTarget,
        DepthStencil,
        UnorderedAccess,
    };

    /**
     * @struct TextureDesc
     * @brief �e�N�X�`���f�B�X�N
     */
    struct TextureDesc {
        std::wstring name; //!< �e�N�X�`����
        std::vector<BYTE> pixels; //!< �s�N�Z���f�[�^
        UINT width; //!< ��
        UINT height; //!< ����
        DXGI_FORMAT format; //!< �t�H�[�}�b�g
        TextureFlags flags = TextureFlags::None; //!< �e�N�X�`���̎g�p�t���O
    };

} // namespace Framework::Desc
