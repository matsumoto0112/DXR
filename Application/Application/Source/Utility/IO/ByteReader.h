#pragma once

namespace Framework::Utility {
    /**
     * @class ByteReader
     * @brief �t�@�C�����o�C�g�f�[�^�œǂݍ���
     */
    class ByteReader {
    public:
        static std::vector<BYTE> read(_In_ const std::filesystem::path& path);
    };
} // namespace Framework::Utility
