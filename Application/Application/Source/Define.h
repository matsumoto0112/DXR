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
    } //Slot
} //GlobalRootSignature

namespace LocalRootSignature {
    namespace Miss {
        enum MyEnum {
            Back,

            Count
        };
    } //Miss
    //マテリアルとして使用する
    //使用するテクスチャなどの種類ごとに作成する必要がある
    namespace HitGroup {
        enum MyEnum {
            WaterTower,
            Quad,

            Count
        };
    } //HitGroup
    struct WaterTowerRootArgument {
        Color color;
    };
    struct QuadRootArgument {
        Color color;
    };
} //LocalRootSignature

namespace RayType {
    enum MyEnum {
        Main,

        Count
    };
} //RayType

namespace BottomLevelASType {
    enum MyEnum {
        WaterTower,
        Quad,

        Count
    };
} //BottomLevelASType

namespace TopLevelASType {
    enum MyEnum {
        One,
    };
} //TopLevelASType
