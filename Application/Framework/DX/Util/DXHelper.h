#pragma once
#include <d3d12.h>
#include "Utility/StringUtil.h"

namespace Framework::DX {
    /**
    * @brief size���A���C�������g����
    */
    inline UINT align(UINT size, UINT alignment) {
        return (size + (alignment - 1)) & ~(alignment - 1);
    }
    /**
    * @brief �R���X�^���g�o�b�t�@�̃A���C�������g���ꂽ�o�b�t�@�T�C�Y���v�Z����
    */
    inline UINT calcConstantBufferByteSize(UINT byteSize) {
        return align(byteSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
    }
} //Framework::DX
