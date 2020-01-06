/**
 * @file ByteReader.h
 * @brief バイナリデータを読み込み
 */

#pragma once

namespace Framework::Utility {
    /**
     * @class ByteReader
     * @brief ファイルをバイトデータで読み込む
     */
    class ByteReader {
    public:
        /**
         * @brief ファイルの読み込み
         * @param path ファイルパス
         */
        static std::vector<BYTE> read(const std::filesystem::path& path);
    };
} // namespace Framework::Utility
