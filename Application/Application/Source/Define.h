#pragma once

namespace GlobalRootSignature {
    namespace Slot {
        enum MyEnum {
            Cbv,
            Srv,
            Uav,

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
                OcclusionMap,

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
            Floor,
            Sphere,

            Count
        };
    } // namespace HitGroupIndex

    inline constexpr UINT rootArgumentSize() {
        return Framework::Math::MathUtil::mymax({ (UINT)sizeof(NormalRootArgument) });
    }
} // namespace LocalRootSignature
