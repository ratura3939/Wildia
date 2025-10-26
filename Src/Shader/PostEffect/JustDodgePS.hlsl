#include "../Common/Pixel/PixelShader2DHeader.hlsli"

SamplerState noiseSampler         : register(s11);      // ノイズテクスチャ
Texture2D    noiseTex             : register(t11);	    // ノイズテクスチャ

// 定数バッファ：スロット0番目(b0と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float g_time;
	float3 dummy_time;
	float g_sizeX;
	float g_sizeY;
	float g_rate;
	float g_radius;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	float2 uv = PSInput.uv;
	float4 srcCol = tex.Sample(texSampler, uv);
	float4 noiseV = noiseTex.Sample(texSampler, uv);

	// UV中心からの距離
	float2 center = float2(0.5f, 0.5f);
	float2 diff = uv - center;
	float dist = length(diff); // 0.0（中心）～ 約0.707（隅）

	// 時間で変化するアニメーション値（0→1→0 の周期）
	float time = g_time; // これは 0 から増え続ける外部変数
	float wave = 0.5 * (1 - cos(time * 3.14159)); // 1秒周期で変化（π rad/sec）

	// 赤枠のフェード効果（中心に近いほど減衰）
	float edge = smoothstep(0.1, 0.8, dist); // 半径0.2～0.5の赤エッジ

	// 最終色：時間に応じて赤がフェードイン・アウト
	float4 gray = float4(0.5, 0.5, 0.5, 1.0) * edge * wave;
	

	// 元のテクスチャ色と加算（または合成）
	//srcCol += redColor;

	float directions = 16.0f;

	// 時間によって広がるブラー距離
	//float g_rate = 0.02f + 0.03f * sin(g_time); // 0.02~0.05くらいの範囲で変動

	float quality = 3.0f;
	float twoPi = 6.28318530718;

	if (dist > g_radius) {
		for (float rad = 0.0f; rad < twoPi; rad += twoPi / directions)
		{
			for (int i = 1; i <= quality; ++i)
			{
				float dist = g_rate * (i / quality) * wave;

				srcCol += tex.Sample(texSampler,
					uv + float2(cos(rad), sin(rad)) * dist);
			}
		}
		srcCol /= (directions * quality);
	}
	
	/*for (float rad = 0.0f; rad < twoPi; rad += twoPi / directions)
	{
		for (int i = 1; i <= quality; ++i)
		{
			float dist = g_rate * (i / quality) * wave;

			srcCol += tex.Sample(texSampler,
				uv + float2(cos(rad), sin(rad)) * dist);
		}
	}
	srcCol /= (directions * quality);*/

	if (srcCol.a < 0.01f)
	{
		discard;
	}

	return float4((srcCol.rgb - gray.rgb),1.0f) * g_color;
}