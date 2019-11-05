#pragma once
#include <d3d12.h>
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @brief �o�b�t�@�܂Ƃ�
    */
    struct Buffer {
        ComPtr<ID3D12Resource> resource; //!< ���\�[�X�{��
        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; //!< CPU�n���h��
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; //!< GPU�n���h��
    };
} //Framework::DX
