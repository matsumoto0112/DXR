#pragma once
#include <d3d12.h>
#include "Utility/Typedef.h"

namespace Framework::DX {
    /**
    * @brief バッファまとめ
    */
    struct Buffer {
        ComPtr<ID3D12Resource> resource; //!< リソース本体
        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; //!< CPUハンドル
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; //!< GPUハンドル
    };
} //Framework::DX
