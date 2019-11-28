#include "RootSignature.h"
#include "Math/MathUtility.h"
#include "Utility\StringUtil.h"

namespace Framework::DX {
    //コンストラクタ
    RootSignature::RootSignature(ID3D12Device* device, const CD3DX12_ROOT_SIGNATURE_DESC& desc) {
        ComPtr<ID3DBlob> blob, error;
        MY_THROW_IF_FAILED_LOG(
            D3D12SerializeRootSignature(
                &desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error),
            error ? Utility::toString(static_cast<wchar_t*>(error->GetBufferPointer())) : "");
        MY_THROW_IF_FAILED(device->CreateRootSignature(
            1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
    }
    //デストラクタ
    RootSignature::~RootSignature() {}
} // namespace Framework::DX
