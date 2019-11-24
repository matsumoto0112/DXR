#pragma once
#include <Windows.h>
#include <string>

namespace Framework::Desc {
    /**
    * @enum HeapType
    * @brief ヒープの種類定義
    */
    enum class HeapType {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
    };

    /**
    * @enum HeapFlag
    * @brief ヒープのフラグ定義
    */
    enum class HeapFlag {
        None,
        ShaderVisible,
    };
    /**
    * @brief デーブルディスク
    */
    struct DescriptorTableDesc {
        std::wstring name; //!< テーブル名
        UINT descriptorNum; //!< 割り当てる数
        HeapType type; //!< 種類
        HeapFlag flag; //!< フラグ
    };
} // namespace Framework::Desc
