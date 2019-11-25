#include "RootSignature.h"
#include "Math/MathUtility.h"
#include "Utility\StringUtil.h"

namespace {
    static constexpr D3D12_ROOT_SIGNATURE_FLAGS toFlags(Framework::DX::RootSignature::Flags flag) {
        switch (flag) {
        case Framework::DX::RootSignature::None:
            return D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;
        case Framework::DX::RootSignature::Global:
            return D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;
        case Framework::DX::RootSignature::Local:
            return D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
        default: return D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;
        }
    }
} // namespace

namespace Framework::DX {
    RootSignature::RootSignature() {}
    RootSignature::~RootSignature() {}
    void RootSignature::create(ID3D12Device* device, Flags flag,
        const std::vector<CD3DX12_ROOT_PARAMETER>& parameters,
        const std::vector<CD3DX12_STATIC_SAMPLER_DESC>& samplers) {
        CD3DX12_ROOT_SIGNATURE_DESC desc(
            parameters.size(), parameters.data(), samplers.size(), samplers.data(), toFlags(flag));

        ComPtr<ID3DBlob> blob, error;
        MY_THROW_IF_FAILED_LOG(
            D3D12SerializeRootSignature(
                &desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error),
            error ? Utility::toString(static_cast<wchar_t*>(error->GetBufferPointer())) : "");
        MY_THROW_IF_FAILED(device->CreateRootSignature(
            1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
    }
} // namespace Framework::DX
