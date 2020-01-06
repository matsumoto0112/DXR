#include "RootSignature.h"
#include "DX/DeviceResource.h"
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
    RootSignature::RootSignature() {}
    //デストラクタ
    RootSignature::~RootSignature() {}
    /**
     * @brief 初期化
     */
    void RootSignature::init(DeviceResource* device, const RootSignatureDesc& desc) {
        CD3DX12_ROOT_SIGNATURE_DESC rootSig(desc.params.size(), desc.params.data(),
            desc.samplers.size(), desc.samplers.data(), toFlags(desc.flag));
        Comptr<ID3DBlob> blob, error;
        MY_THROW_IF_FAILED_LOG(
            D3D12SerializeRootSignature(
                &rootSig, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error),
            error ? Utility::toString(static_cast<wchar_t*>(error->GetBufferPointer())) : "");
        MY_THROW_IF_FAILED(device->getDevice()->CreateRootSignature(
            1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
    }
    void RootSignature::init(
        DeviceResource* device, const std::vector<CD3DX12_STATIC_SAMPLER_DESC>& samplers) {
        constexpr UINT PARAMETER_NUM = 4;
        CD3DX12_DESCRIPTOR_RANGE ranges[PARAMETER_NUM]{

            //コンスタントバッファ
            { D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 16, 0 },
            //シェーダーリソースビュー
            { D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 48, 0 },
            //アンオーダードアクセスビュー
            { D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 16, 0 },
            //サンプラー
            { D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 16, 0 },
        };

        mInputIndex.constantBuffer = 0;
        mInputIndex.shaderResource = 1;
        mInputIndex.unorderedAccess = 2;
        mInputIndex.sampler = 3;

        CD3DX12_ROOT_PARAMETER params[PARAMETER_NUM];
        params[mInputIndex.constantBuffer].InitAsDescriptorTable(1, &ranges[0]);
        params[mInputIndex.shaderResource].InitAsDescriptorTable(1, &ranges[1]);
        params[mInputIndex.unorderedAccess].InitAsDescriptorTable(1, &ranges[2]);
        params[mInputIndex.sampler].InitAsDescriptorTable(1, &ranges[3]);

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(params), params,
            static_cast<UINT>(samplers.size()), samplers.data(),
            D3D12_ROOT_SIGNATURE_FLAGS::
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        Comptr<ID3DBlob> blob, error;
        MY_THROW_IF_FAILED_LOG(
            D3D12SerializeRootSignature(&rootSignatureDesc,
                D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error),
            error ? static_cast<char*>(error->GetBufferPointer()) : "");
        MY_THROW_IF_FAILED(device->getDevice()->CreateRootSignature(
            0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
    }
    void RootSignature::setGraphicsCommandList(ID3D12GraphicsCommandList* commandList) {
        commandList->SetGraphicsRootSignature(mRootSignature.Get());
    }
} // namespace Framework::DX
