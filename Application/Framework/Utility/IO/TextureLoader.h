#pragma once
#include <vector>
#include <Windows.h>
#include <filesystem>
#include "Desc/TextureDesc.h"

namespace Framework::Utility {
    /**
    * @namespace TextureLoader
    * @brief テクスチャ読み込み
    */
    namespace TextureLoader {
        /**
        * @brief テクスチャの読み込み
        */
        Desc::TextureDesc load(const std::filesystem::path& path);
        /**
        * @brief メモリからテクスチャを読み込む
        */
        Desc::TextureDesc loadFromMemory(const std::vector<BYTE>& data);
    } //TextureLoader
} //Framework::Utility
