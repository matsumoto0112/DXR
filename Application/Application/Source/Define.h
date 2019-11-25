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

        Count
    };
} // namespace BottomLevelASType

namespace TopLevelASType {
    enum MyEnum {
        One,
    };
} // namespace TopLevelASType
