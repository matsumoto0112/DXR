#pragma once
#include <vector>
#include <Windows.h>
#include <filesystem>
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
    /**
    * @namespace TextureLoader
    * @brief �e�N�X�`���ǂݍ���
    */
    namespace TextureLoader {
        /**
        * @brief �e�N�X�`���̓ǂݍ���
        */
        Desc::TextureDesc load(const std::filesystem::path& path);
        /**
        * @brief ����������e�N�X�`����ǂݍ���
        */
        Desc::TextureDesc loadFromMemory(const std::vector<BYTE>& data);
    } //TextureLoader
} //Framework::Utility
