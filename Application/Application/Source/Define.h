#pragma once

namespace GlobalRootSignature {
    namespace Slot {
        enum MyEnum {
            RenderTarget,
            AccelerationStructure,
            SceneConstant,
            VertexBuffers,

            Count
        };
    } //Slot
} //GlobalRootSignature

namespace LocalRootSignature {

} //LocalRootSignature

namespace RayType {
    enum MyEnum {
        Main,

        Count
    };
} //RayType

namespace BottomLevelASType {
    enum MyEnum {
        Triangle,

        Count
    };
} //BottomLevelASType

namespace TopLevelASType {
    enum MyEnum {
        One,
    };
} //TopLevelASType
