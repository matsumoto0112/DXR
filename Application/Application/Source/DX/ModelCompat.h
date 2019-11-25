#pragma once

namespace Framework::DX {
    /**
     * @brief ���_�\����
     */
    struct Vertex {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
        Vec4 tangent;
    };

    using Index = UINT16;
} // namespace Framework::DX
