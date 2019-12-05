#pragma once

namespace Framework::DX {
    /**
     * @class ShaderResourceView
     * @brief シェーダーリソースビュー
     */
    class ShaderResourceView {
    public:
        /**
         * @brief コンストラクタ
         */
        ShaderResourceView();
        /**
         * @brief デストラクタ
         */
        ~ShaderResourceView();

    private:
        D3D12_SHADER_RESOURCE_VIEW_DESC mView;
    };
} // namespace Framework::DX