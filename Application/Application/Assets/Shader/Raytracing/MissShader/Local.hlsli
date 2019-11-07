#ifndef SHADER_RAYTRACING_MISS_LOCAL_HLSLI
#define SHADER_RAYTRACING_MISS_LOCAL_HLSLI

#define HLSL
#include "../Util/GlobalCompat.h"
#include "../Util/MissCompat.h"

//ミスシェーダー用コンスタントバッファ
ConstantBuffer<MissConstant> l_missCB : register(b1);

#endif //! SHADER_RAYTRACING_MISS_LOCAL_HLSLI