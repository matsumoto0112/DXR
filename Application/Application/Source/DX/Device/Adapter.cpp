#include "Adapter.h"

namespace Framework::DX {
    //初期化
    void Adapter::init(IDXGIFactory4* factory) {
        mAdapter = nullptr;

        Comptr<IDXGIAdapter1> adapter;
        //使用可能なアダプターを探す
        for (UINT id = 0; factory->EnumAdapters1(id, &adapter) != DXGI_ERROR_NOT_FOUND; ++id) {
            DXGI_ADAPTER_DESC1 desc;
            MY_THROW_IF_FAILED(adapter->GetDesc1(&desc));
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) { continue; }

            if (SUCCEEDED(D3D12CreateDevice(
                    adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) {
                mAdapterID = id;
                mAdapterDescription = desc.Description;
#ifdef _DEBUG
                wchar_t buff[256] = {};
                swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", id,
                    desc.VendorId, desc.DeviceId, desc.Description);
                OutputDebugStringW(buff);
#endif
                break;
            }
        }

#ifdef _DEBUG
        if (!adapter) {
            if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)))) {
                throw std::exception(
                    "WARP12 not available. Enable the 'Graphics Tools' optional feature");
            }

            OutputDebugStringA("Direct3D Adapter - WARP12\n");
        }
#endif

        MY_THROW_IF_FALSE(adapter);
        mAdapter = adapter;
    }
    void Adapter::reset() {
        mAdapter.Reset();
        mAdapterID = UINT_MAX;
        mAdapterDescription.clear();
    }
} // namespace Framework::DX
