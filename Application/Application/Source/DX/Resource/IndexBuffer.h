/**
 * @file IndexBuffer.h
 * @brief インデックスバッファ
 */

#pragma once
#include "DX/Resource/IBuffer.h"

namespace Framework::DX {
    using Index = UINT16;
    /**
     * @class IndexBuffer
     * @brief インデックスバッファ
     */
    class IndexBuffer : public IBuffer {
    public:
        /**
         * @brief
         */
        IndexBuffer(ID3D12Device* device, const std::vector<Index>& indices,
            D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, const std::wstring& name);
        /* *
         * @brief
         */
        ~IndexBuffer();
        void createSRV(ID3D12Device* device);

    private:
        UINT mIndexCount;
        D3D12_PRIMITIVE_TOPOLOGY_TYPE mTopologyType;
    };
} // namespace Framework::DX
