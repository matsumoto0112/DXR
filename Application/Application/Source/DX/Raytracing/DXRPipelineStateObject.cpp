#include "DXRPipelineStateObject.h"

namespace Framework::DX {

    DXRPipelineStateObject::DXRPipelineStateObject()
        : mPipelineStateObjectDesc{
              D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE
          } {}
    DXRPipelineStateObject::~DXRPipelineStateObject() {}
} // namespace Framework::DX
