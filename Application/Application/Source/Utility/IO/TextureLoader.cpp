#include "TextureLoader.h"
#include <unordered_map>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "Libs/stb/stb_image.h"

#include "Utility/Debug.h"

namespace Framework::Utility {
    //テクスチャの読み込み
    Desc::TextureDesc TextureLoader::load(const std::filesystem::path& filepath) {
        int w, h, bpp;
        BYTE* data = stbi_load(toString(filepath.c_str()).c_str(), &w, &h, &bpp, 4);
        MY_ASSERTION(data, "テクスチャの読み込みに失敗しました。\n%s\n", filepath.string().c_str());

        Desc::TextureDesc desc = {};
        desc.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.width = w;
        desc.height = h;
        desc.pixels = std::vector<BYTE>(data, data + w * h * BYTES_PER_PIXEL);
        desc.name = filepath.filename().replace_extension();

        stbi_image_free(data);
        return desc;
    }
    //テクスチャをデータから作成
    Desc::TextureDesc TextureLoader::loadFromMemory(const std::vector<BYTE>& data) {
        int w, h, bpp;
        BYTE* texByte = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data.data()),
            static_cast<int>(data.size()), &w, &h, &bpp, 4);

        Desc::TextureDesc desc = {};
        desc.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.pixels = std::vector<BYTE>(texByte, texByte + w * h * BYTES_PER_PIXEL);
        desc.width = w;
        desc.height = h;

        stbi_image_free(texByte);
        return desc;
    }
} // namespace Framework::Utility
