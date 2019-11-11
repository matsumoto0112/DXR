#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace Framework::DX {
    /**
    * @brief í∏ì_ç\ë¢ëÃ
    */
    struct Vertex {
        Math::Vector3 position;
        Math::Vector3 normal;
        Math::Vector2 uv;
        Math::Vector4 tangent;
    };

    using Index = UINT16;
} //Framework::DX
