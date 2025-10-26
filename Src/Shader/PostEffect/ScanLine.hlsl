#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float  g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float3 red = { 1.0f, 0.0f, 0.0f };
	float2 uv = PSInput.uv;

	// UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
	float4 srcCol = tex.Sample(texSampler, uv);
	//�����̐F�̋�����Ԃ̋����ɂ���
    red.r *= (srcCol.r + srcCol.g + srcCol.b) / 3.0f;
	
	
	// �c�̑傫���Ǝ��Ԃ�-1.0�`1.0�̒l�����
	float area = sin(uv.y * 2.0f - g_time * 0.5f);

	// �c�ɓ���͈͂����
	// 1 or 0 �c�c 1 <= 2 = 1�A2 <= 1 = 0 
	float isArea = step(0.996f, area * area);

    red.r -= abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
    red.r -= abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

	// ���G���A����
	// ------------------------------------------------------------------------------
	// ���G���A�ȊO�A�Ԋu���󂯂āA�ȁX�͗l(���n)�����(�F�̌��Z�ŐF���Â�����)
	//srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y *  60.0f + g_time * 1.0f)) * 0.05f;
	//srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

	// ����͈͂������邭����(�F�̉��Z�Ŗ��邭����)
	//srcCol.rgb += isArea * 0.5f;
	// ------------------------------------------------------------------------------

	// ���n����
	//srcCol.rgb -= abs(sin(uv.y *  60.0f + g_time * 1.0f)) * 0.10f;
	//srcCol.rgb -= abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

    return float4(red, 1.0f);

}
