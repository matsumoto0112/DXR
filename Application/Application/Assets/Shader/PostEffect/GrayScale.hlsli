#ifndef SHADER_POSTEFFECT_GRAYSCALE_HLSLI
#define SHADER_POSTEFFECT_GRAYSCALE_HLSLI

Texture2D mainTexture : register(t0);
SamplerState samLinear : register(s0);

struct VSInput {
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

struct PSOutput {
    float4 color : SV_TARGET;
};

VSOutput vsMain(VSInput v) {
    VSOutput o = (VSOutput)0;
    o.pos = float4(v.pos, 1.0);
    o.uv = v.uv;
    return o;
}

PSOutput psMain(PSInput i) {
    PSOutput o = (PSOutput)0;
    /*flaot4 color = mainTexture.Sample(samLinear,i.uv);*/
    o.color = float4(1, 0, 0, 1);
    return o;
}

#endif //! SHADER_POSTEFFECT_GRAYSCALE_HLSLI
