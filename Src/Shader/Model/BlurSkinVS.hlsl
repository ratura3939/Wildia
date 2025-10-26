//�X�L�����b�V��*�u���[�Ή�
// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_8FRAME

#define L_W_MAT g_localWorldMatrix.lwMatrix

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"


cbuffer BoneMat:register(b7) {
	float4x4 g_currentLocalWorldMatrix[256]; // ���݂̃{�[���s��
	float4x4 g_prevLocalWorldMatrix[256];    // �O�t���[���̃{�[���s��
}

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	// // --- ���݂̃t���[���̃X�L�����b�V���ϊ��s����\�z ---
	////// BONE4
	//float4x4 current_CombinedBoneMatrix = (float4x4)0;
	//int4 lBoneIdx = VSInput.blendIndices0;
	//float4 lWeight = VSInput.blendWeight0;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.x] * lWeight.xxxx;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.y] * lWeight.yyyy;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.z] * lWeight.zzzz;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.w] * lWeight.wwww;

	////// BONE8
	//lBoneIdx = VSInput.blendIndices1;
	//lWeight = VSInput.blendWeight1;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.x] * lWeight.xxxx;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.y] * lWeight.yyyy;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.z] * lWeight.zzzz;
	//current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.w] * lWeight.wwww;

	//// ���݂̃��[���h���W
	//float4 lCurrentWorldPosition = mul(lLocalPosition, current_CombinedBoneMatrix);
	//ret.worldPos = lCurrentWorldPosition.xyz;
	//ret.svPos = mul(lCurrentWorldPosition, g_currentViewProjectionMatrix); 

	// �X�L�����b�V���p�̃��[�J���˃��[���h�ϊ��s����쐬+++( �J�n )
	int4 lBoneIdx;
	float4 lL_W_Mat[3];
	float4 lWeight;
	// BONE4
	lBoneIdx = VSInput.blendIndices0;
	lWeight = VSInput.blendWeight0;
	lL_W_Mat[0] = L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
	lL_W_Mat[1] = L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
	lL_W_Mat[2] = L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;
	// BONE8
	lBoneIdx = VSInput.blendIndices1;
	lWeight = VSInput.blendWeight1;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;
	// �X�L�����b�V���p�̃��[�J���˃��[���h�ϊ��s����쐬+++( �I�� 


	// ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �J�n )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	// float3 �� float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;

	//lLocalPosition.xyz += (VSInput.norm * 2.0f);

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.w = 1.0f;
	//lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	lWorldPosition.x = dot(lLocalPosition, lL_W_Mat[0]);
	lWorldPosition.y = dot(lLocalPosition, lL_W_Mat[1]);
	lWorldPosition.z = dot(lLocalPosition, lL_W_Mat[2]);
	ret.worldPos = lWorldPosition.xyz;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;

	// �r���[���W���ˉe���W�ɕϊ�
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);


	// ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �J�n )
	// UV���W
	ret.uv.x = VSInput.uv0.x;
	ret.uv.y = VSInput.uv0.y;

	// �@��
	// �@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
	//ret.normal = normalize(
	//mul(VSInput.norm, (float3x3)g_base.localWorldMatrix));
	ret.normal.x = float(dot(VSInput.norm, lL_W_Mat[0]));
	ret.normal.y = float(dot(VSInput.norm, lL_W_Mat[1]));
	ret.normal.z = float(dot(VSInput.norm, lL_W_Mat[2]));

	// �f�B�t���[�Y�J���[
	ret.diffuse = VSInput.diffuse;
	// ���C�g����(���[�J��)
	ret.lightDir = float3(0.0f, 0.0f, 0.0f);
	// ���C�g���猩�����W
	ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
	// ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �I��




	// ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �I�� )
	// �o�̓p�����[�^��Ԃ�

	return ret;
}