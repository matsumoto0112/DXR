#pragma once

namespace Framework::DX {
    /**
     * @class ShaderResourceView
     * @brief �V�F�[�_�[���\�[�X�r���[
     */
    class ShaderResourceView {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        ShaderResourceView();
        /**
         * @brief �f�X�g���N�^
         */
        ~ShaderResourceView();

    private:
        D3D12_SHADER_RESOURCE_VIEW_DESC mView;
    };
} // namespace Framework::DX