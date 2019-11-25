/**
 * @file ShaderRecord.h
 * @brief シェーダーレコード管理
 */

#pragma once

namespace Framework::DX {
    /**
     * @brief シェーダーレコードクラス
     */
    class ShaderRecord {
    public:
        /**
         * @brief コンストラクタ
         * @param id シェーダーの先頭ID
         * @param size シェーダーの大きさ
         */
        ShaderRecord(void* id, UINT size) : shaderID(id, size) {}
        /**
         * @brief コンストラクタ
         * @param id シェーダーの先頭ID
         * @param size シェーダーの大きさ
         * @param localRootArgument ローカルアーギュメント
         * @param localRootArgumentSize ローカルアーギュメントの大きさ
         */
        ShaderRecord(void* id, UINT size, void* localRootArgument, UINT localRootArgumentSize)
            : shaderID(id, size), localRootArgument(localRootArgument, localRootArgumentSize) {}
        /**
         * @brief データをコピーする
         * @param dest コピー先
         */
        void copyTo(void* dest) const {
            uint8_t* byteDest = static_cast<uint8_t*>(dest);
            memcpy(byteDest, shaderID.ptr, shaderID.size);
            if (localRootArgument.ptr) {
                memcpy(byteDest + shaderID.size, localRootArgument.ptr, localRootArgument.size);
            }
        }

    private:
        /**
         * @struct PointerWithSize
         * @brief データのポインタとその大きさのペア
         */
        struct PointerWithSize {
            void* ptr; //!< ポインタ
            UINT size; //!< 大きさ
            PointerWithSize() : ptr(nullptr), size(0) {}
            PointerWithSize(void* ptr, UINT size) : ptr(ptr), size(size) {}
        };
        PointerWithSize shaderID; //!< シェーダーID
        PointerWithSize localRootArgument; //!< ローカルアーギュメント
    };
} // namespace Framework::DX
