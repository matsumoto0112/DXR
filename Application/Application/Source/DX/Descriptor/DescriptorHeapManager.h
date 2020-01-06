#pragma once
#include "DX/Descriptor/DescriptorHeapFlag.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Descriptor/GlobalDescriptorHeap.h"
#include "DX/Raytracing/RaytracingDescriptorHeapManager.h"

namespace Framework::DX {
    /**
     * @class DescriptorHeapManager
     * @brief �f�B�X�N���v�^�q�[�v�Ǘ�
     */
    class DescriptorHeapManager {
    private:
        static constexpr UINT GLOBAL_RESOURCE_HEAP_SIZE = 100000;
        static constexpr UINT GLOBAL_SAMPLER_HEAP_SIZE = 1000;
        static constexpr UINT RESOURCE_VIEW_ALLOCATE_SIZE = 2000;
        static constexpr UINT SAMPLER_VIEW_ALLOCATOR_SIZE = 1000;
        static constexpr UINT RTV_ALLOCATOR_SIZE = 10;
        static constexpr UINT DSV_ALLOCATOR_SIZE = 5;

    public:
        /**
         * @brief �R���X�g���N�^
         */
        DescriptorHeapManager();
        /**
         * @brief �f�X�g���N�^
         */
        ~DescriptorHeapManager();
        /**
         * @brief ������
         * @param device �f�o�C�X
         */
        void init(DeviceResource* device);
        /**
         * @brief �q�[�v�̃A���P�[�g
         * @param type �q�[�v�̎��
         */
        DescriptorInfo allocate(DescriptorHeapType type);
        /**
         * @brief �t���[���J�n���������s��
         */
        void beginFrame();
        /**
         * @brief �f�B�X�N���v�^�̃R�s�[�����A�R���s���[�g�V�F�[�_�[�ɃZ�b�g����
         */
        void copyAndSetGraphicsDescriptorHeap(DescriptorHeapType type, DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& set);
        /**
         * @brief �f�B�X�N���v�^�̃R�s�[�����A�R���s���[�g�V�F�[�_�[�ɃZ�b�g����
         */
        void copyAndSetComputeDescriptorHeap(DescriptorHeapType type, DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& set);
        /**
         * @brief �f�B�X�N���v�^�q�[�v�̃Z�b�g
         * @param commandList �R�}���h���X�g
         * @param types �Z�b�g����q�[�v�̎��
         * @details types��DescriptorHeapType�����󂯕t���Ȃ�
         */
        template <class... Types>
        void setDescriptorHeap(ID3D12GraphicsCommandList* commandList, Types... types);

    private:
        /**
         * @brief �q�[�v�̎�ނ���f�B�X�N���v�^�q�[�v���擾����
         */
        ID3D12DescriptorHeap* getHeapFromType(DescriptorHeapType type);

    private:
        GlobalDescriptorHeap mGlobalHeap;
        GlobalDescriptorHeap mSamplerHeap;
        DescriptorAllocator mCbvSrvUavAllocator;
        DescriptorAllocator mSamplerAllocator;
        DescriptorAllocator mRtvAllocator;
        DescriptorAllocator mDsvAllocator;
        DescriptorInfo mDefaultResourceInfo;
        DescriptorInfo mDefaultSamplerInfo;
        RaytracingDescriptorHeapManager mRaytracingDescriptor;
    };
    template <class... Types>
    inline void DescriptorHeapManager::setDescriptorHeap(
        ID3D12GraphicsCommandList* commandList, Types... types) {
        //��������5�����Ȃ��̂ŗ\��͂��Ȃ��Œǉ����Ă���
        std::vector<ID3D12DescriptorHeap*> heaps;
        for (auto&& type : { types... }) { heaps.emplace_back(getHeapFromType(type)); }
        commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
    }
} // namespace Framework::DX
