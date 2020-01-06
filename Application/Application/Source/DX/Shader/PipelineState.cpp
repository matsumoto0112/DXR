#include "PipelineState.h"

namespace Framework::DX {

    void PipelineState::init(
        DeviceResource* device, const PipelineStateDesc& desc, const std::wstring& name) {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = desc.rootSignature.getRootSignature();
        psoDesc.VS = desc.vs;
        psoDesc.PS = desc.ps;
        psoDesc.DS = desc.ds;
        psoDesc.HS = desc.hs;
        psoDesc.GS = desc.gs;
        psoDesc.StreamOutput = desc.streamOutput;
        psoDesc.BlendState = desc.blend;
        psoDesc.SampleMask = desc.sampleMask;
        psoDesc.RasterizerState = desc.rasterizer;
        psoDesc.DepthStencilState = desc.depthStencil;
        psoDesc.InputLayout = desc.inputLayout;
        psoDesc.IBStripCutValue = desc.stripCutValue;
        psoDesc.PrimitiveTopologyType = desc.topologyType;
        psoDesc.NumRenderTargets = desc.renderTargetNum;
        memcpy(psoDesc.RTVFormats, desc.rtvFormat, sizeof(psoDesc.RTVFormats));
        psoDesc.DSVFormat = desc.dsvFormat;
        psoDesc.SampleDesc = desc.sample;
        psoDesc.NodeMask = desc.nodeMask;
        psoDesc.CachedPSO = desc.cachedPSO;
        psoDesc.Flags = desc.flags;

        MY_THROW_IF_FAILED(
            device->getDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipeline)));

        MY_THROW_IF_FAILED(mPipeline->SetName(name.c_str()));
    }
    void PipelineState::setCommandList(ID3D12GraphicsCommandList* commandList) {
        commandList->SetPipelineState(mPipeline.Get());
    }
} // namespace Framework::DX
