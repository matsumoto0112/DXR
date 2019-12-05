#include "RootSignature.h"
#include "Math/MathUtility.h"
#include "Utility\StringUtil.h"

namespace {
    //DX12フラグに変換する
    inline constexpr D3D12_ROOT_SIGNATURE_FLAGS toFlags(Framework::DX::RootSignature::Flags flag) {
        switch (flag) {
        case Framework::DX::RootSignature::Flags::Local:
            return D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
        default: return D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;
        }
    }
} // namespace

namespace Framework::DX {
    //コンストラクタ
    RootSignature::RootSignature(ID3D12Device* device, const RootSignatureDesc& desc) {
        CD3DX12_ROOT_SIGNATURE_DESC rootSig(desc.params.size(), desc.params.data(),
            desc.samplers.size(), desc.samplers.data(), toFlags(desc.flag));
        Comptr<ID3DBlob> blob, error;
        MY_THROW_IF_FAILED_LOG(
            D3D12SerializeRootSignature(
                &rootSig, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error),
            error ? Utility::toString(static_cast<wchar_t*>(error->GetBufferPointer())) : "");
        MY_THROW_IF_FAILED(device->CreateRootSignature(
            1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
    }
    //デストラクタ
    RootSignature::~RootSignature() {}
} // namespace Framework::DX
