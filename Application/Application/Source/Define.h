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
