/**
 * @file ByteReader.h
 * @brief �o�C�i���f�[�^��ǂݍ���
 */

#pragma once

namespace Framework::Utility {
    /**
     * @class ByteReader
     * @brief �t�@�C�����o�C�g�f�[�^�œǂݍ���
     */
    class ByteReader {
    public:
        /**
         * @brief �t�@�C���̓ǂݍ���
         * @param path �t�@�C���p�X
         */
        static std::vector<BYTE> read(const std::filesystem::path& path);
    };
} // namespace Framework::Utility
