#pragma once

namespace Framework::DX {
    namespace DepthStencilFormat {
        static constexpr DXGI_FORMAT shaderResourceViewFormat(DXGI_FORMAT dsvFormat) {
            switch (dsvFormat) {
            case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT: return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
            case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
                return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            default: return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
            }
        }
        static constexpr DXGI_FORMAT bufferFormat(DXGI_FORMAT dsvFormat) {
            switch (dsvFormat) {
            case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT: return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
            case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
                return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
            default: return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
            }
        }
    } // namespace DepthStencilFormat
} // namespace Framework::DX