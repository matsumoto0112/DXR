#pragma once

namespace Framework::DX {
    /**
     * @class Handle
     * @brief discription
     */
    template <UINT N>
    struct Handle {
        std::array<D3D12_CPU_DESCRIPTOR_HANDLE, N> handle = {};
        UINT max = 0;
    };
    /**
     * @class DescriptorSet
     * @brief discription
     */
    class DescriptorSet {
    private:
        static constexpr UINT MAX_SRV_NUM = 48;
        static constexpr UINT MAX_CBV_NUM = 16;
        static constexpr UINT MAX_UAV_NUM = 16;
        static constexpr UINT MAX_SAMPLER_NUM = 16;
        using CBVHandle = Handle<MAX_CBV_NUM>;
        using SRVHandle = Handle<MAX_SRV_NUM>;
        using UAVHandle = Handle<MAX_UAV_NUM>;
        using SamplerHandle = Handle<MAX_SAMPLER_NUM>;

    public:
        /**
         * @brief コンストラクタ
         */
        DescriptorSet() {}
        /**
         * @brief デストラクタ
         */
        ~DescriptorSet() {}
        void setCbvHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);
        void setSrvHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);
        void setUavHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);
        void setSamplerHandle(UINT index, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);
        const CBVHandle& getCbvHandle() const {
            return mCbvHandle;
        }
        const SRVHandle& getSrvHandle() const {
            return mSrvHandle;
        }
        const UAVHandle& getUavHandle() const {
            return mUavHandle;
        }
        const SamplerHandle& getSamplerHandle() const {
            return mSamplerHandle;
        }

    private:
        CBVHandle mCbvHandle;
        SRVHandle mSrvHandle;
        UAVHandle mUavHandle;
        SamplerHandle mSamplerHandle;
    };
} // namespace Framework::DX