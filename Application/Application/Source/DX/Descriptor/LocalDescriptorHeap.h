#pragma once

namespace Framework::DX {
    /**
     * @class LocalDescriptorHeap
     * @brief ローカルのディスクリプタヒープ
     */
    class LocalDescriptorHeap {
    public:
        /**
         * @brief コンストラクタ
         */
        LocalDescriptorHeap() {}
        /**
         * @brief デストラクタ
         */
        ~LocalDescriptorHeap() {}
        /**
         * @brief 初期化
         */
        void init(ID3D12Device* device, UINT descriptorNum, D3D12_DESCRIPTOR_HEAP_TYPE type);
        /**
         * @brief アロケート処理
         * @param[out] cpuHandle 確保したCPUハンドル
         * @param[out] gpuHandle 確保したGPUハンドル
         * @return アロケートに成功したらtrueを返す
         */
        bool allocate(
            D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle);
        /**
         * @brief ヒープの取得
         */
        ID3D12DescriptorHeap* getHeap() const {
            return mDescriptorHeap.Get();
        }
        /**
         * @brief ディスクリプタの大きさを返す
         */
        UINT getDescriptorHeapSize() const {
            return mDescriptorHeapSize;
        }
        /**
         * @brief ディスクリプタの確保数を返す
         */
        UINT getDescriptorHeapNum() const {
            return mDescriptorHeapNum;
        }

    private:
        Comptr<ID3D12DescriptorHeap> mDescriptorHeap;
        UINT mCurrentAllocatedNum = 0;
        UINT mDescriptorHeapSize = 0;
        UINT mDescriptorHeapNum = 0;
    };
} // namespace Framework::DX