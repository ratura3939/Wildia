#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
	float4 g_color;	//�g�U��
	float  g_time;	//����
	float3 dummmy_time;
	float2 g_screen_size;	//�X�N���[���̑傫��
	float2 dumme_size;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // ��ʒ�����UV���W
   float2 center = float2(0.5, 0.5);

   // ���S����̋����i���K���j
   float2 delta = PSInput.uv - center;
   float dist = length(delta);

   // ���ԂƋ����ɉ������g�`�����i���K�̔g�j
   float wave = sin(dist * 50.0 - g_time * 10.0) * 0.02 / (dist * 10.0 + 1.0);

   // UV���䂪�܂���
   float2 distortedUV = PSInput.uv + normalize(delta) * wave;

   // �g��̋����i���S����L������̃����O�I�Ȃ��́j
   float ring = exp(-pow((dist - g_time * 0.2), 2.0) * 100.0);

   // ���̐F���擾
   float4 col = tex.Sample(texSampler, distortedUV);

   // ���̃����O�����Z
   col.rgb += ring;

   return col;

}