#pragma once

namespace Framework::DX {
    class DescriptorSet;
    class DeviceResource;
    /**
     * @class GlobalDescriptorHeap
     * @brief 常にパイプラインにセットされるディスクリプタヒープ
     */
    class GlobalDescriptorHeap {
    public:
        /**
         * @brief コンストラクタ
         */
        GlobalDescriptorHeap() {}
        /**
         * @brief デストラクタ
         */
        ~GlobalDescriptorHeap() {}

        void init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorHeapNum);
        void reset() {
            mAllocatedNum = 0;
        }
        ID3D12DescriptorHeap* getHeap() const {
            return mDescriptorHeap.Get();
        }
        UINT getHeapSize() const {
            return mDescriptorHeapSize;
        }
        bool canCopy(UINT num) const {
            return mAllocatedNum + num < mDescriptorHeap->GetDesc().NumDescriptors;
        }

    //private:
        Comptr<ID3D12DescriptorHeap> mDescriptorHeap;
        UINT mDescriptorHeapSize = 0;
        UINT mAllocatedNum = 0;
        D3D12_DESCRIPTOR_HEAP_TYPE mType = {};
    };
} // namespace Framework::DX