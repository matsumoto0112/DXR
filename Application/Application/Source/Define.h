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
    //�}�e���A���Ƃ��Ďg�p����
    //�g�p���郋�[�g�V�O�l�`�����Ƃɕ�����
    //���[�g�V�O�l�`���̕������̓R���X�^���g�o�b�t�@�̍\���̂̎��
    namespace HitGroup {
        namespace Constants {
            enum MyEnum {
                Albedo,
                Texture1,
                SceneConstants,

                Count
            };
        } //Constants
        enum MyEnum {
            Normal,

            Count
        };
    } //HitGroup
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
    } //HitGroupIndex

    inline constexpr UINT rootArgumentSize() {
        return Framework::Math::MathUtil::mymax({ (UINT)sizeof(NormalRootArgument) });
    }
} //LocalRootSignature

namespace RayType {
    enum MyEnum {
        Main,

        Count
    };
} //RayType

namespace BottomLevelASType {
    enum MyEnum {
        UFO,
        Quad,
        Floor,

        Count
    };
} //BottomLevelASType

namespace TopLevelASType {
    enum MyEnum {
        One,
    };
} //TopLevelASType
