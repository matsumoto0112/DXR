#include "TextureLoader.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "Libs/stb/stb_image.h"

#include "Framework/Utility/Debug.h"

namespace Framework::Utility::TextureLoader {
    Desc::TextureDesc load(const std::filesystem::path& filepath) {
        int w, h, bpp;
        BYTE* data = stbi_load(toString(filepath.c_str()).c_str(), &w, &h, &bpp, 0);

        size_t size = w * h * 4;
        Desc::TextureDesc desc = { };
        int src, dst;
        //アルファ値がないテクスチャデータならアルファデータを追加する
        if (bpp == 3) {
            desc.pixels.resize(size);
            for (src = 0, dst = 0; src < w * h * bpp; src++, dst++) {
                desc.pixels[dst] = data[src];
                if (src % 3 == 2) {
                    dst++;
                    desc.pixels[dst] = 0xff;
                }
            }
        }
        else {
            desc.pixels = std::vector<BYTE>(data, data + size);
        }

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
        const int size = width * height * 4;
        std::vector<BYTE> textureByte;
        int src, dst;
        //アルファ値がないテクスチャデータならアルファデータを追加する
        if (bpp == 3) {
            textureByte.resize(size);
            for (src = 0, dst = 0; src < width * height * bpp; src++, dst++) {
                textureByte[dst] = texByte[src];
                if (src % 3 == 2) {
                    dst++;
                    textureByte[dst] = 0xff;
                }
            }
        }
        else {
            textureByte = std::vector<BYTE>(texByte, texByte + size);
        }

        desc.pixels = textureByte;
        desc.width = width;
        desc.height = height;

        stbi_image_free(texByte);
        return desc;
    }
} //Framework::Utility::TextureLoader
