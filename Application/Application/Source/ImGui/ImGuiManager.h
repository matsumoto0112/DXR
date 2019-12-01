/**
 * @file ImGuiManager.h
 * @brief ImGui�Ǘ�
 */

#pragma once

#include <d3d12.h>
#include "ImGui/ImGuiInclude.h"
#include "Utility/Singleton.h"

namespace Framework {
    /**
     * @class ImGui
     * @brief ImGui�Ǘ��N���X
     */
    class ImGuiManager {
    public:
        /**
         * @brief �R���X�g���N�^
         * @param hWnd �E�B���h�E�n���h��
         * @param device �f�o�C�X
         * @param format �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
         * @param frameCount �o�b�N�o�b�t�@�̖���
         * @param enableImGui ImGui��L���ɂ��邩�ǂ���
         */
        ImGuiManager(HWND hWnd, ID3D12Device* device, DXGI_FORMAT format, UINT frameCount,
            bool enableImGui = true);
        /**
         * @brief �f�X�g���N�^
         */
        ~ImGuiManager();
        /**
         * @brief ImGui��L���ɂ��邩�ǂ���
         */
        void setImGuiEnable(bool enable) {
            mEnableImGui = enable;
        }
        /**
         * @brief �t���[���J�n���ɌĂ�
         */
        void beginFrame();
        /**
         * @brief �t���[���I�����ɌĂ�
         */
        void endFrame(ID3D12GraphicsCommandList* commandList);

    private:
        ComPtr<ID3D12DescriptorHeap> mHeap; //!< ImGui�p�q�[�v
        bool mEnableImGui;
    };
} // namespace Framework
