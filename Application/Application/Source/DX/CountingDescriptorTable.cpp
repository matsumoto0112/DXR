#include "CountingDescriptorTable.h"
#include "Utility/Debug.h"

namespace Framework::DX {
    //コンストラクタ
    CountingDescriptorTable::CountingDescriptorTable(
        ID3D12Device* device, const Desc::DescriptorTableDesc& desc)
        : DescriptorTable(device, desc), mIndex(0), mDescriptorNum(desc.descriptorNum) {}
    //デストラクタ
    CountingDescriptorTable::~CountingDescriptorTable() {}
    //ヒープの確保
    void CountingDescriptorTable::allocate(IBuffer* buffer) {
        MY_ASSERTION(mIndex < mDescriptorNum, "確保したヒープの領域を超えています");
        buffer->setCPUHandle(getCPUHandle(mIndex));
        buffer->setGPUHandle(getGPUHandle(mIndex));
        mIndex++;
    }
} // namespace Framework::DX
