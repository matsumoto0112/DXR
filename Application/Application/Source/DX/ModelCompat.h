#pragma once

#ifdef HLSL
#else
namespace Framework::DX {
#endif
/**
 * @brief í∏ì_ç\ë¢ëÃ
 */
struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec4 tangent;
};

#ifdef HLSL
#else
} // namespace Framework::DX
using Index = UINT16;
#endif
