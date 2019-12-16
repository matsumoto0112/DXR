/**
 * @file TextureLoader.h
 * @brief �e�N�X�`���̓ǂݍ���
 */

#pragma once
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
    /**
     * @class TextureLoader
     * @brief �e�N�X�`���ǂݍ���
     */
    class TextureLoader {
    public:
        /**
         * @brief �e�N�X�`���̓ǂݍ���
         */
        static Desc::TextureDesc load(const std::filesystem::path& path);
        /**
         * @brief ����������e�N�X�`����ǂݍ���
         */
        static Desc::TextureDesc loadFromMemory(const std::vector<BYTE>& data);

    private:
        static constexpr UINT BYTES_PER_PIXEL = 4;
    };
} // namespace Framework::Utility
