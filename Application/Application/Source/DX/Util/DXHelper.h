#pragma once
#include <d3d12.h>
#include "Utility/StringUtil.h"

namespace Framework::DX {
    /**
    * @brief sizeをアラインメントする
    */
    inline UINT align(UINT size, UINT alignment) {
        return (size + (alignment - 1)) & ~(alignment - 1);
    }
    /**
    * @brief コンスタントバッファのアラインメントされたバッファサイズを計算する
    */
    inline UINT calcConstantBufferByteSize(UINT byteSize) {
        return align(byteSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
    }
} //Framework::DX
