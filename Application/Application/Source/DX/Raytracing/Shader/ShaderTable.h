/**
 * @file ShaderTable.h
 * @brief シェーダーテーブル
 */

#pragma once
#include "DX/Raytracing/Shader/ShaderRecord.h"
#include "DX/Util/GPUUploadBuffer.h"

namespace Framework::DX {
    /**
     * @brief シェーダーテーブルクラス
     */
    class ShaderTable : public GPUUploadBuffer {
    public:
        /**
         * @brief コンストラクタ
         * @param device デバイス
         * @param shaderRecordNum リンクするシェーダー数
         * @param shaderRecordSize レコード一つの大きさ
         * @param name シェーダー名
         */
        ShaderTable(
            ID3D12Device* device, UINT shaderRecordNum, UINT shaderRecordSize, LPCWSTR name) {
            mRecordSize = Math::MathUtil::alignPow2(
                shaderRecordSize, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);
            mShaderRecords.reserve(shaderRecordNum);
            UINT bufferSize = shaderRecordNum * mRecordSize;
            allocate(device, bufferSize, name);
            mMapped = getMapCPUWriteOnly();
        }
        /**
         * @brief レコードを追加する
         */
        void push_back(const ShaderRecord& record) {
            MY_THROW_IF_FALSE(mShaderRecords.size() < mShaderRecords.capacity());
            mShaderRecords.emplace_back(record);
            record.copyTo(mMapped);
            mMapped += mRecordSize;
        }
        /**
         * @brief シェーダーレコードの大きさを取得する
         */
        UINT getShaderRecordSize() const { return mRecordSize; }

    private:
        uint8_t* mMapped; //!< 書き込み領域
        UINT mRecordSize; //!< レコードの大きさ
        std::vector<ShaderRecord> mShaderRecords; //!< シェーダーレコード
    };
} // namespace Framework::DX
