#include "DeviceResource.h"
#ifdef _DEBUG
#include <dxgidebug.h>
#endif
#include "DX/Util/DXHelper.h"
#include "Math/MathUtility.h"
#include "Utility/Debug.h"

namespace {
    static constexpr D3D_FEATURE_LEVEL MIN_FEATURE_LEVEL = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0; //最低要求機能レベル

    /**
    * @brief SRGBフォーマットを差し替える
    */
    inline DXGI_FORMAT noSRGB(DXGI_FORMAT fmt) {
        switch (fmt) {
            case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
            case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
            case DXGI_FORMAT::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT::DXGI_FORMAT_B8G8R8X8_UNORM;
            default:                                return fmt;
        }
    }

    //矩形の比較
    bool operator==(const RECT& r1, const RECT& r2) {
        return r1.left == r2.left
            && r1.top == r2.top
            && r1.right == r2.right
            && r1.bottom == r2.bottom;
    }
}

namespace Framework::DX {
    //コンストラクタ
    DeviceResource::DeviceResource(DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthBufferFormat, UINT flags)
        : mBackBufferIndex(0),
        mAdapterID(0),
        mAdapterDescription(),
        mFenceEvent{},
        mScreenViewport{},
        mScissorRect{},
        mBackBufferFormat(backBufferFormat),
        mDepthBufferFormat(depthBufferFormat),
        mFeatureLevel(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0),
        mOutputSize{},
        mIsWindowVisible(true),
        mOptions(flags),
        mWindow(nullptr),
        mDeviceNotify(nullptr)
    {
        //テアリングサポートが要求されているならオプションにテアリングを追加する
        if (mOptions & REQUIRE_TEARING_SUPPORT) {
            mOptions |= ALLOW_TEARING;
        }
    }
    //デストラクタ
    DeviceResource::~DeviceResource() {
        //終了前にGPUの処理を終わらせる
        waitForGPU();
    }
    //アダプタを初期化する
    void DeviceResource::initializeDXGIAdapter() {
        bool useDebug = false;
        //デバッグ時はデバッグレイヤーを有効にする
#ifdef _DEBUG
        ComPtr<ID3D12Debug> debug;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
            debug->EnableDebugLayer();
        }
        else {
            MY_DEBUG_LOG("WARNING:デバッグレイヤーが使用できません。");
        }
        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue)))) {
            useDebug = true;
            MY_THROW_IF_FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mFactory)));
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        }
#endif

        if (!useDebug) {
            MY_THROW_IF_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&mFactory)));
        }

        //テアリング
        if (mOptions & (ALLOW_TEARING | REQUIRE_TEARING_SUPPORT)) {
            BOOL allowTearing = FALSE;

            ComPtr<IDXGIFactory5> factory5;
            HRESULT hr = mFactory.As(&factory5);
            if (SUCCEEDED(hr)) {
                hr = factory5->CheckFeatureSupport(DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
            }
            if (FAILED(hr) || !allowTearing) {
                MY_DEBUG_LOG("WARNING: ディスプレイがテアリングに対応していません");
                if (mOptions & REQUIRE_TEARING_SUPPORT) {
                    MY_THROW_IF_FALSE_LOG(false, "テアリングに対応していないと実行できません");
                }
                mOptions &= ALLOW_TEARING;
            }
        }

        initializeAdapter(&mAdapter);
    }
    //デバイスリソースを作成する
    void DeviceResource::createDeviceResources() {
        MY_THROW_IF_FAILED(D3D12CreateDevice(mAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mDevice)));

#ifndef NDEBUG
        ComPtr<ID3D12InfoQueue> infoQueue;
        if (SUCCEEDED(mDevice.As(&infoQueue))) {
#ifdef _DEBUG
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY::D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY::D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D12_MESSAGE_ID hide[] =
            {
                D3D12_MESSAGE_ID::D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
                D3D12_MESSAGE_ID::D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
            };
            D3D12_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            infoQueue->AddStorageFilterEntries(&filter);
        }
#endif

        //機能レベルを調べる
        static constexpr D3D_FEATURE_LEVEL FEATURE_LEVELS[] =
        {
            D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS featLevels =
        {
            _countof(FEATURE_LEVELS),FEATURE_LEVELS,D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0
        };
        MY_THROW_IF_FAILED(mDevice->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_FEATURE_LEVELS, &featLevels, sizeof(featLevels)));
        mFeatureLevel = featLevels.MaxSupportedFeatureLevel;

        //コマンドキューを作成する
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
        MY_THROW_IF_FAILED(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

        //レンダーターゲットのディスクリプタヒープを作成する
        Desc::DescriptorTableDesc rtvHeapDesc = { L"RenderTargetHeap",BACK_BUFFER_COUNT,Desc::HeapType::RTV,Desc::HeapFlag::None };
        mRTVHeap = std::make_unique<DescriptorTable>(mDevice.Get(), rtvHeapDesc);

        //デプス・ステンシルを使用するならディスクリプタヒープを作成する
        if (mDepthBufferFormat != DXGI_FORMAT::DXGI_FORMAT_UNKNOWN) {
            Desc::DescriptorTableDesc dsvHeapDesc = { L"DepthStencilHeap",1,Desc::HeapType::DSV,Desc::HeapFlag::None };
            mDSVHeap = std::make_unique<DescriptorTable>(mDevice.Get(), dsvHeapDesc);
        }

        //バックバッファの枚数分アロケータを作成する
        for (UINT n = 0; n < BACK_BUFFER_COUNT; n++) {
            MY_THROW_IF_FAILED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocators[n])));
        }

        //コマンドリストを作成する
        MY_THROW_IF_FAILED(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
            mCommandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&mCommandList)));
        MY_THROW_IF_FAILED(mCommandList->Close());

        //フェンス作成
        MY_THROW_IF_FAILED(mDevice->CreateFence(mFenceValues[mBackBufferIndex], D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
        mFenceValues[mBackBufferIndex]++;
        mFenceEvent.Attach(CreateEvent(nullptr, FALSE, FALSE, nullptr));
        if (!mFenceEvent.IsValid()) {
            MY_THROW_IF_FAILED(E_FAIL);
        }
    }
    //ウィンドウに関連するリソース作成
    void DeviceResource::createWindowDependentResources() {
        MY_THROW_IF_FALSE_LOG(mWindow, "ウィンドウがセットされていません");
        waitForGPU();

        //レンダーターゲットを初期化する
        for (UINT n = 0; n < BACK_BUFFER_COUNT; n++) {
            mRenderTargets[n].Reset();
            mFenceValues[n] = mFenceValues[mBackBufferIndex];
        }

        UINT backBufferWidth = Math::MathUtil::mymax(mOutputSize.right - mOutputSize.left, 1L);
        UINT backBufferHeight = Math::MathUtil::mymax(mOutputSize.bottom - mOutputSize.top, 1L);
        DXGI_FORMAT backBufferFormat = noSRGB(mBackBufferFormat);

        //スワップチェインがもうあるなら再設定する
        if (mSwapChain) {
            HRESULT hr = mSwapChain->ResizeBuffers(
                BACK_BUFFER_COUNT,
                backBufferWidth,
                backBufferHeight,
                backBufferFormat,
                (mOptions & ALLOW_TEARING) ? DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);

            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
#ifdef _DEBUG
                MY_DEBUG_LOG("デバイスロスト:0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? mDevice->GetDeviceRemovedReason() : hr);
#endif
                handleDeviceLost();
                return;
            }
            else {
                MY_THROW_IF_FAILED(hr);
            }
        }
        //まだ存在しなければ新しく作成
        else {
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = BACK_BUFFER_COUNT;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_IGNORE;
            swapChainDesc.Flags = (mOptions & ALLOW_TEARING) ? DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenChainDesc = { 0 };
            fullScreenChainDesc.Windowed = TRUE;

            ComPtr<IDXGISwapChain1> swapChain;

            //フルスクリーン状態だと作成できないので一時的に後ろにする
            bool prevIsFullScreen = mWindow->isFullScreen();
            if (prevIsFullScreen) {
                mWindow->setWindowZOrderToTopMost(false);
            }

            MY_THROW_IF_FAILED(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), mWindow->getHWnd(), &swapChainDesc, &fullScreenChainDesc, nullptr, &swapChain));

            if (prevIsFullScreen) {
                mWindow->setWindowZOrderToTopMost(true);
            }

            MY_THROW_IF_FAILED(swapChain.As(&mSwapChain));

            if (isTearingSupported()) {
                mFactory->MakeWindowAssociation(mWindow->getHWnd(), DXGI_MWA_NO_ALT_ENTER);
            }
        }

        //バックバッファのレンダーターゲットの作成
        for (UINT n = 0; n < BACK_BUFFER_COUNT; n++) {
            MY_THROW_IF_FAILED(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n])));

            mRenderTargets[n]->SetName(Utility::format(L"RenderTarget[%u]", n).c_str());
            D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = mBackBufferFormat;
            rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

            D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor = mRTVHeap->getCPUHandle(n);
            mDevice->CreateRenderTargetView(mRenderTargets[n].Get(), &rtvDesc, rtvDescriptor);
        }

        mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

        //デプス・ステンシル作成
        if (mDepthBufferFormat != DXGI_FORMAT::DXGI_FORMAT_UNKNOWN) {
            CD3DX12_HEAP_PROPERTIES depthHeapProps(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
            D3D12_RESOURCE_DESC dsvResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
                mDepthBufferFormat,
                backBufferWidth,
                backBufferHeight,
                1,
                1);
            dsvResourceDesc.Flags |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

            D3D12_CLEAR_VALUE dsvClearValue = {};
            dsvClearValue.Format = mDepthBufferFormat;
            dsvClearValue.DepthStencil.Depth = 1.0f;
            dsvClearValue.DepthStencil.Stencil = 0;

            MY_THROW_IF_FAILED(mDevice->CreateCommittedResource(&depthHeapProps,
                D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &dsvResourceDesc,
                D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &dsvClearValue,
                IID_PPV_ARGS(&mDepthStencil)));

            mDepthStencil->SetName(L"DepthStencil");

            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
            dsvDesc.Format = mDepthBufferFormat;
            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

            mDevice->CreateDepthStencilView(mDepthStencil.Get(), &dsvDesc, mDSVHeap->getCPUHandle(0));
        }

        mScreenViewport.TopLeftX = mScreenViewport.TopLeftY = 0.0f;
        mScreenViewport.Width = static_cast<float>(backBufferWidth);
        mScreenViewport.Height = static_cast<float>(backBufferHeight);
        mScreenViewport.MinDepth = D3D12_MIN_DEPTH;
        mScreenViewport.MaxDepth = D3D12_MAX_DEPTH;

        mScissorRect.left = mScissorRect.top = 0;
        mScissorRect.right = backBufferWidth;
        mScissorRect.bottom = backBufferHeight;
    }
    //ウィンドウを設定する
    void DeviceResource::setWindow(Window::Window* window, UINT width, UINT height) {
        mWindow = window;
        mOutputSize.left = mOutputSize.top = 0;
        mOutputSize.right = width;
        mOutputSize.bottom = height;
    }
    //ウィンドウのサイズを変更
    bool DeviceResource::windowSizeChanged(UINT width, UINT height, bool minimized) {
        mIsWindowVisible = !minimized;

        if (minimized || width == 0 || height == 0) {
            return false;
        }

        RECT newRect;
        newRect.left = newRect.top = 0;
        newRect.right = width;
        newRect.bottom = height;

        //前と同じ大きさなら終了
        if (newRect == mOutputSize) {
            return false;
        }

        mOutputSize = newRect;
        createWindowDependentResources();
        return true;
    }
    //デバイスロスト発生
    void DeviceResource::handleDeviceLost() {
        //ロストしたことを通知
        if (mDeviceNotify) {
            mDeviceNotify->onDeviceLost();
        }

        for (UINT n = 0; n < BACK_BUFFER_COUNT; n++) {
            mCommandAllocators[n].Reset();
            mRenderTargets[n].Reset();
        }

        mDepthStencil.Reset();
        mCommandQueue.Reset();
        mCommandList.Reset();
        mFence.Reset();
        mRTVHeap->reset();
        mDSVHeap->reset();
        mSwapChain.Reset();
        mDevice.Reset();
        mFactory.Reset();
        mAdapter.Reset();

#ifdef _DEBUG
        {
            ComPtr<IDXGIDebug1> debug;
            if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
                debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
            }
        }
#endif
        initializeDXGIAdapter();
        createDeviceResources();
        createWindowDependentResources();

        //デバイスの復活を通知
        if (mDeviceNotify) {
            mDeviceNotify->onDeviceRestored();
        }

    }

    //デバイスイベントの通知先をセット
    void DeviceResource::registerDeviceNotify(IDeviceNotify* deviceNotify) {
        mDeviceNotify = deviceNotify;

        __if_exists(DXGIDeclareAdapterRemovalSupport) {
            if (deviceNotify) {
                if (FAILED(DXGIDeclareAdapterRemovalSupport())) {
                    MY_DEBUG_LOG("Warning: application failed to declare adapter removal support.\n");
                }
            }
        }
    }

    //描画準備
    void DeviceResource::prepare(D3D12_RESOURCE_STATES beforeState) {
        MY_THROW_IF_FAILED(mCommandAllocators[mBackBufferIndex]->Reset());
        MY_THROW_IF_FAILED(mCommandList->Reset(mCommandAllocators[mBackBufferIndex].Get(), nullptr));

        //レンダーターゲットのバリア
        if (beforeState != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET) {
            D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mBackBufferIndex].Get(),
                beforeState, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
            mCommandList->ResourceBarrier(1, &barrier);
        }
    }

    //描画プレゼント
    void DeviceResource::present(D3D12_RESOURCE_STATES beforeState) {
        if (beforeState != D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT) {
            D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mBackBufferIndex].Get(),
                beforeState, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
            mCommandList->ResourceBarrier(1, &barrier);
        }

        executeCommandList();

        HRESULT hr;
        //テアリング許容時はsyncIntervalを0にする
        if (mOptions & ALLOW_TEARING) {
            hr = mSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
        }
        else {
            //テアリング非許容時はVsyncを待機する
            hr = mSwapChain->Present(1, 0);
        }

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
#ifdef _DEBUG
            MY_DEBUG_LOG("プレゼント時にデバイスロスト発生:0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? mDevice->GetDeviceRemovedReason() : hr);
#endif
            handleDeviceLost();
        }
        else {
            MY_THROW_IF_FAILED(hr);
            moveToNextFrame();
        }
    }
    //コマンドを実行する
    void DeviceResource::executeCommandList() {
        MY_THROW_IF_FAILED(mCommandList->Close());
        ID3D12CommandList* lists[] = { mCommandList.Get() };
        mCommandQueue->ExecuteCommandLists(_countof(lists), lists);
    }
    //GPUの待機
    void DeviceResource::waitForGPU() noexcept {
        if (mCommandQueue && mFence && mFenceEvent.IsValid()) {
            UINT64 fenceValue = mFenceValues[mBackBufferIndex];
            if (SUCCEEDED(mCommandQueue->Signal(mFence.Get(), fenceValue))) {
                if (SUCCEEDED(mFence->SetEventOnCompletion(fenceValue, mFenceEvent.Get()))) {
                    WaitForSingleObjectEx(mFenceEvent.Get(), INFINITE, FALSE);
                    mFenceValues[mBackBufferIndex]++;
                }
            }
        }
    }
    //次のフレームの準備
    void DeviceResource::moveToNextFrame() {
        waitForGPU();
        const UINT64 currentFenceValue = mFenceValues[mBackBufferIndex];
        MY_THROW_IF_FAILED(mCommandQueue->Signal(mFence.Get(), currentFenceValue));
        mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

        //準備が完了するまで待機
        if (mFence->GetCompletedValue() < mFenceValues[mBackBufferIndex]) {
            MY_THROW_IF_FAILED(mFence->SetEventOnCompletion(mFenceValues[mBackBufferIndex], mFenceEvent.Get()));
            WaitForSingleObjectEx(mFenceEvent.Get(), INFINITE, FALSE);
        }

        mFenceValues[mBackBufferIndex] = currentFenceValue + 1;
    }
    //アダプターの初期化
    void DeviceResource::initializeAdapter(IDXGIAdapter1** adapter) {
        *adapter = nullptr;

        ComPtr<IDXGIAdapter1> adap;
        MY_THROW_IF_FAILED(mFactory->EnumAdapters1(0, &adap));
        DXGI_ADAPTER_DESC1 desc;
        MY_THROW_IF_FAILED(adap->GetDesc1(&desc));

#ifdef _DEBUG
        MY_DEBUG_LOG("Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", mAdapter, desc.VendorId, desc.DeviceId, desc.Description);
#endif
        if (!adap) {
            throw std::exception("Unavailable adapter.");
        }

        mAdapterDescription = desc.Description;
        *adapter = adap.Detach();
    }
} //Framework::DX
