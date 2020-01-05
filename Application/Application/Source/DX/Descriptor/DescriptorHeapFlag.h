#pragma once

namespace Framework::DX {
    enum class DescriptorHeapType {
        CbvSrvUav,
        Sampler,
        Rtv,
        Dsv,
        RaytracingGlobal,
        RaytracingLocal,
    };

} // namespace Framework::DX
