#pragma once
#include "DX/Descriptor/DescriptorHeapFlag.h"
#include "DX/Descriptor/DescriptorSet.h"
#include "DX/Raytracing/RaytracingDescriptorManager.h"

namespace Framework::DX {
    /**
     * @class DescriptorHeapManager
     * @brief �f�B�X�N���v�^�q�[�v�Ǘ�
     */
    class DescriptorHeapManager {
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
         * @brief �f�B�X�N���v�^�q�[�v�̃Z�b�g
         * @param commandList �R�}���h���X�g
         * @param types �Z�b�g����q�[�v�̎��
         * @details types��DescriptorHeapType�����󂯕t���Ȃ�
         */
        template <class... Types>
        auto setDescriptorHeap(ID3D12GraphicsCommandList* commandList, Types... types) -> decltype(
            std::initializer_list<DescriptorHeapType>{ types... }, std::declval<void>());
        /**
         * @brief �f�B�X�N���v�^�̃R�s�[�����A�R���s���[�g�V�F�[�_�[�ɃZ�b�g����
         */
        void copyAndSetComputeDescriptorHeap(DescriptorHeapType type, DeviceResource* device,
            ID3D12GraphicsCommandList* commandList, const DescriptorSet& set);

    private:
        /**
         * @brief �q�[�v�̎�ނ���f�B�X�N���v�^�q�[�v���擾����
         */
        ID3D12DescriptorHeap* getHeapFromType(DescriptorHeapType type);

    private:
        RaytracingDescriptorManager mRaytracingDescriptor;
    };
    template <class... Types>
    inline auto DescriptorHeapManager::setDescriptorHeap(
        ID3D12GraphicsCommandList* commandList, Types... types)
        -> decltype(std::initializer_list<DescriptorHeapType>{ types... }, std::declval<void>()) {
        //��������5�����Ȃ��̂ŗ\��͂��Ȃ��Œǉ����Ă���
        std::vector<ID3D12DescriptorHeap*> heaps;
        for (auto&& type : { types... }) { heaps.emplace_back(getHeapFromType(type)); }
        commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
    }
} // namespace Framework::DX
