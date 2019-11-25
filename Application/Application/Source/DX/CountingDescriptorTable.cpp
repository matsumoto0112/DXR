#include "CountingDescriptorTable.h"
#include "Utility/Debug.h"

namespace Framework::DX {
    //�R���X�g���N�^
    CountingDescriptorTable::CountingDescriptorTable(
        ID3D12Device* device, const Desc::DescriptorTableDesc& desc)
        : DescriptorTable(device, desc), mIndex(0), mDescriptorNum(desc.descriptorNum) {}
    //�f�X�g���N�^
    CountingDescriptorTable::~CountingDescriptorTable() {}
    //�q�[�v�̊m��
    void CountingDescriptorTable::allocate(IBuffer* buffer) {
        MY_ASSERTION(mIndex < mDescriptorNum, "�m�ۂ����q�[�v�̗̈�𒴂��Ă��܂�");
        buffer->setCPUHandle(getCPUHandle(mIndex));
        buffer->setGPUHandle(getGPUHandle(mIndex));
        mIndex++;
    }
} // namespace Framework::DX
