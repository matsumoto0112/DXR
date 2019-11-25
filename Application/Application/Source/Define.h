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
    //�}�e���A���Ƃ��Ďg�p����
    //�g�p���郋�[�g�V�O�l�`�����Ƃɕ�����
    //���[�g�V�O�l�`���̕������̓R���X�^���g�o�b�t�@�̍\���̂̎��
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
