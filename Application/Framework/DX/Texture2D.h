#pragma once
#include <vector>
#include <Windows.h>
#include "Desc/TextureDesc.h"

namespace Framework::DX {
    /**
    * @class Texture2D
    * @brief 2Dテクスチャクラス
    */
    class Texture2D {
    public:
        /**
        * @brief コンストラクタ
        */
        Texture2D(const Desc::TextureDesc& desc);
        /**
        * @brief デストラクタ
        */
        ~Texture2D();

        void createSRV();
    private:

    };
} //Framework::DX
