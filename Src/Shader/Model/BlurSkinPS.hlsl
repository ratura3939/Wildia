// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;		//各参考

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

	//関数の戻り値がラスタライザに渡される
	return color * g_color;

	//********************************************************************************************


}