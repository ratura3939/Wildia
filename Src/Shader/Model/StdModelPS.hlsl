//シンプルな光の計算も行うPS

// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float4 g_light;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;

	//テクスチャーの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler,PSInput.uv);
	if (color.a < 0.01f) {
		discard;
	}

	//どちらもワールド座標
	//法線
	float3 nomal = PSInput.normal;
	//ライト方向
	//float3 lightDir = g_light.xyz;
	//lightDir.xyz = 1.0f;

	//float dotF = dot(nomal,lightDir);
	//dotF += 1.0f;
	//dotF /= 2.0f;

	//color.rgb *= (1 - dotF);

	//関数の戻り値がラスタライザに渡される
	return color * g_color;
}