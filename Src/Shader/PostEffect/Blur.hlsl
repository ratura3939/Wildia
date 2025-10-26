#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;	//拡散光
	float  g_time;	//時間
	float3 dummmy_time;
	float2 g_screen_size;	//スクリーンの大きさ
	float2 dumme_size;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 画面中央のUV座標
   float2 center = float2(0.5, 0.5);

   // 中心からの距離（正規化）
   float2 delta = PSInput.uv - center;
   float dist = length(delta);

   // 時間と距離に応じた波形を作る（咆哮の波）
   float wave = sin(dist * 50.0 - g_time * 10.0) * 0.02 / (dist * 10.0 + 1.0);

   // UVをゆがませる
   float2 distortedUV = PSInput.uv + normalize(delta) * wave;

   // 波紋の強さ（中心から広がる光のリング的なもの）
   float ring = exp(-pow((dist - g_time * 0.2), 2.0) * 100.0);

   // 元の色を取得
   float4 col = tex.Sample(texSampler, distortedUV);

   // 光のリングを加算
   col.rgb += ring;

   return col;

}