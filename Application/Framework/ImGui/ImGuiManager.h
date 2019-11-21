#pragma once

#include <d3d12.h>
#include "ImGui/ImGuiInclude.h"
#include "Utility/Singleton.h"
#include "Utility/Typedef.h"

namespace Framework {
    /**
    * @class ImGui
    * @brief ImGui�Ǘ��N���X
    */
    class ImGuiManager {
    public:
        /**
        * @brief �R���X�g���N�^
        */
        ImGuiManager(HWND hWnd, ID3D12Device* device, DXGI_FORMAT format);
        /**
        * @brief �f�X�g���N�^
        */
        ~ImGuiManager();
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
    };
} //Framework
