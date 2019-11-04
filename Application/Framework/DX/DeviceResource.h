#pragma once
#include <array>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "Libs/d3dx12.h"
#include "Utility/Typedef.h"
#include "Window/Window.h"

namespace Framework::DX {

    class IDeviceNotify {
    public:
        virtual  ~IDeviceNotify() { }
        virtual void onDeviceLost() = 0;
        virtual void onDeviceRestored() = 0;
    };

    /**
    * @class DeviceResource
    * @brief DX12デバイス管理
    */
    class DeviceResource {
    public:
        static constexpr UINT REQUIRE_TEARING_SUPPORT = 0x2;
        static constexpr size_t MAX_BACK_BUFFER_COUNT = 3;
    public:
        /**
        * @brief コンストラクタ
        */
        DeviceResource(DXGI_FORMAT backBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT depthBufferFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
            UINT flags = 0);
        /**
        * @brief デストラクタ
        */
        ~DeviceResource();
        void initializeDXGIAdapter();
        void createDeviceResources();
        void createWindowDependentResources();
        void setWindow(Window::Window* window);
        bool windowSizeChanged(UINT width, UINT height, bool minimized);
        void handleDeviceLost();
        void registerDeviceNotify(IDeviceNotify* deviceNotify);
        void prepare(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
        void present(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
        void executeCommandList();
        void waitForGPU() noexcept;
    public:
        RECT getOutputSize() const { return mOutputSize; }
        bool isWindowVisible() const { return mIsWindowVisible; }
        bool isTearingSupported() const { return mOptions & ALLOW_TEARING; }

        IDXGIAdapter1* getAdapter() const { return mAdapter.Get(); }
        ID3D12Device* getDevice()const { return mDevice.Get(); }
        IDXGIFactory4* getFactory() const { return mFactory.Get(); }
        IDXGISwapChain3* getSwapChain() const { return mSwapChain.Get(); }
        D3D_FEATURE_LEVEL getFeatureLevel() const { return mFeatureLevel; }
        ID3D12Resource* getRenderTarget()const { return mRenderTargets[mBackBufferIndex].Get(); }
        ID3D12Resource* getDepthStencil() const { return mDepthStencil.Get(); }
        ID3D12CommandQueue* getCommandQueue() const { return mCommandQueue.Get(); }
        ID3D12CommandAllocator* getCommandAllocator() const { return mCommandAllocators[mBackBufferIndex].Get(); }
        ID3D12GraphicsCommandList* getCommandList() const { return mCommandList.Get(); }
        DXGI_FORMAT getBackBufferFormat() const { return mBackBufferFormat; }
        DXGI_FORMAT getDepthBufferFormat() const { return mDepthBufferFormat; }
        D3D12_VIEWPORT getScreenViewport() const { return mScreenViewport; }
        D3D12_RECT getScissorRect() const { return mScissorRect; }
        UINT getCurrentFrameIndex() const { return mBackBufferIndex; }
        UINT getPreviousFrameIndex() const { return mBackBufferIndex == 0 ? MAX_BACK_BUFFER_COUNT - 1 : mBackBufferIndex - 1; }
        UINT getBackBufferCount() const { return MAX_BACK_BUFFER_COUNT; }
        UINT getDeviceOptions() const { return mOptions; }
        const std::wstring& getAdapterDescription() const { return mAdapterDescription; }
        UINT getAdapterID() const { return mAdapterID; }
        CD3DX12_CPU_DESCRIPTOR_HANDLE getRenderTargetView()const {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mBackBufferIndex, mRTVDescriptorSize);
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE getDepthStencilView() const {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(mDSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        }
    private:
        void moveToNextFrame();
        void initializeAdapter(IDXGIAdapter1** adapter);
    private:
        static constexpr UINT ALLOW_TEARING = 0x1;
    private:
        UINT mAdapterIDoverride;
        UINT mBackBufferIndex;
        ComPtr<IDXGIAdapter1> mAdapter;
        UINT mAdapterID;
        std::wstring mAdapterDescription;

        ComPtr<ID3D12Device> mDevice;
        ComPtr<ID3D12CommandQueue> mCommandQueue;
        ComPtr<ID3D12GraphicsCommandList> mCommandList;
        std::array<ComPtr<ID3D12CommandAllocator>, MAX_BACK_BUFFER_COUNT> mCommandAllocators;

        ComPtr<IDXGIFactory4> mFactory;
        ComPtr<IDXGISwapChain3> mSwapChain;
        ComPtr<ID3D12Resource> mRenderTargets[MAX_BACK_BUFFER_COUNT];
        ComPtr<ID3D12Resource> mDepthStencil;

        ComPtr<ID3D12Fence> mFence;
        std::array<UINT64, MAX_BACK_BUFFER_COUNT> mFenceValues;
        Microsoft::WRL::Wrappers::Event mFenceEvent;
        ComPtr<ID3D12DescriptorHeap> mRTVDescriptorHeap;
        ComPtr<ID3D12DescriptorHeap> mDSVDescriptorHeap;
        UINT mRTVDescriptorSize;
        D3D12_VIEWPORT mScreenViewport;
        D3D12_RECT mScissorRect;

        DXGI_FORMAT mBackBufferFormat;
        DXGI_FORMAT mDepthBufferFormat;
        D3D_FEATURE_LEVEL mMinFeatureLevel;

        Window::Window* mWindow;
        D3D_FEATURE_LEVEL mFeatureLevel;
        RECT mOutputSize;
        bool mIsWindowVisible;

        UINT mOptions;
        IDeviceNotify* mDeviceNotify;
    };
} //Framework::DX
