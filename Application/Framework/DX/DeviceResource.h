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
    * @brief デバイスイベント通知
    */
    class IDeviceNotify {
    public:
        /**
        * @brief デストラクタ
        */
        virtual  ~IDeviceNotify() { }
        /**
        * @brief デバイスロスト
        */
        virtual void onDeviceLost() = 0;
        /**
        * @brief デバイスの再登録
        */
        virtual void onDeviceRestored() = 0;
    };

    /**
    * @class DeviceResource
    * @brief DX12デバイス管理
    */
    class DeviceResource {
    public:
        static constexpr UINT ALLOW_TEARING = 0x1; //!< テアリングを許可するフラグ
        static constexpr UINT REQUIRE_TEARING_SUPPORT = 0x2; //!< テアリングをサポートする際のフラグ
        static constexpr size_t BACK_BUFFER_COUNT = 3; //!< バックバッファの枚数
    public:
        /**
        * @brief コンストラクタ
        * @param backBufferFormat バックバッファのフォーマット
        * @param depthBufferFormat デプス・ステンシルバッファのフォーマット
        * @param flags スワップチェインのフラグ
        */
        DeviceResource(DXGI_FORMAT backBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT depthBufferFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
            UINT flags = 0);
        /**
        * @brief デストラクタ
        */
        ~DeviceResource();
        /**
        * @brief アダプターの初期化
        */
        void initializeDXGIAdapter();
        /**
        * @brief デバイスリソースの作成
        */
        void createDeviceResources();
        /**
        * @brief ウィンドウに依存するリソースの作成
        */
        void createWindowDependentResources();
        /**
        * @brief ウィンドウをセットする
        * @param[in] window ウィンドウ
        */
        void setWindow(Window::Window* window, UINT width, UINT height);
        /**
        * @brief ウィンドウのサイズを変更する
        * @param width ウィンドウ幅
        * @param height ウィンドウ高さ
        * @param minimized 最小化かどうか
        */
        bool windowSizeChanged(UINT width, UINT height, bool minimized);
        /**
        * @brief デバイスロスト
        */
        void handleDeviceLost();
        /**
        * @brief デバイスのイベントの通知先をセットする
        * @param deviceNotify 通知先
        */
        void registerDeviceNotify(IDeviceNotify* deviceNotify);
        /**
        * @brief 描画準備
        * @param beforeState 前のリソースの状態
        */
        void prepare(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
        /**
        * @brief プレゼント
        * @param beforeState 前のリソースの状態
        */
        void present(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
        /**
        * @brief コマンドを実行する
        */
        void executeCommandList();
        /**
        * @brief GPUの処理を待機する
        */
        void waitForGPU() noexcept;
    public:
        /**
        * @brief 出力先の矩形を取得する
        */
        RECT getOutputSize() const { return mOutputSize; }
        /**
        * @brief ウィンドウが可視状態か
        */
        bool isWindowVisible() const { return mIsWindowVisible; }
        /**
        * @brief テアリングサポートしているか
        */
        bool isTearingSupported() const { return mOptions & ALLOW_TEARING; }
        /**
        * @brief アダプタを取得する
        */
        IDXGIAdapter1* getAdapter() const { return mAdapter.Get(); }
        /**
        * @brief デバイスを取得する
        */
        ID3D12Device* getDevice()const { return mDevice.Get(); }
        /**
        * @brief ファクトリを取得する
        */
        IDXGIFactory4* getFactory() const { return mFactory.Get(); }
        /**
        * @brief スワップチェインを取得する
        */
        IDXGISwapChain3* getSwapChain() const { return mSwapChain.Get(); }
        /**
        * @brief 機能レベルを取得する
        */
        D3D_FEATURE_LEVEL getFeatureLevel() const { return mFeatureLevel; }
        /**
        * @brief レンダーターゲットを取得する
        */
        ID3D12Resource* getRenderTarget()const { return mRenderTargets[mBackBufferIndex].Get(); }
        /**
        * @brief デプス・ステンシルを取得する
        */
        ID3D12Resource* getDepthStencil() const { return mDepthStencil.Get(); }
        /**
        * @brief コマンドキューを取得する
        */
        ID3D12CommandQueue* getCommandQueue() const { return mCommandQueue.Get(); }
        /**
        * @brief コマンドアロケータを取得する
        */
        ID3D12CommandAllocator* getCommandAllocator() const { return mCommandAllocators[mBackBufferIndex].Get(); }
        /**
        * @brief コマンドリストを取得する
        */
        ID3D12GraphicsCommandList* getCommandList() const { return mCommandList.Get(); }
        /**
        * @brief バックバッファのフォーマットを取得する
        */
        DXGI_FORMAT getBackBufferFormat() const { return mBackBufferFormat; }
        /**
        * @brief デプス・ステンシルバッファのフォーマットを取得する
        */
        DXGI_FORMAT getDepthBufferFormat() const { return mDepthBufferFormat; }
        /**
        * @brief ビューポートを取得する
        */
        D3D12_VIEWPORT getScreenViewport() const { return mScreenViewport; }
        /**
        * @brief シザー矩形を取得する
        */
        D3D12_RECT getScissorRect() const { return mScissorRect; }
        /**
        * @brief 現在のフレームインデックスを取得する
        */
        UINT getCurrentFrameIndex() const { return mBackBufferIndex; }
        /**
        * @brief 前のフレームインデックスを取得する
        */
        UINT getPreviousFrameIndex() const { return mBackBufferIndex == 0 ? BACK_BUFFER_COUNT - 1 : mBackBufferIndex - 1; }
        /**
        * @brief バックバッファの枚数を取得する
        */
        UINT getBackBufferCount() const { return BACK_BUFFER_COUNT; }
        /**
        * @brief デバイスのオプションを取得する
        */
        UINT getDeviceOptions() const { return mOptions; }
        /**
        * @brief アダプタの説明を取得する
        */
        const std::wstring& getAdapterDescription() const { return mAdapterDescription; }
        /**
        * @brief アダプタIDを取得する
        */
        UINT getAdapterID() const { return mAdapterID; }
        /**
        * @brief レンダーターゲットのハンドルを取得する
        */
        D3D12_CPU_DESCRIPTOR_HANDLE getRenderTargetView()const {
            return mRTVHeap->getCPUHandle(mBackBufferIndex);
        }
        /**
        * @brief デプス・ステンシルのハンドルを取得する
        */
        CD3DX12_CPU_DESCRIPTOR_HANDLE getDepthStencilView() const {
            return mDSVHeap->getCPUHandle(0);
        }
    private:
        /**
        * @brief 次のフレームに遷移する
        */
        void moveToNextFrame();
        /**
        * @brief アダプタを初期化する
        */
        void initializeAdapter(IDXGIAdapter1** adapter);
    private:
        UINT mBackBufferIndex; //!< バックバッファのインデックス
        ComPtr<IDXGIAdapter1> mAdapter; //!< アダプタ
        UINT mAdapterID; //!< アダプタID
        std::wstring mAdapterDescription; //!< アダプタの説明
        ComPtr<ID3D12Device> mDevice; //!< デバイス
        ComPtr<ID3D12CommandQueue> mCommandQueue; //!< コマンドキュー
        ComPtr<ID3D12GraphicsCommandList> mCommandList; //!< コマンドリスト
        std::array<ComPtr<ID3D12CommandAllocator>, BACK_BUFFER_COUNT> mCommandAllocators; //!< コマンドアロケータ
        ComPtr<IDXGIFactory4> mFactory; //!< ファクトリ
        ComPtr<IDXGISwapChain3> mSwapChain; //!< スワップチェイン
        ComPtr<ID3D12Resource> mRenderTargets[BACK_BUFFER_COUNT]; //!< レンダーターゲット
        ComPtr<ID3D12Resource> mDepthStencil; //!< デプス・ステンシル
        ComPtr<ID3D12Fence> mFence; //!< フェンス
        std::array<UINT64, BACK_BUFFER_COUNT> mFenceValues; //!< フェンスの値
        Microsoft::WRL::Wrappers::Event mFenceEvent; //!< フェンスイベント
        std::unique_ptr<DescriptorTable> mRTVHeap; //!< レンダーターゲットのディスクリプタヒープ
        std::unique_ptr<DescriptorTable> mDSVHeap; //!< デプス・ステンシルのディスクリプタヒープ
        D3D12_VIEWPORT mScreenViewport; //!< ビューポート
        D3D12_RECT mScissorRect; //!< シザー矩形
        DXGI_FORMAT mBackBufferFormat; //!< バックバッファのフォーマット
        DXGI_FORMAT mDepthBufferFormat; //!< デプス・ステンシルフォーマット
        D3D_FEATURE_LEVEL mFeatureLevel; //!< 機能レベル
        RECT mOutputSize; //!< 出力先の矩形
        bool mIsWindowVisible; //!< ウィンドウが可視状態か
        UINT mOptions; //!< デバイスのオプション
        Window::Window* mWindow; //!< ウィンドウ
        IDeviceNotify* mDeviceNotify; //!< デバイスイベントの通知先
    };
} //Framework::DX
