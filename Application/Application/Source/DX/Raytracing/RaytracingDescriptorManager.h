#pragma once
#include "DX/Descriptor/DescriptorAllocator.h"
#include "DX/Descriptor/GlobalDescriptorHeap.h"
#include "DX/Raytracing/RaytracingDescriptorHeap.h"

namespace Framework::DX {
    class DeviceResource;
    /**
     * @class RaytracingDescriptorManager
     * @brief ���C�g���[�V���O�p�f�B�X�N���v�^�Ǘ�
     */
    class RaytracingDescriptorManager {
    public:
        /**
         * @brief �R���X�g���N�^
         */
        RaytracingDescriptorManager() {}
        /**
         * @brief �f�X�g���N�^
         */
        ~RaytracingDescriptorManager() {}
        /**
         * @brief ������
         */
        void init(DeviceResource* device);

        DescriptorInfo allocateGlobal();
        DescriptorInfo allocateLocal();
        void copyAndSetComputeDescriptorTable(DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& globalSet);
        //private:
        DeviceResource* mDeviceResource = nullptr;
        DescriptorAllocator mGlobalView; //!<�O���[�o���p�A���P�[�^
        RaytracingDescriptorHeap mHeap; //!<���C�g���[�V���O��p�f�B�X�N���v�^

        DescriptorInfo mDefaultGlobalView;
    };
} // namespace Framework::DX
