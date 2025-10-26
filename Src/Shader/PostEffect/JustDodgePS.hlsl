#include "../Common/Pixel/PixelShader2DHeader.hlsli"

SamplerState noiseSampler         : register(s11);      // �m�C�Y�e�N�X�`��
Texture2D    noiseTex             : register(t11);	    // �m�C�Y�e�N�X�`��

// �萔�o�b�t�@�F�X���b�g0�Ԗ�(b0�Ə���)
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

	// UV���S����̋���
	float2 center = float2(0.5f, 0.5f);
	float2 diff = uv - center;
	float dist = length(diff); // 0.0�i���S�j�` ��0.707�i���j

	// ���Ԃŕω�����A�j���[�V�����l�i0��1��0 �̎����j
	float time = g_time; // ����� 0 ���瑝��������O���ϐ�
	float wave = 0.5 * (1 - cos(time * 3.14159)); // 1�b�����ŕω��i�� rad/sec�j

	// �Ԙg�̃t�F�[�h���ʁi���S�ɋ߂��قǌ����j
	float edge = smoothstep(0.1, 0.8, dist); // ���a0.2�`0.5�̐ԃG�b�W

	// �ŏI�F�F���Ԃɉ����ĐԂ��t�F�[�h�C���E�A�E�g
	float4 gray = float4(0.5, 0.5, 0.5, 1.0) * edge * wave;
	

	// ���̃e�N�X�`���F�Ɖ��Z�i�܂��͍����j
	//srcCol += redColor;

	float directions = 16.0f;

	// ���Ԃɂ���čL����u���[����
	//float g_rate = 0.02f + 0.03f * sin(g_time); // 0.02~0.05���炢�͈̔͂ŕϓ�

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