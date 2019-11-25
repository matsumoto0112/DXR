#pragma once

namespace Framework::DX {
    /**
     * @brief í∏ì_ç\ë¢ëÃ
     */
    struct Vertex {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
        Vec4 tangent;
    };

    using Index = UINT16;
} // namespace Framework::DX
