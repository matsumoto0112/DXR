#include "DescriptorTable.h"
#include <unordered_map>
#include "Utility/Debug.h"

namespace {
    //ヒープの種類のテーブル
    static const std::unordered_map<Framework::DX::HeapType, D3D12_DESCRIPTOR_HEAP_TYPE> TYPES =
    {
        {Framework::DX::HeapType::CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV},
        {Framework::DX::HeapType::Sampler, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER},
        {Framework::DX::HeapType::RTV, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV},
        {Framework::DX::HeapType::DSV, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV},
    };
    //ヒープのフラグのテーブル
    static const std::unordered_map<Framework::DX::HeapFlag, D3D12_DESCRIPTOR_HEAP_FLAGS> FLAGS =
    {
        {Framework::DX::HeapFlag::None,D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE },
        {Framework::DX::HeapFlag::ShaderVisible,D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE },
    };
}

namespace Framework::DX {
    //コンストラクタ
    DescriptorTable::DescriptorTable()
        :mHeap(nullptr), mDescriptorSize(0), mAllocatedNum(0) { }
    //デストラクタ
    DescriptorTable::~DescriptorTable() { }
    //ヒープの作成
    void DescriptorTable::create(ID3D12Device* device, HeapType heapType, HeapFlag heapFlag, UINT descriptorNum, LPCWSTR name) {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = descriptorNum;
        heapDesc.Type = TYPES.at(heapType);
        heapDesc.Flags = FLAGS.at(heapFlag);
        heapDesc.NodeMask = 0;
        device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap));
        mHeap->SetName(name);

        mDescriptorSize = device->GetDescriptorHandleIncrementSize(TYPES.at(heapType));
    }
    //ヒープのリセット
    void DescriptorTable::reset() {
        mHeap.Reset();
        mDescriptorSize = 0;
        mAllocatedNum = 0;
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorTable::getCPUHandle(UINT index) {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(mHeap->GetCPUDescriptorHandleForHeapStart(), index, mDescriptorSize);
    }
    CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorTable::getGPUHandle(UINT index) {
        return CD3DX12_GPU_DESCRIPTOR_HANDLE(mHeap->GetGPUDescriptorHandleForHeapStart(), index, mDescriptorSize);
    }
} //Framework::DX
