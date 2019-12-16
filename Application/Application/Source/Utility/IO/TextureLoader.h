/**
 * @file TextureLoader.h
 * @brief テクスチャの読み込み
 */

#pragma once
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
    /**
     * @class TextureLoader
     * @brief テクスチャ読み込み
     */
    class TextureLoader {
    public:
        /**
         * @brief テクスチャの読み込み
         */
        static Desc::TextureDesc load(const std::filesystem::path& path);
        /**
         * @brief メモリからテクスチャを読み込む
         */
        static Desc::TextureDesc loadFromMemory(const std::vector<BYTE>& data);

    private:
        static constexpr UINT BYTES_PER_PIXEL = 4;
    };
} // namespace Framework::Utility
