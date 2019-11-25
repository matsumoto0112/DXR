#include "DescriptorTable.h"
#include <unordered_map>
#include "Utility/Debug.h"

using Framework::Desc::HeapFlag;
using Framework::Desc::HeapType;

namespace {
    //ヒープの種類のテーブル
    static const std::unordered_map<HeapType, D3D12_DESCRIPTOR_HEAP_TYPE> TYPES = {
        { HeapType::CBV_SRV_UAV,
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV },
        { HeapType::Sampler, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER },
        { HeapType::RTV, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV },
        { HeapType::DSV, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV },
    };
    //ヒープのフラグのテーブル
    static const std::unordered_map<HeapFlag, D3D12_DESCRIPTOR_HEAP_FLAGS> FLAGS = {
        { HeapFlag::None, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE },
        { HeapFlag::ShaderVisible,
            D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE },
    };
} // namespace

namespace Framework::DX {
    //コンストラクタ
    DescriptorTable::DescriptorTable(ID3D12Device* device, const Desc::DescriptorTableDesc& desc)
        : mHeap(nullptr), mDescriptorSize(0) {
        create(device, desc);
    }
    //デストラクタ
    DescriptorTable::~DescriptorTable() {}
    //ヒープの作成
    void DescriptorTable::create(ID3D12Device* device, const Desc::DescriptorTableDesc& desc) {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = desc.descriptorNum;
        heapDesc.Type = TYPES.at(desc.type);
        heapDesc.Flags = FLAGS.at(desc.flag);
        heapDesc.NodeMask = 0;
        MY_THROW_IF_FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap)));
        mHeap->SetName(desc.name.c_str());

        mDescriptorSize = device->GetDescriptorHandleIncrementSize(TYPES.at(desc.type));
    }
    //ヒープのリセット
    void DescriptorTable::reset() {
        mHeap.Reset();
        mDescriptorSize = 0;
    }
    //CPUハンドル
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorTable::getCPUHandle(UINT index) {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            mHeap->GetCPUDescriptorHandleForHeapStart(), index, mDescriptorSize);
    }
    //GPUハンドル
    CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorTable::getGPUHandle(UINT index) {
        return CD3DX12_GPU_DESCRIPTOR_HANDLE(
            mHeap->GetGPUDescriptorHandleForHeapStart(), index, mDescriptorSize);
    }
} // namespace Framework::DX
