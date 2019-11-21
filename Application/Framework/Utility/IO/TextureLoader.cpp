#include "TextureLoader.h"
#include <unordered_map>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "Libs/stb/stb_image.h"

#include "Framework/Utility/Debug.h"

namespace {
    inline std::vector<BYTE> toRGBAFormat(int width, int height, int bpp, const BYTE* data) {
        const size_t size = width * height * 4;
        if (bpp == 4) return std::vector<BYTE>(data, data + size);
        std::vector<BYTE> result(size);
        int src, dst;
        for (src = 0, dst = 0; src < width * height * 3; src++, dst++) {
            result[dst] = data[src];
            if (src % 3 == 2) {
                dst++;
                result[dst] = 0xff;
            }
        }
        return result;
    }
}

namespace Framework::Utility::TextureLoader {
    Desc::TextureDesc load(const std::filesystem::path& filepath) {
        int w, h, bpp;
        BYTE* data = stbi_load(toString(filepath.c_str()).c_str(), &w, &h, &bpp, 0);

        size_t size = w * h * bpp;
        Desc::TextureDesc desc = { };

        desc.pixels = toRGBAFormat(w, h, bpp, data);

        desc.width = w;
        desc.height = h;
        desc.format = Desc::TextureFormat::R8G8B8A8;
        desc.name = filepath.filename().replace_extension();

        stbi_image_free(data);
        return desc;
    }
    Desc::TextureDesc loadFromMemory(const std::vector<BYTE>& data) {
        Desc::TextureDesc desc = {};
        desc.format = Desc::TextureFormat::R8G8B8A8;
        int width, height, bpp;

        BYTE* texByte = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data.data()), static_cast<int>(data.size()), &width, &height, &bpp, 0);

        desc.pixels = toRGBAFormat(width, height, bpp, texByte);
        desc.width = width;
        desc.height = height;

        stbi_image_free(texByte);
        return desc;
    }
} //Framework::Utility::TextureLoader
