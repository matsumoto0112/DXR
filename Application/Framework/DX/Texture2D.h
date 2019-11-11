#pragma once
#include <vector>
#include <Windows.h>

namespace Framework::DX {
    /**
    * @brief テクスチャデータ
    */
    struct TextureData {
        std::vector<BYTE> pixels;
        UINT width;
        UINT height;
    };

    /**
    * @class Texture2D
    * @brief 2Dテクスチャクラス
    */
    class Texture2D {
    public:
        /**
        * @brief コンストラクタ
        */
        Texture2D(const TextureData& texture);
        /**
        * @brief デストラクタ
        */
        ~Texture2D();

        void createSRV();
    private:

    };
} //Framework::DX
