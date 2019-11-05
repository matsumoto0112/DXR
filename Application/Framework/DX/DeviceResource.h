#pragma once
#include <array>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"
#include "Window/Window.h"
#include "DX/DescriptorTable.h"

namespace Framework::DX {
    /**
    * @brief �f�o�C�X�C�x���g�ʒm
    */
    class IDeviceNotify {
    public:
        /**
        * @brief �f�X�g���N�^
        */
        virtual  ~IDeviceNotify() { }
        /**
        * @brief �f�o�C�X���X�g
        */
        virtual void onDeviceLost() = 0;
        /**
        * @brief �f�o�C�X�̍ēo�^
        */
        virtual void onDeviceRestored() = 0;
    };

    /**
    * @class DeviceResource
    * @brief DX12�f�o�C�X�Ǘ�
    */
    class DeviceResource {
    public:
        static constexpr UINT ALLOW_TEARING = 0x1; //!< �e�A�����O��������t���O
        static constexpr UINT REQUIRE_TEARING_SUPPORT = 0x2; //!< �e�A�����O���T�|�[�g����ۂ̃t���O
        static constexpr size_t BACK_BUFFER_COUNT = 3; //!< �o�b�N�o�b�t�@�̖���
    public:
        /**
        * @brief �R���X�g���N�^
        * @param backBufferFormat �o�b�N�o�b�t�@�̃t�H�[�}�b�g
        * @param depthBufferFormat �f�v�X�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g
        * @param flags �X���b�v�`�F�C���̃t���O
        */
        DeviceResource(DXGI_FORMAT backBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT depthBufferFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
            UINT flags = 0);
        /**
        * @brief �f�X�g���N�^
        */
        ~DeviceResource();
        /**
        * @brief �A�_�v�^�[�̏�����
        */
        void initializeDXGIAdapter();
        /**
        * @brief �f�o�C�X���\�[�X�̍쐬
        */
        void createDeviceResources();
        /**
        * @brief �E�B���h�E�Ɉˑ����郊�\�[�X�̍쐬
        */
        void createWindowDependentResources();
        /**
        * @brief �E�B���h�E���Z�b�g����
        * @param[in] window �E�B���h�E
        */
        void setWindow(Window::Window* window, UINT width, UINT height);
        /**
        * @brief �E�B���h�E�̃T�C�Y��ύX����
        * @param width �E�B���h�E��
        * @param height �E�B���h�E����
        * @param minimized �ŏ������ǂ���
        */
        bool windowSizeChanged(UINT width, UINT height, bool minimized);
        /**
        * @brief �f�o�C�X���X�g
        */
        void handleDeviceLost();
        /**
        * @brief �f�o�C�X�̃C�x���g�̒ʒm����Z�b�g����
        * @param deviceNotify �ʒm��
        */
        void registerDeviceNotify(IDeviceNotify* deviceNotify);
        /**
        * @brief �`�揀��
        * @param beforeState �O�̃��\�[�X�̏��
        */
        void prepare(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
        /**
        * @brief �v���[���g
        * @param beforeState �O�̃��\�[�X�̏��
        */
        void present(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
        /**
        * @brief �R�}���h�����s����
        */
        void executeCommandList();
        /**
        * @brief GPU�̏�����ҋ@����
        */
        void waitForGPU() noexcept;
    public:
        /**
        * @brief �o�͐�̋�`���擾����
        */
        RECT getOutputSize() const { return mOutputSize; }
        /**
        * @brief �E�B���h�E������Ԃ�
        */
        bool isWindowVisible() const { return mIsWindowVisible; }
        /**
        * @brief �e�A�����O�T�|�[�g���Ă��邩
        */
        bool isTearingSupported() const { return mOptions & ALLOW_TEARING; }
        /**
        * @brief �A�_�v�^���擾����
        */
        IDXGIAdapter1* getAdapter() const { return mAdapter.Get(); }
        /**
        * @brief �f�o�C�X���擾����
        */
        ID3D12Device* getDevice()const { return mDevice.Get(); }
        /**
        * @brief �t�@�N�g�����擾����
        */
        IDXGIFactory4* getFactory() const { return mFactory.Get(); }
        /**
        * @brief �X���b�v�`�F�C�����擾����
        */
        IDXGISwapChain3* getSwapChain() const { return mSwapChain.Get(); }
        /**
        * @brief �@�\���x�����擾����
        */
        D3D_FEATURE_LEVEL getFeatureLevel() const { return mFeatureLevel; }
        /**
        * @brief �����_�[�^�[�Q�b�g���擾����
        */
        ID3D12Resource* getRenderTarget()const { return mRenderTargets[mBackBufferIndex].Get(); }
        /**
        * @brief �f�v�X�E�X�e���V�����擾����
        */
        ID3D12Resource* getDepthStencil() const { return mDepthStencil.Get(); }
        /**
        * @brief �R�}���h�L���[���擾����
        */
        ID3D12CommandQueue* getCommandQueue() const { return mCommandQueue.Get(); }
        /**
        * @brief �R�}���h�A���P�[�^���擾����
        */
        ID3D12CommandAllocator* getCommandAllocator() const { return mCommandAllocators[mBackBufferIndex].Get(); }
        /**
        * @brief �R�}���h���X�g���擾����
        */
        ID3D12GraphicsCommandList* getCommandList() const { return mCommandList.Get(); }
        /**
        * @brief �o�b�N�o�b�t�@�̃t�H�[�}�b�g���擾����
        */
        DXGI_FORMAT getBackBufferFormat() const { return mBackBufferFormat; }
        /**
        * @brief �f�v�X�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g���擾����
        */
        DXGI_FORMAT getDepthBufferFormat() const { return mDepthBufferFormat; }
        /**
        * @brief �r���[�|�[�g���擾����
        */
        D3D12_VIEWPORT getScreenViewport() const { return mScreenViewport; }
        /**
        * @brief �V�U�[��`���擾����
        */
        D3D12_RECT getScissorRect() const { return mScissorRect; }
        /**
        * @brief ���݂̃t���[���C���f�b�N�X���擾����
        */
        UINT getCurrentFrameIndex() const { return mBackBufferIndex; }
        /**
        * @brief �O�̃t���[���C���f�b�N�X���擾����
        */
        UINT getPreviousFrameIndex() const { return mBackBufferIndex == 0 ? BACK_BUFFER_COUNT - 1 : mBackBufferIndex - 1; }
        /**
        * @brief �o�b�N�o�b�t�@�̖������擾����
        */
        UINT getBackBufferCount() const { return BACK_BUFFER_COUNT; }
        /**
        * @brief �f�o�C�X�̃I�v�V�������擾����
        */
        UINT getDeviceOptions() const { return mOptions; }
        /**
        * @brief �A�_�v�^�̐������擾����
        */
        const std::wstring& getAdapterDescription() const { return mAdapterDescription; }
        /**
        * @brief �A�_�v�^ID���擾����
        */
        UINT getAdapterID() const { return mAdapterID; }
        /**
        * @brief �����_�[�^�[�Q�b�g�̃n���h�����擾����
        */
        D3D12_CPU_DESCRIPTOR_HANDLE getRenderTargetView()const {
            return mRTVHeap->getCPUHandle(mBackBufferIndex);
        }
        /**
        * @brief �f�v�X�E�X�e���V���̃n���h�����擾����
        */
        CD3DX12_CPU_DESCRIPTOR_HANDLE getDepthStencilView() const {
            return mDSVHeap->getCPUHandle(0);
        }
    private:
        /**
        * @brief ���̃t���[���ɑJ�ڂ���
        */
        void moveToNextFrame();
        /**
        * @brief �A�_�v�^������������
        */
        void initializeAdapter(IDXGIAdapter1** adapter);
    private:
        UINT mBackBufferIndex; //!< �o�b�N�o�b�t�@�̃C���f�b�N�X
        ComPtr<IDXGIAdapter1> mAdapter; //!< �A�_�v�^
        UINT mAdapterID; //!< �A�_�v�^ID
        std::wstring mAdapterDescription; //!< �A�_�v�^�̐���
        ComPtr<ID3D12Device> mDevice; //!< �f�o�C�X
        ComPtr<ID3D12CommandQueue> mCommandQueue; //!< �R�}���h�L���[
        ComPtr<ID3D12GraphicsCommandList> mCommandList; //!< �R�}���h���X�g
        std::array<ComPtr<ID3D12CommandAllocator>, BACK_BUFFER_COUNT> mCommandAllocators; //!< �R�}���h�A���P�[�^
        ComPtr<IDXGIFactory4> mFactory; //!< �t�@�N�g��
        ComPtr<IDXGISwapChain3> mSwapChain; //!< �X���b�v�`�F�C��
        ComPtr<ID3D12Resource> mRenderTargets[BACK_BUFFER_COUNT]; //!< �����_�[�^�[�Q�b�g
        ComPtr<ID3D12Resource> mDepthStencil; //!< �f�v�X�E�X�e���V��
        ComPtr<ID3D12Fence> mFence; //!< �t�F���X
        std::array<UINT64, BACK_BUFFER_COUNT> mFenceValues; //!< �t�F���X�̒l
        Microsoft::WRL::Wrappers::Event mFenceEvent; //!< �t�F���X�C�x���g
        std::unique_ptr<DescriptorTable> mRTVHeap; //!< �����_�[�^�[�Q�b�g�̃f�B�X�N���v�^�q�[�v
        std::unique_ptr<DescriptorTable> mDSVHeap; //!< �f�v�X�E�X�e���V���̃f�B�X�N���v�^�q�[�v
        D3D12_VIEWPORT mScreenViewport; //!< �r���[�|�[�g
        D3D12_RECT mScissorRect; //!< �V�U�[��`
        DXGI_FORMAT mBackBufferFormat; //!< �o�b�N�o�b�t�@�̃t�H�[�}�b�g
        DXGI_FORMAT mDepthBufferFormat; //!< �f�v�X�E�X�e���V���t�H�[�}�b�g
        D3D_FEATURE_LEVEL mFeatureLevel; //!< �@�\���x��
        RECT mOutputSize; //!< �o�͐�̋�`
        bool mIsWindowVisible; //!< �E�B���h�E������Ԃ�
        UINT mOptions; //!< �f�o�C�X�̃I�v�V����
        Window::Window* mWindow; //!< �E�B���h�E
        IDeviceNotify* mDeviceNotify; //!< �f�o�C�X�C�x���g�̒ʒm��
    };
} //Framework::DX
