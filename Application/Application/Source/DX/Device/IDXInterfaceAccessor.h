/**
 * @file IDXInterfaceAccessor.h
 * @brief DXインターフェースへのアクセサの提供
 */

#pragma once

namespace Framework::DX {
    /**
     * @class IDXInterfaceAccessor
     * @brief DXインターフェースへのアクセサの提供
     */
    class IDXInterfaceAccessor {
    public:
        /**
         * @brief デストラクタ
         */
        virtual ~IDXInterfaceAccessor() = default;
        /**
         * @brief デバイスの取得
         */
        virtual ID3D12Device* getDevice() const = 0;
    };
} // namespace Framework::DX
