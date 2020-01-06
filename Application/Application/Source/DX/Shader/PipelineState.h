#pragma once
#include "DX/DeviceResource.h"
#include "DX/Shader/RootSignature.h"

namespace Framework::DX {
    struct PipelineStateDesc {
        RootSignature& rootSignature;
        D3D12_SHADER_BYTECODE vs = {};
        D3D12_SHADER_BYTECODE ps = {};
        D3D12_SHADER_BYTECODE ds = {};
        D3D12_SHADER_BYTECODE hs = {};
        D3D12_SHADER_BYTECODE gs = {};
        D3D12_STREAM_OUTPUT_DESC streamOutput = {};
        D3D12_BLEND_DESC blend = {};
        UINT sampleMask = UINT_MAX;
        D3D12_RASTERIZER_DESC rasterizer = {};
        D3D12_DEPTH_STENCIL_DESC depthStencil = {};
        D3D12_INPUT_LAYOUT_DESC inputLayout = {};
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE stripCutValue = {};
        D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = {};
        UINT renderTargetNum = 1;
        DXGI_FORMAT rtvFormat[8] = {};
        DXGI_FORMAT dsvFormat = {};
        DXGI_SAMPLE_DESC sample = { 1, 0 };
        UINT nodeMask = 0;
        D3D12_CACHED_PIPELINE_STATE cachedPSO = {};
        D3D12_PIPELINE_STATE_FLAGS flags
            = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
        PipelineStateDesc(RootSignature& rs) : rootSignature(rs) {}
    };
    /**
     * @class PipelineState
     * @brief discription
     */
    class PipelineState {
    public:
        /**
         * @brief コンストラクタ
         */
        PipelineState() {}
        /**
         * @brief デストラクタ
         */
        ~PipelineState() {}

        void init(DeviceResource* device, const PipelineStateDesc& desc, const std::wstring& name);
        /**
         * @brief コマンドリストにセットする
         */
        void setCommandList(ID3D12GraphicsCommandList* commandList);
        ID3D12PipelineState* getPipelineState() const {
            return mPipeline.Get();
        }

    private:
        Comptr<ID3D12PipelineState> mPipeline;
    };
} // namespace Framework::DX
