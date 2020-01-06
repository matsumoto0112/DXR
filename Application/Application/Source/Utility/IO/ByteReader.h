#pragma once

namespace Framework::Utility {
    /**
     * @class ByteReader
     * @brief ファイルをバイトデータで読み込む
     */
    class ByteReader {
    public:
        static std::vector<BYTE> read(_In_ const std::filesystem::path& path);
    };
} // namespace Framework::Utility
