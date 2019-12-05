#pragma once

namespace GlobalRootSignature {
    namespace Slot {
        enum MyEnum {
            RenderTarget,
            AccelerationStructure,
            SceneConstant,
            IndexBuffer,
            VertexBuffer,

            Count
        };
    } // namespace Slot
} // namespace GlobalRootSignature

namespace LocalRootSignature {
    namespace Miss {
        enum MyEnum {
            Back,

            Count
        };
    } // namespace Miss
    //マテリアルとして使用する
    //使用するルートシグネチャごとに分ける
    //ルートシグネチャの分け方はコンスタントバッファの構造体の種類
    namespace HitGroup {
        namespace Constants {
            enum MyEnum {
                AlbedoTex,
                NormalMap,
                MetallicRoughnessMap,
                EmissiveMap,
                OcculusionMap,

                SceneConstants,

                Count
            };
        } // namespace Constants
        enum MyEnum {
            Normal,

            Count
        };
    } // namespace HitGroup
    struct NormalRootArgument {
        Color color;
    };
    namespace HitGroupIndex {
        enum MyEnum {
            UFO,
            Quad,
            Floor,
            Sphere,

            Count
        };
    } // namespace HitGroupIndex

    inline constexpr UINT rootArgumentSize() {
        return Framework::Math::MathUtil::mymax({ (UINT)sizeof(NormalRootArgument) });
    }
} // namespace LocalRootSignature

namespace RayType {
    enum MyEnum {
        Main,

        Count
    };
} // namespace RayType

namespace BottomLevelASType {
    enum MyEnum {
        UFO,
        Quad,
        Floor,
        Sphere,

        Count
    };
} // namespace BottomLevelASType

namespace TopLevelASType {
    enum MyEnum {
        One,
    };
} // namespace TopLevelASType

namespace DescriptorHeapIndex {
    enum MyEnum {
        DefaultTexture_Albedo,
        DefaultTexture_NormalMap,
        DefaultTexture_MetallicRoughnessMap,
        DefaultTexture_EmissiveMap,
        DefaultTexture_OcclusionMap,

        UFO_Albedo,
        UFO_NormalMap,
        UFO_MetallicRoughnessMap,
        UFO_EmissiveMap,
        UFO_OcclusionMap,

        Quad_Albedo,

        Plane_Albedo,
        Plane_NormalMap,
        Plane_MetallicRoughnessMap,
        Plane_EmissiveMap,
        Plane_OcclusionMap,

        Sphere_Albedo,
        Sphere_NormalMap,
        Sphere_MetallicRoughnessMap,
        Sphere_EmissiveMap,
        Sphere_OcclusionMap,

        Texture_Checker,

        ResourceIndexBuffer,
        ResourceVertexBuffer,
        RaytracingOutput,
    };
}
