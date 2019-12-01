/**
 * @file ImGuiManager.h
 * @brief ImGui管理
 */

#pragma once

#include <d3d12.h>
#include "ImGui/ImGuiInclude.h"
#include "Utility/Singleton.h"

namespace Framework {
    /**
     * @class ImGui
     * @brief ImGui管理クラス
     */
    class ImGuiManager {
    public:
        /**
         * @brief コンストラクタ
         * @param hWnd ウィンドウハンドル
         * @param device デバイス
         * @param format レンダーターゲットのフォーマット
         * @param frameCount バックバッファの枚数
         * @param enableImGui ImGuiを有効にするかどうか
         */
        ImGuiManager(HWND hWnd, ID3D12Device* device, DXGI_FORMAT format, UINT frameCount,
            bool enableImGui = true);
        /**
         * @brief デストラクタ
         */
        ~ImGuiManager();
        /**
         * @brief ImGuiを有効にするかどうか
         */
        void setImGuiEnable(bool enable) {
            mEnableImGui = enable;
        }
        /**
         * @brief フレーム開始時に呼ぶ
         */
        void beginFrame();
        /**
         * @brief フレーム終了時に呼ぶ
         */
        void endFrame(ID3D12GraphicsCommandList* commandList);

    private:
        ComPtr<ID3D12DescriptorHeap> mHeap; //!< ImGui用ヒープ
        bool mEnableImGui;
    };
} // namespace Framework
