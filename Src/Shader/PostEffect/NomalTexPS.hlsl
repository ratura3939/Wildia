#include "../Common/Pixel/PixelShader2DHeader.hlsli"

float4 main(PS_INPUT PSInput) : SV_TARGET
{
   return tex.Sample(texSampler, PSInput.uv);
}