// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
	float4 g_color;		//�e�Q�l

}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;

	//�e�N�X�`���[�̐F���擾
	color = diffuseMapTexture.Sample(diffuseMapSampler,PSInput.uv);
	if (color.a < 0.01f) {
		discard;
	}

	//�ǂ�������[���h���W
	//�@��
	float3 nomal = PSInput.normal;

	//�֐��̖߂�l�����X�^���C�U�ɓn�����
	return color * g_color;

	//********************************************************************************************


}